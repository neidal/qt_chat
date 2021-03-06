#ifndef __LOCK_H_
#define __LOCK_H_

#include "../fcdefs.h"
//#include <pthread.h>
//#include <semaphore.h>
//#include <string.h>
//#include <fcntl.h>
#ifdef __ANDROID__
#include <linux/stat.h>
#endif

namespace OFC
{
	class CSemaphore
	{
	public:
		explicit CSemaphore(const char * name = NULL)
		{
			sem = NULL;
			semhname = NULL;
			Open(name);
		}

		~CSemaphore()
		{
			Destroy();
		}

		bool Open(const char * name)
		{
			if (sem)
				return true;

			if (name != NULL && name[0] != 0)
			{
				size_t len = strlen(name);
				semhname = (char*)malloc(len + 1);
				if (semhname)
				{
					strncpy_s(semhname, 100, name, len);
					sem = CreateSemaphoreA(NULL, 1, 1, semhname);
				}
			}
			else
			{
				sem = CreateSemaphoreA(NULL, 1, 1, NULL);
			}
			return sem != NULL;
		}

		void Destroy()
		{
			if (sem)
			{
				CloseHandle(sem);
				sem = NULL;
			}
			if (semhname)
			{
				free(semhname);
				semhname = NULL;
			}
		}

		fcinline void Lock()
		{
			WaitForSingleObject(sem, INFINITE);
		}

		fcinline void Unlock()
		{
			ReleaseSemaphore(sem, 1, NULL);
		}
	private:
		HANDLE sem;
		char* semhname;

		DISALLOW_COPY_AND_ASSIGN(CSemaphore);
	};

	class CAutoSemaphore
	{
	public:
		explicit CAutoSemaphore(CSemaphore& __semh)
			: semh(__semh)
		{
			semh.Lock();
		}

		~CAutoSemaphore()
		{
			semh.Unlock();
		}

	private:
		CSemaphore& semh;

		DISALLOW_COPY_AND_ASSIGN(CAutoSemaphore);
	};

	class CWaitSemaphore
	{
	public:      
		explicit CWaitSemaphore(const char * name = NULL)
        {
            sem = NULL;
            semh_name = NULL;
			Open(name);
		}

		~CWaitSemaphore()
		{
            Destroy();
		}
        
        bool Open(const char * name)
        {
			if (sem)
				return true;

			if (name != NULL && name[0] != 0)
			{
				size_t len = strlen(name);
				semh_name = (char*)malloc(len + 1);
				if (semh_name)
				{
					strncpy_s(semh_name, 100, name, len);
					sem = CreateSemaphoreA(NULL, 1, 1, semh_name);
				}
			}
			else
			{
				sem = CreateSemaphoreA(NULL, 1, 1, NULL);
			}
			return sem != NULL;
        }
        
        void Destroy()
		{
			if (sem)
			{
				CloseHandle(sem);
				sem = NULL;
			}
			if (semh_name)
			{
				free(semh_name);
				semh_name = NULL;
			}
        }

		bool IsOpen()
		{
			return (bool)(sem != NULL);
		}

		void Post()
		{
			ReleaseSemaphore(sem, 1, NULL);
		}

		void Wait()
		{
			WaitForSingleObject(sem, INFINITE);
		}

	private:
		HANDLE sem;
        char* semh_name;

		DISALLOW_COPY_AND_ASSIGN(CWaitSemaphore);
	};

    class CCriticalSection
    {
    public:
        CCriticalSection()
        {
			InitializeCriticalSection(&criticalSection);
        }

        ~CCriticalSection()
        {
			DeleteCriticalSection(&criticalSection);
        }

        fcinline void Lock()
        {
			EnterCriticalSection(&criticalSection);
        }

        fcinline void Unlock()
        {
			LeaveCriticalSection(&criticalSection);
        }
    private:
		CRITICAL_SECTION criticalSection;
		DISALLOW_COPY_AND_ASSIGN(CCriticalSection);
    };

    class CAutoCriticalSection
    {
    public:
        explicit CAutoCriticalSection(CCriticalSection& __cs)
            :cs(__cs)
        {
            cs.Lock();
        }

        ~CAutoCriticalSection()
        {
            cs.Unlock();
        }

    private:
        CCriticalSection& cs;

		DISALLOW_COPY_AND_ASSIGN(CAutoCriticalSection);
    };

    class CFakeCriticalSection
    {
    public:
        fcinline void Lock(){}
        fcinline void Unlock(){}
    };

#define AUTO_CRITICAL_SECTION(T)  OFC::CAutoCriticalSection _autolock(T)
//
//    class CAutoMutex
//    {
//    public:
//        explicit CAutoMutex(pthread_mutex_t& __mutex)
//            :mutex(__mutex)
//        {
//            pthread_mutex_lock(&mutex);
//        }
//
//        ~CAutoMutex()
//        {
//            pthread_mutex_lock(&mutex);
//        }
//
//    private:
//        pthread_mutex_t& mutex;
//
//		DISALLOW_COPY_AND_ASSIGN(CAutoMutex);
//    };
//
//    //////////////////////////////////////////////////////////////////////////
//    ///< 读写锁，用于读频率远大于写频率的加锁场景
//    class CRWLock
//    {
//    public:
//        CRWLock()
//        {
//            //将临界区设置 在同线程下，不互锁
//            pthread_rwlockattr_t attr;
//            pthread_rwlockattr_init(&attr);
//            //pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
//            pthread_rwlockattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
//            pthread_rwlock_init(&lock, &attr);
//            pthread_rwlockattr_destroy(&attr);
//        }
//
//        ~CRWLock()
//        {
//            pthread_rwlock_destroy(&lock);
//        }
//
//        fcinline int Rdlock()
//        {
//            return pthread_rwlock_rdlock(&lock);
//        }
//
//        fcinline int Wrlock()
//        {
//            return pthread_rwlock_wrlock(&lock);
//        }
//
//        fcinline int Unlock()
//        {
//            return pthread_rwlock_unlock(&lock);
//        }
//    private:
//        pthread_rwlock_t lock;
//
//		DISALLOW_COPY_AND_ASSIGN(CRWLock);
//    };
//
//    class CAutoRDLock
//    {
//    public:
//        explicit CAutoRDLock(CRWLock& __lock)
//            :lock(__lock)
//        {
//            lock.Rdlock();
//
//        }
//
//        ~CAutoRDLock()
//        {
//
//            lock.Unlock();
//        }
//    private:
//        CRWLock& lock;
//
//		DISALLOW_COPY_AND_ASSIGN(CAutoRDLock);
//    };
//
//    class CAutoWRLock
//    {
//    public:
//        explicit CAutoWRLock(CRWLock& __lock)
//            :lock(__lock)
//        {
//            lock.Wrlock();
//        }
//
//        ~CAutoWRLock()
//        {
//
//            lock.Unlock();
//        }
//
//    private:
//        CRWLock& lock;
//
//		DISALLOW_COPY_AND_ASSIGN(CAutoWRLock);
//    };

#define AUTO_RD_LOCK(T)  OFC::CAutoRDLock _autordlock(T)
#define AUTO_RW_LOCK(T)  OFC::CAutoWRLock _autowrlock(T)
};

#endif // __LOCK_H_
