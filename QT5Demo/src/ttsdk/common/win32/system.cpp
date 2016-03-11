#include <stdlib.h>
//#include <pthread.h>
//#include <dlfcn.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include "system.h"
//#include <sys/time.h>
#include <imagehlp.h>
#pragma comment(lib, "imagehlp.lib")

OFC::CCriticalSection CSystem::cs_;

void CSystem::InitRandom()
{
	int value = CSystem::GetTime();
	::srand(value);
	int randvalue = (::rand() + value) % 16 + 8;
	for (int i = 0; i < randvalue; i++)
		rand();
}

void* CSystem::LoadLib(const char*__lib)
{
	return LoadLibraryA(__lib);
}

void* CSystem::GetFunc(void* __handle, const char* __funcname)
{
	if (NULL == __handle || NULL == __funcname || '\0' == __funcname[0])
		return NULL;

	return (void*)GetProcAddress((HMODULE)__handle, __funcname);
}

bool CSystem::FreeLib(void* __handle)
{
	if (!__handle)
		return false;

	return (bool)(FreeLibrary((HMODULE)__handle) == 0);
}

const char* CSystem::GetLibError()
{
	return "win32 不支持";
}

bool CSystem::CreateProcess( const char* __cmdline, PID* __pid)
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;

	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = TRUE;
	
	BOOL bRet = ::CreateProcessA(
		NULL,
		LPSTR(__cmdline),
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi);
	if (bRet)
	{
		*__pid = pi.dwProcessId;
	}
	return bRet>0;
}

int CSystem::GetAppPath(char* __path, int __len)
{
	DWORD path_len = 0;
	int len = 0;

	if (!__path || __len <= 0)
	{
		char fullpath[MAX_PATH] = { 0 };
		path_len = GetModuleFileNameA(NULL, fullpath, MAX_PATH);

		if (path_len != -1)
		{
			fullpath[path_len] = 0;
			char* findchar = strrchr(fullpath, '\\');
			if (findchar)
			{
				len = findchar + 1 - fullpath;
			}
		}
	}
	else
	{
		path_len = GetModuleFileNameA(NULL, __path, MAX_PATH);
		if (path_len != -1)
		{
			__path[path_len] = 0;
			char* findchar = strrchr(__path, '\\');
			if (findchar)
			{
				*(++findchar) = '\0';
				len = findchar - __path;
			}
		}
	}

	return len;
}

int CSystem::GetAppExeName(char* __exename, int __len)
{
	char fullpath[MAX_PATH] = { 0 };
	int len = 0;
	len = 0;
	DWORD path_len = GetModuleFileNameA(NULL, fullpath, MAX_PATH);
	if (path_len != -1)
	{
		fullpath[path_len] = 0;
		char* findchar = strrchr(fullpath, '/');
		if (findchar)
		{
			++findchar;
			len = fullpath + path_len - findchar;
			if (__exename && __len > 0 && (len + 1) <= __len)
			{
				memcpy(__exename, findchar, len);
				__exename[len] = 0;
			}
		}
	}
	return len;
}

int CSystem::GetFullPath(char* __dest, int __len, const char* __filename)
{
	if (!__filename)
		return 0;

	int len = strlen(__filename);
	if (!__dest || __len <= 0)
	{
		len += GetAppPath(NULL, 0);
		return len;
	}

	int path_len = GetAppPath(__dest, __len);
	if ((len + path_len + 1) > __len)
		return 0;

	memcpy(__dest + path_len, __filename, len);
	len += path_len;
	__dest[len] = 0;
	return len;
}

bool CSystem::MakeDirectory(const char* __dir)
{
	return MakeSureDirectoryPathExists(__dir);
	//return CreateDirectoryA(__dir, NULL);
}

char CSystem::FillDirSeparator(const char* __dest, int __len)
{
	if (!__dest || !__len)
		return 0;

	char dirsep = 0;
	int len = strlen(__dest);
	if (len < __len)
		__len = len;

#ifdef _WIN32
	if (__dest[__len - 1] != '/' && __dest[__len - 1] != '\\')
	{
		dirsep = '\\';
	}
#else
	if (__dest[__len - 1] != '/')
	{
		dirsep = '/';
	}
#endif
	return dirsep;
}

DWORD WINAPI _ThreadProxyFunc(LPVOID __param)
{
	assert(__param != NULL);
	THREAD_PARAMINFO threadinfo = *((THREAD_PARAMINFO*)__param);
	assert(threadinfo.func != NULL);
	delete (THREAD_PARAMINFO*)__param;
	__param = NULL;
	threadinfo.func(threadinfo.param);
	return NULL;
}

bool CSystem::BeginThread(PTHREAD_FUNCTION __func, void* __param)
{
	//复制函数指针和参数
	THREAD_PARAMINFO* threadinfo = new THREAD_PARAMINFO;
	if(NULL == threadinfo)
		return FALSE;

	threadinfo->param = __param;
	threadinfo->func = __func;

	if (NULL == CreateThread(NULL, 0, _ThreadProxyFunc, threadinfo, 0, NULL))
	{
		delete threadinfo;
		return false;
	}
    return true;
}

void CSystem::Sleep(unsigned int __milliseconds)
{
	::Sleep(__milliseconds);
}
//得到毫秒级系统时间
uint64 CSystem::GetSystemTime()
{
	struct timeb tmb = { 0 };
	//memset(&tmb, 0, sizeof(timeb));
	ftime(&tmb);
	return ((uint64)tmb.time * 1000) + tmb.millitm;
}

uint64 CSystem::GetTickCount()
{
	return ::GetTickCount();
}

time_t CSystem::Now()
{
	time_t now;
	time(&now);
	return now;
}
