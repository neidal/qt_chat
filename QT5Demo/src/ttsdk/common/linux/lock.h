#ifndef __LOCK_H_
#define __LOCK_H_

#include "../fcdefs.h"
#include <pthread.h>
#ifdef FC_SEMAPHORE_SURRPORT
#include <semaphore.h>
#endif
#include <string.h>
#include <fcntl.h>
#ifdef __ANDROID__
#include <linux/stat.h>
#endif

namespace OFC
{
#ifdef FC_SEMAPHORE_SURRPORT
	class CSemaphore
	{
	public:
		explicit CSemaphore(const char * name = NULL)
		{
			writeprotect = NULL;
			semhname = NULL;
			Open(name);
		}

		~CSemaphore()
		{
			Destroy();
		}

		bool Open(const char * name)
		{
			if (writeprotect)
				return true;

			if (name != NULL && name[0] != 0)
			{
				size_t len = strlen(name);
				semhname = (char*)malloc(len + 1);
				if (semhname)
				{
					strncpy(semhname, name, len);
					writeprotect = sem_open(name, O_CREAT, S_IRUSR | S_IWUSR, 1);
				}
			}
#ifndef __APPLE__
			else
			{
				writeprotect = new sem_t;
				if (sem_init(writeprotect, 0, 1) < 0)
                {
                    delete writeprotect;
                    writeprotect = NULL;
                }
			}
#endif
			return writeprotect != NULL;
		}

		void Destroy()
		{
			if (writeprotect)
			{
				if (semhname)
				{
					sem_close(writeprotect);
					sem_unlink(semhname);
					free(semhname);
					semhname = NULL;
				}
				else
				{
					sem_destroy(writeprotect);
					delete writeprotect;
				}
				writeprotect = NULL;
			}
			else if (semhname)
			{
				free(semhname);
				semhname = NULL;
			}
		}

		fcinline void Lock()
		{
			sem_wait(writeprotect);
		}

		fcinline void Unlock()
		{
			sem_post(writeprotect);
		}
	private:
		sem_t* writeprotect;
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
            wait_semh = NULL;
            semh_name = NULL;
			Open(name);
		}

		~CWaitSemaphore()
		{
            Destroy();
		}
        
        bool Open(const char * name)
        {
			if (wait_semh)
				return true;
            
            if (name != NULL && name[0] != 0)
            {
                size_t len = strlen(name);
                semh_name = (char*)malloc(len+1);
                if (semh_name)
                {
                    strncpy(semh_name, name, len+1);
					wait_semh = sem_open(name, O_CREAT, S_IRUSR | S_IWUSR, 0);
                }
            }
#ifndef __APPLE__
            else
            {
				wait_semh = new sem_t;
				if (sem_init(wait_semh, 0, 0) < 0)
                {
                    delete wait_semh;
                    wait_semh = NULL;
                }
            }
#endif
            return wait_semh != NULL;
        }
        
        void Destroy()
        {
            if (wait_semh)
            {
                if (semh_name)
                {
                    sem_close(wait_semh);
                    sem_unlink(semh_name);
                    free(semh_name);
                    semh_name = NULL;
                }
                else
                {
                    sem_destroy(wait_semh);
					delete wait_semh;
                }
                wait_semh = NULL;
            }
            else if (semh_name)
            {
                free(semh_name);
                semh_name = NULL;
            }
        }

		bool IsOpen()
		{
			return (bool)(wait_semh != NULL);
		}

		void Post()
		{
			sem_post(wait_semh);
		}

		void Wait()
		{
			sem_wait(wait_semh);
		}

	private:
		sem_t* wait_semh;
        char* semh_name;

		DISALLOW_COPY_AND_ASSIGN(CWaitSemaphore);
	};
#endif
    class CCriticalSection
    {
    public:
        CCriticalSection()
        {
			//将临界区设置 在同线程下，不互锁
            pthread_mutexattr_t attr;
            pthread_mutexattr_init(&attr);
            pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
            pthread_mutex_init(&mutex, &attr);
            pthread_mutexattr_destroy(&attr);
        }

        ~CCriticalSection()
        {
            pthread_mutex_destroy(&mutex);
        }

        fcinline void Lock()
        {
            pthread_mutex_lock(&mutex);
        }

        fcinline void Unlock()
        {
            pthread_mutex_unlock(&mutex);
        }
    private:
        pthread_mutex_t mutex;

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

    class CAutoMutex
    {
    public:
        explicit CAutoMutex(pthread_mutex_t& __mutex)
            :mutex(__mutex)
        {
            pthread_mutex_lock(&mutex);
        }

        ~CAutoMutex()
        {
            pthread_mutex_lock(&mutex);
        }

    private:
        pthread_mutex_t& mutex;

		DISALLOW_COPY_AND_ASSIGN(CAutoMutex);
    };

    //////////////////////////////////////////////////////////////////////////
    ///< 读写锁，用于读频率远大于写频率的加锁场景
    class CRWLock
    {
    public:
        CRWLock()
        {
            //将临界区设置 在同线程下，不互锁
            pthread_rwlockattr_t attr;
            pthread_rwlockattr_init(&attr);
            //pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
            pthread_rwlockattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
            pthread_rwlock_init(&lock, &attr);
            pthread_rwlockattr_destroy(&attr);
        }

        ~CRWLock()
        {
            pthread_rwlock_destroy(&lock);
        }

        fcinline int Rdlock()
        {
            return pthread_rwlock_rdlock(&lock);
        }

        fcinline int Wrlock()
        {
            return pthread_rwlock_wrlock(&lock);
        }

        fcinline int Unlock()
        {
            return pthread_rwlock_unlock(&lock);
        }
    private:
        pthread_rwlock_t lock;

		DISALLOW_COPY_AND_ASSIGN(CRWLock);
    };

    class CAutoRDLock
    {
    public:
        explicit CAutoRDLock(CRWLock& __lock)
            :lock(__lock)
        {
            lock.Rdlock();

        }

        ~CAutoRDLock()
        {

            lock.Unlock();
        }
    private:
        CRWLock& lock;

		DISALLOW_COPY_AND_ASSIGN(CAutoRDLock);
    };

    class CAutoWRLock
    {
    public:
        explicit CAutoWRLock(CRWLock& __lock)
            :lock(__lock)
        {
            lock.Wrlock();
        }

        ~CAutoWRLock()
        {

            lock.Unlock();
        }

    private:
        CRWLock& lock;

		DISALLOW_COPY_AND_ASSIGN(CAutoWRLock);
    };

#define AUTO_RD_LOCK(T)  OFC::CAutoRDLock _autordlock(T)
#define AUTO_RW_LOCK(T)  OFC::CAutoWRLock _autowrlock(T)
};

#endif // __LOCK_H_
