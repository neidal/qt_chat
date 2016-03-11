#include <stdlib.h>
#include <pthread.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include "system.h"
#include <sys/time.h>

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
	return (void*)::dlopen(__lib, RTLD_LAZY);
}

void* CSystem::GetFunc(void* __handle, const char* __funcname)
{
	if (NULL == __handle || NULL == __funcname || '\0' == __funcname[0])
		return NULL;

	return (void*)::dlsym(__handle, __funcname);
}

bool CSystem::FreeLib(void* __handle)
{
	if (!__handle)
		return false;

	return (bool)(::dlclose(__handle) == 0);
}

const char* CSystem::GetLibError()
{
	return ::dlerror();
}

bool CSystem::CreateProcess( const char* __cmdline, PID* __pid)
{
	PID pid = 0;
	if((pid = ::fork()) < 0)
	{
		return false;
	}

	if(0 == pid)
	{
		if(system(__cmdline) < 0)
		{
			exit(-1);
		}
	}
	else
	{
		if(__pid)
		{
			*__pid = pid;
		}
	}

	return true;
}

int CSystem::GetAppPath(char* __path, int __len)
{
	ssize_t path_len = 0;
	char apppath[32] = { 0 };
	int len = sprintf(apppath, "/proc/%d/exe", getpid());
	apppath[len] = 0;
	len = 0;

	if (!__path || __len <= 0)
	{
		char fullpath[MAX_PATH] = { 0 };
		path_len = readlink(apppath, fullpath, MAX_PATH);
		if (path_len != -1)
		{
			fullpath[path_len] = 0;
			char* findchar = strrchr(fullpath, '/');
			if (findchar)
			{
				len = findchar + 1 - fullpath;
			}
		}
	}
	else
	{
		path_len = readlink(apppath, __path, __len);
		if (path_len != -1)
		{
			__path[path_len] = 0;
			char* findchar = strrchr(__path, '/');
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
	char apppath[32] = { 0 };
	char fullpath[MAX_PATH] = { 0 };
	int len = sprintf(apppath, "/proc/%d/exe", getpid());
	apppath[len] = 0;
	len = 0;
	ssize_t path_len = readlink(apppath, fullpath, MAX_PATH);
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
    struct stat filestate;
    if (-1 != stat(__dir, &filestate) && S_ISDIR(filestate.st_mode))
        return true;

    const char* findchar = strrchr(__dir, '/');
    if (NULL != findchar)
    {
		int len = findchar - __dir;
		char* dir = new char[len+1];
		if (!dir)
			return false;

		memcpy(dir, __dir, len);
		dir[len] = 0;
		bool result = MakeDirectory(dir);
		delete[] dir;
		if (!result)
            return false;
    }
    int ret = mkdir(__dir, S_IRWXU|S_IRWXG|S_IRWXO);
    return (-1 != ret) ? true : false;
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

void * _ThreadProxyFunc(void* __param)
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
	//启动线程
	pthread_t thread;
	pthread_attr_t moattr;
	pthread_attr_init(&moattr);
    // 设置分离线程线程，优点：1、速度很快;2、自动释放资源
    pthread_attr_setdetachstate(&moattr, PTHREAD_CREATE_DETACHED);
    // 注意: 由于分离线程创建速度很快，可能在pthread_create之前就已经起动了,
    //      因此直接返回TRUE时，有可能线程运行已经结束。
	if (pthread_create(&thread,&moattr,_ThreadProxyFunc,threadinfo) != 0)
    {
        delete threadinfo;
        return false;
    }
    return true;
}

void CSystem::Sleep(unsigned int __milliseconds)
{
	__milliseconds *= 1000;
	usleep(__milliseconds);
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
	struct timeval tmv = { 0 };
	gettimeofday(&tmv, 0);
	return tmv.tv_sec * 1000 + tmv.tv_usec / 1000;
}

time_t CSystem::Now()
{
#if defined (__APPLE__)
    struct timeval tv = { 0 };
    
    if (gettimeofday(&tv, NULL) != 0)
    {
		return -1;
	}
	return tv.tv_sec;
#else
	struct timespec ts = { 0 };
	if (::clock_gettime(CLOCK_MONOTONIC, &ts) == -1)
	{
		return -1;
	}
	return ts.tv_sec;
#endif
}
