#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#include "../fcdefs.h"
#include <time.h>
#include <sys/timeb.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>

typedef pid_t PID;

#ifdef __NR_gettid
//syscall0(int, gettid)
#define gettid() syscall(__NR_gettid)
#define getpdd() syscall(__NR_getpid)
#else
#define gettid() ((int)-ENOSYS)
#endif //!__NR_gettid

#define STILL_ACTIVE ((DWORD )0x00000103L)  // ��ʾ�ȴ��Ľ�����������,ֻ�е�IsHangΪfalseʱ,ͨ��exitcode���ش�ֵ

enum ENMU_PROCESS_WAIT_RESULT
{
	PROCESS_WAIT_SUCCESS = 0,	// ��ʾ�ȴ��Ľ����Ѿ�����
	PROCESS_WAIT_FAIL,			// �ȴ����ִ���
	PROCESS_GET_EXITCODE_ERROR,	// ��ʾ�ȴ��ɹ�,���ǻ�ý��̵ķ���ֵ���ִ���
};

//�߳�ִ�к���
typedef void (CDECL * PTHREAD_FUNCTION)(void *);
typedef struct _THREAD_PARAMINFO
{
	void* param;
	PTHREAD_FUNCTION func;
}THREAD_PARAMINFO;

// ------------------------------------
class CSystem
{
public:
	static void InitRandom();
    static void* LoadLib(const char*__lib);
    static void* GetFunc(void* __handle, const char* __funcname);
	static bool FreeLib(void* __handle);
	static const char* GetLibError();
	static bool CreateProcess( const char* __cmdline, PID* __pid);
	static int GetAppPath(char* __path, int __len);
	static int GetAppExeName(char* __exename, int __len);
	static int GetFullPath(char* __dest, int __len, const char* __filename);
	static bool MakeDirectory(const char* __dir);
	static char FillDirSeparator(const char* __dest, int __len);
	//�����߳�
	static bool BeginThread(PTHREAD_FUNCTION __func, void* __param);
	static void Sleep(unsigned int __milliseconds);
	//�õ����뼶ϵͳʱ��
	static uint64 GetSystemTime();
	static uint64 GetTickCount();
	static time_t Now();

	static long InterlockedIncrement(long volatile * ref_count)
	{
		//need gcc 4.1.0 later
		return __sync_add_and_fetch(ref_count, 1);
	}
	static long InterlockedDecrement(long volatile * ref_count)
	{
		//need gcc 4.1.0 later
		return __sync_sub_and_fetch(ref_count, 1);
	}
	static int InterlockedIncrement(int volatile * ref_count)
	{
		//need gcc 4.1.0 later
		return __sync_add_and_fetch(ref_count, 1);
	}
	static int InterlockedDecrement(int volatile * ref_count)
	{
		//need gcc 4.1.0 later
		return __sync_sub_and_fetch(ref_count, 1);
	}
	static int64 InterlockedIncrement(int64 volatile * ref_count)
	{
		//need gcc 4.1.0 later
		return __sync_add_and_fetch(ref_count, 1);
	}
	static int64 InterlockedDecrement(int64 volatile * ref_count)
	{
		//need gcc 4.1.0 later
		return __sync_sub_and_fetch(ref_count, 1);
	}
	static bool InterlockesSwap(int64 volatile * ptr, int64 oldval, int64 newval)
	{
		//need gcc 4.1.0 later
		return __sync_bool_compare_and_swap(ptr, oldval, newval);
	}
	//�õ��뼶ϵͳʱ��
	static fcinline time_t GetTime()
	{
		return time(NULL);
	}
	static inline int GetThreadID()
	{
		return gettid();
	}
	static inline int GetProccessID()
	{
		return getpid();
	}
};

#endif // __SYSTEM_H_
