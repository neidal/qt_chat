#include "crashhandler.h"
#include "linux_syscall_support.h"
#include "linux_libc_support.h"
#include "line_reader.h"
#include "signal_util.h"
#include "simple_string_builder.h"

static const char kPackagePrefix[] = "/data/data/com.ttsdk/lib/";


static char procfs_path[NAME_MAX] = { 0 };

CCrashHandler * CCrashHandler::instance()
{
	pid_t pid = getpid();
	snprintf(procfs_path, sizeof(procfs_path), "/proc/%d", pid);

	static CCrashHandler crashhandler(pid, procfs_path);
	crashhandler.Init();

	return &crashhandler;
}

static OnSignalCallback SignalCallback = NULL;
static struct sigaction old_sa[4] = { 0 };

static void sigaction_handler(int sig, siginfo_t *siginfo, void *ucontext)
{
	CCrashHandler::instance()->Init();
	CSimpleStringBuilder sb(4096);

	// "signal %d (%s), %d (%s), fault addr %08x\n"
	sb.appendStringWithFmt("signal %d", sig);
	const char *name = getSignalName(sig);
	if (name != NULL)
		sb.appendStringWithFmt(" (%s)", name);
	sb.appendStringWithFmt(", code %d", siginfo->si_code);
	name = getSignalCodeName(sig, siginfo->si_code);
	if (name != NULL)
		sb.appendStringWithFmt(" (%s)", name);
	uintptr_t faultaddr = (uintptr_t) siginfo->si_addr;
	if (sig == SIGILL || sig == SIGFPE || sig == SIGSEGV || sig == SIGBUS)
		sb.appendStringWithFmt(", fault addr %08x", faultaddr);
	sb.appendString("\n");

	if (ucontext != NULL)
	{
		ucontext_t *context = (ucontext_t *) ucontext;
		sigcontext *sigctx = &(context->uc_mcontext);

		// "  r0 %08x  r1 %08x  r2 %08x  r3 %08x"
		sb.appendStringWithFmt("\n  r0 %08x  r1 %08x  r2 %08x  r3 %08x\n",
				sigctx->arm_r0, sigctx->arm_r1, sigctx->arm_r2, sigctx->arm_r3);
		sb.appendStringWithFmt("  r4 %08x  r5 %08x  r6 %08x  r7 %08x\n",
				sigctx->arm_r4, sigctx->arm_r5, sigctx->arm_r6, sigctx->arm_r7);
		sb.appendStringWithFmt("  r8 %08x  r9 %08x  10 %08x  fp %08x\n",
				sigctx->arm_r8, sigctx->arm_r9, sigctx->arm_r10,
				sigctx->arm_fp);
		sb.appendStringWithFmt(
				"  ip %08x  sp %08x  lr %08x  pc %08x  cpsr %08x\n",
				sigctx->arm_ip, sigctx->arm_sp, sigctx->arm_lr, sigctx->arm_pc,
				sigctx->arm_cpsr);

		sb.appendString("\ncall stack:\n");
		// "  %08x  %s + %08x"
		const MappingInfo *mapInfo;
		uintptr_t pc = context->uc_mcontext.arm_pc;
		sb.appendStringWithFmt("  %08x", pc);
		if (pc != 0)
		{
			mapInfo = CCrashHandler::instance()->FindMapping((void *) pc);
			if (mapInfo != NULL && mapInfo->name[0] != '\0')
			{
				sb.appendStringWithFmt("  %s + %08x", mapInfo->name,
						pc - mapInfo->start_addr);
			}
		}
		sb.appendString("\n");

		// 参考资料： http://blog.chinaunix.net/uid-16459552-id-3364761.html
		// arm apcs 调用堆栈，编译时需加参数 -mapcs-frams
		//   mainfunc  |    PC    | <-----------|
		//             |    LR    |             |
		//             |    SP    |             |
		//             |    FP    |             |
		//             |    ...   |             |
		//   subfunc   |    PC    | <--FP  <-|  |
		//             |    LR    |          |  |
		//             |    SP    | ---------|  |
		//             |    FP    | ------------|
		//             |    ...   |
		unsigned long cpsr_mode = sigctx->arm_cpsr & MODE_MASK;
		if (cpsr_mode == USR_MODE)
		{
			unsigned long ufp; /* Upper level fp */
			unsigned long ulr; /* Upper level lr */
			unsigned long s_sp; /* Start of sp */
			unsigned long e_sp; /* End of sp */
			unsigned long usp;
			s_sp = sigctx->arm_sp;
			e_sp = s_sp + CONFIG_STACK_DEPTH;
			ufp = sigctx->arm_fp;
			if ((ufp >= s_sp) && (ufp <= e_sp))
			{
				int stacknum = 0;

				while (true)
				{
					ulr = *(unsigned long *) (ufp - 4);
					usp = *(unsigned long *) (ufp - 8);
					if (usp != ufp + 4)
						break;

					// "  %08x  %s + %08x"
					sb.appendStringWithFmt("  %08x", ulr);
					mapInfo = CCrashHandler::instance()->FindMapping((void *) ulr);
					if (mapInfo != NULL && mapInfo->name[0] != '\0')
					{
						sb.appendStringWithFmt("  %s + %08x", mapInfo->name,
								ulr - mapInfo->start_addr);
					}
					sb.appendString("\n");

					ufp = *(unsigned long *) (ufp - 12);
					if ((ufp > e_sp) || (ufp < s_sp))
						break;

					stacknum++;
					if (stacknum >= CONFIG_STACK_MAX_NUM)
					{
						sb.appendString("  ...more...\n");
						break;
					}
				}
			}
		}
	}

	if (SignalCallback != NULL) {
		SignalCallback(sig, sb.getString());
	}

	int index = -1;
	switch (sig)
	{
	case SIGILL:
		index = 0;
		break;
	case SIGFPE:
		index = 1;
		break;
	case SIGSEGV:
		index = 2;
		break;
	case SIGBUS:
		index = 3;
		break;
	}

	if (index != -1)
	{
		if ((old_sa[index].sa_flags & SA_SIGINFO) != 0)
		{
			if (old_sa[index].sa_sigaction != NULL)
				old_sa[index].sa_sigaction(sig, siginfo, ucontext);
		}
		else
		{
			if (old_sa[index].sa_handler != NULL)
				old_sa[index].sa_handler(sig);
		}
	}
}

void CCrashHandler::Install(OnSignalCallback callback)
{
	SignalCallback = callback;

	struct sigaction sa = { 0 };
	sa.sa_sigaction = sigaction_handler;
	sa.sa_flags = SA_SIGINFO;

#define CATCHSIG(X, I) sigaction(X, &sa, &old_sa[I])
	CATCHSIG(SIGILL, 0);
	CATCHSIG(SIGFPE, 1);
	CATCHSIG(SIGSEGV, 2);
	CATCHSIG(SIGBUS, 3);
}

void CCrashHandler::Uninstall()
{
	sigaction(SIGILL, &old_sa[0], NULL);
	sigaction(SIGFPE, &old_sa[1], NULL);
	sigaction(SIGSEGV, &old_sa[2], NULL);
	sigaction(SIGBUS, &old_sa[3], NULL);

	my_memset(old_sa, 0, sizeof(old_sa));
}

bool CCrashHandler::Init()
{
	if (!auxv_.empty())
	{
		auxv_.clear();
		auxv_.resize(AT_MAX + 1);
	}
	if (!mappings_.empty())
		mappings_.clear();
	cacheMapping_ = NULL;

	return ReadAuxv() && EnumerateThreads() && EnumerateMappings();
}

const MappingInfo* CCrashHandler::FindMapping(const void* address)
{
	const uintptr_t addr = (uintptr_t) address;

	if (cacheMapping_ != NULL)
	{
		const uintptr_t start = static_cast<uintptr_t>(cacheMapping_->start_addr);
		if (addr >= start && addr - start < cacheMapping_->size)
			return cacheMapping_;
		cacheMapping_ = NULL;
	}

	for (size_t i = 0; i < mappings_.size(); ++i)
	{
		const uintptr_t start = static_cast<uintptr_t>(mappings_[i]->start_addr);
		if (addr >= start && addr - start < mappings_[i]->size) {
			cacheMapping_ = mappings_[i];
			return cacheMapping_;
		}
	}

	return NULL;
}

bool CCrashHandler::ReadAuxv()
{
	char auxv_path[NAME_MAX];
	if (!BuildProcPath(auxv_path, pid_, "auxv"))
	{
		return false;
	}

	int fd = sys_open(auxv_path, O_RDONLY, 0);
	if (fd < 0)
	{
		return false;
	}

	elf_aux_entry one_aux_entry;
	bool res = false;
	while (sys_read(fd, &one_aux_entry, sizeof(elf_aux_entry))
			== sizeof(elf_aux_entry) && one_aux_entry.a_type != AT_NULL)
	{
		if (one_aux_entry.a_type <= AT_MAX)
		{
			auxv_[one_aux_entry.a_type] = one_aux_entry.a_un.a_val;
			res = true;
			break;
		}
	}
	sys_close(fd);
	return res;
}

// This function wraps sys_readlink() and performs the same functionalty,
// but guarantees |buffer| is NULL-terminated if sys_readlink() returns
// no error. It takes the same arguments as sys_readlink(), but unlike
// sys_readlink(), it returns true on success.
//
// |buffer_size| specifies the size of |buffer| in bytes. As this function
// always NULL-terminates |buffer| on success, |buffer_size| should be
// at least one byte longer than the expected path length (e.g. PATH_MAX,
// which is typically defined as the maximum length of a path name
// including the NULL byte).
//
// The implementation of this function calls sys_readlink() instead of
// readlink(), it can thus be used in the context where calling to libc
// functions is discouraged.
bool SafeReadLink(const char *path, char *buffer, size_t buffer_size)
{
	// sys_readlink() does not add a NULL byte to |buffer|. In order to return
	// a NULL-terminated string in |buffer|, |buffer_size| should be at least
	// one byte longer than the expected path length. Also, sys_readlink()
	// returns the actual path length on success, which does not count the
	// NULL byte, so |result_size| should be less than |buffer_size|.
	ssize_t result_size = sys_readlink(path, buffer, buffer_size);
	if (result_size >= 0 && static_cast<size_t>(result_size) < buffer_size)
	{
		buffer[result_size] = '\0';
		return true;
	}
	return false;
}

// Same as the three-argument version of SafeReadLink() but deduces the
// size of |buffer| if it is a char array of known size.
template <size_t N>
bool SafeReadLink(const char* path, char (&buffer)[N]) {
  return SafeReadLink(path, buffer, sizeof(buffer));
}

static const char kDeletedSuffix[] = " (deleted)";

bool CCrashHandler::HandleDeletedFileInMapping(char *path) const
{
	static const size_t kDeletedSuffixLen = sizeof(kDeletedSuffix) - 1;

	// Check for ' (deleted)' in |path|.
	// |path| has to be at least as long as "/x (deleted)".
	const size_t path_len = my_strlen(path);
	if (path_len < kDeletedSuffixLen + 2)
		return false;
	if (my_strncmp(path + path_len - kDeletedSuffixLen, kDeletedSuffix, kDeletedSuffixLen) != 0)
		return false;

	// Check |path| against the /proc/pid/exe 'symlink'.
	char exe_link[NAME_MAX];
	char new_path[NAME_MAX];
	if (!BuildProcPath(exe_link, pid_, "exe"))
		return false;
	if (!SafeReadLink(exe_link, new_path))
		return false;
	if (my_strcmp(path, new_path) != 0)
		return false;

	// Check to see if somone actually named their executable 'foo (deleted)'.
	struct kernel_stat exe_stat;
	struct kernel_stat new_path_stat;
	if (sys_stat(exe_link, &exe_stat) == 0 &&
		sys_stat(new_path, &new_path_stat) == 0 &&
		exe_stat.st_dev == new_path_stat.st_dev &&
		exe_stat.st_ino == new_path_stat.st_ino)
	{
		return false;
	}

	my_memcpy(path, exe_link, NAME_MAX);
	return true;
}

bool CCrashHandler::HandleTrimPackagePrefixInMapping(char *path) const
{
	static const size_t kPackagePrefixLen = sizeof(kPackagePrefix) - 1;

	const size_t path_len = my_strlen(path);
	if (path_len < kPackagePrefixLen + 1)
		return false;
	if (my_strncmp(path, kPackagePrefix, kPackagePrefixLen) != 0)
		return false;

	my_memcpy(path, path + kPackagePrefixLen, path_len - kPackagePrefixLen);
	path[path_len - kPackagePrefixLen] = '\0';
	return true;
}

bool CCrashHandler::EnumerateMappings()
{
	char maps_path[NAME_MAX];
	if (!BuildProcPath(maps_path, pid_, "maps"))
		return false;

	// linux_gate_loc is the beginning of the kernel's mapping of
	// linux-gate.so in the process.  It doesn't actually show up in the
	// maps list as a filename, but it can be found using the AT_SYSINFO_EHDR
	// aux vector entry, which gives the information necessary to special
	// case its entry when creating the list of mappings.
	// See http://www.trilithium.com/johan/2005/08/linux-gate/ for more
	// information.
	const void* linux_gate_loc =
			reinterpret_cast<void *>(auxv_[AT_SYSINFO_EHDR]);
	// Although the initial executable is usually the first mapping, it's not
	// guaranteed (see http://crosbug.com/25355); therefore, try to use the
	// actual entry point to find the mapping.
	const void* entry_point_loc = reinterpret_cast<void *>(auxv_[AT_ENTRY]);

	const int fd = sys_open(maps_path, O_RDONLY, 0);
	if (fd < 0)
		return false;
	LineReader* const line_reader = new LineReader(fd);

	const char* line;
	unsigned line_len;
	while (line_reader->GetNextLine(&line, &line_len))
	{
		uintptr_t start_addr, end_addr, offset;

		const char* i1 = my_read_hex_ptr(&start_addr, line);
		if (*i1 == '-')
		{
			const char* i2 = my_read_hex_ptr(&end_addr, i1 + 1);
			if (*i2 == ' ')
			{
				const char* i3 = my_read_hex_ptr(&offset,
						i2 + 6 /* skip ' rwxp ' */);
				if (*i3 == ' ')
				{
					const char* name = NULL;
					// Only copy name if the name is a valid path name, or if
					// it's the VDSO image.
					if (((name = my_strchr(line, '/')) == NULL)
							&& linux_gate_loc
							&& reinterpret_cast<void*>(start_addr)
									== linux_gate_loc)
					{
						name = kLinuxGateLibraryName;
						offset = 0;
					}
					// Merge adjacent mappings with the same name into one module,
					// assuming they're a single library mapped by the dynamic linker
					if (name && !mappings_.empty())
					{
						MappingInfo* module = mappings_.back();
						if ((start_addr == module->start_addr + module->size)
								&& (my_strlen(name) == my_strlen(module->name))
								&& (my_strncmp(name, module->name,
										my_strlen(name)) == 0))
						{
							module->size = end_addr - module->start_addr;
							line_reader->PopLine(line_len);
							continue;
						}
					}
					MappingInfo* const module = new MappingInfo;
					my_memset(module, 0, sizeof(MappingInfo));
					module->start_addr = start_addr;
					module->size = end_addr - start_addr;
					module->offset = offset;
					if (name != NULL)
					{
						const unsigned l = my_strlen(name);
						if (l < sizeof(module->name)) {
							my_memcpy(module->name, name, l);

//							HandleDeletedFileInMapping(module->name);
							HandleTrimPackagePrefixInMapping(module->name);
						}
					}
					// If this is the entry-point mapping, and it's not already the
					// first one, then we need to make it be first.  This is because
					// the minidump format assumes the first module is the one that
					// corresponds to the main executable (as codified in
					// processor/minidump.cc:MinidumpModuleList::GetMainModule()).
					if (entry_point_loc
							&& (entry_point_loc
									>= reinterpret_cast<void*>(module->start_addr))
							&& (entry_point_loc
									< reinterpret_cast<void*>(module->start_addr
											+ module->size))
							&& !mappings_.empty())
					{
						// push the module onto the front of the list.
						mappings_.resize(mappings_.size() + 1);
						for (size_t idx = mappings_.size() - 1; idx > 0; idx--)
							mappings_[idx] = mappings_[idx - 1];
						mappings_[0] = module;
					}
					else
					{
						mappings_.push_back(module);
					}
				}
			}
		}
		line_reader->PopLine(line_len);
	}

	sys_close(fd);

	return !mappings_.empty();
}

bool CCrashHandler::EnumerateThreads()
{
	// TODO
	return true;
}

bool CCrashHandler::BuildProcPath(char* path, pid_t pid, const char* node) const
{
	if (!path || !node)
		return false;

	size_t node_len = my_strlen(node);
	if (node_len == 0)
		return false;

	size_t procfs_path_len = my_strlen(procfs_path_);
	size_t total_length = procfs_path_len + 1 + node_len;
	if (total_length >= NAME_MAX)
		return false;

	my_memcpy(path, procfs_path_, procfs_path_len);
	path[procfs_path_len] = '/';
	my_memcpy(path + procfs_path_len + 1, node, node_len);
	path[total_length] = '\0';
	return true;
}
