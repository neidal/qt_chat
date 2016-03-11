/*
 * CrashHandler.h
 *
 *  Created on: 2013-8-2
 *      Author: liaofeng
 *
 *	部分代码参考 google breakpad
 *	http://google-breakpad.googlecode.com/svn/trunk
 */

#ifndef CRASHHANDLER_H_
#define CRASHHANDLER_H_

#include <linux/limits.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <stddef.h>
#include <elf.h>
#include <vector>

#include "linux_syscall_support.h"

// 最大回溯堆栈32K以内的调用帧
#define CONFIG_STACK_DEPTH		0x8000
// 最大回溯10条调用堆栈
#define CONFIG_STACK_MAX_NUM	10

typedef struct {
	uintptr_t start_addr;
	size_t size;
	size_t offset; // offset into the backed file.
	char name[NAME_MAX];
}MappingInfo;

//typedef struct {
//	uint32_t a_type;
//	union {
//		uint32_t a_val;
//	} a_un;
//} Elf32_auxv_t;

typedef Elf32_auxv_t elf_aux_entry;
typedef typeof(((elf_aux_entry*) 0)->a_un.a_val) elf_aux_val_t;

// When we find the VDSO mapping in the process's address space, this
// is the name we use for it when writing it to the minidump.
// This should always be less than NAME_MAX!
const char kLinuxGateLibraryName[] = "linux-gate.so";

#define AT_SYSINFO_EHDR 33

// All interesting auvx entry types are below AT_SYSINFO_EHDR
#define AT_MAX AT_SYSINFO_EHDR

//
typedef void (*OnSignalCallback)(int sig, const char *info);


class CCrashHandler {
public:
	static CCrashHandler *instance();

	static void Install(OnSignalCallback callback);
	static void Uninstall();

	bool Init();

	const MappingInfo* FindMapping(const void* address);

protected:
	// Constructs a dumper for extracting information of a given process
	// with a process ID of |pid| via its core dump file at |core_path| and
	// its proc files at |procfs_path|. If |procfs_path| is a copy of
	// /proc/<pid>, it should contain the following files:
	//     auxv, cmdline, environ, exe, maps, status
	CCrashHandler(pid_t pid, const char* procfs_path) :
			pid_(pid), procfs_path_(procfs_path), mappings_(), auxv_(AT_MAX + 1) {
	};

	bool ReadAuxv();

	bool EnumerateMappings();

	// Implements LinuxDumper::EnumerateThreads().
	// Enumerates all threads of the given process into |threads_|.
	bool EnumerateThreads();

	// ID of the crashed process.
	const pid_t pid_;

	// Builds a proc path for a certain pid for a node (/proc/<pid>/<node>).
	// |path| is a character array of at least NAME_MAX bytes to return the
	// result.|node| is the final node without any slashes. Returns true on
	// success.
	bool BuildProcPath(char* path, pid_t pid, const char* node) const;

	// Info from /proc/<pid>/maps.
	std::vector<MappingInfo*> mappings_;

	// Info from /proc/<pid>/auxv
	std::vector<elf_aux_val_t> auxv_;

private:

	// Path of the directory containing the proc files of the given process,
	// which is usually a copy of /proc/<pid>.
	const char* procfs_path_;

	const MappingInfo* cacheMapping_;

	bool HandleDeletedFileInMapping(char *path) const;

	bool HandleTrimPackagePrefixInMapping(char *path) const;
};

#endif /* CRASHHANDLER_H_ */
