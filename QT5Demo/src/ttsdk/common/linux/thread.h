#ifndef __THREAD_H_
#define __THREAD_H_

#include "event.h"

#define MAX_WAIT_FOR_TIME           6000  //���ʱ

namespace OFC
{
	class CThread
	{
	public:
		CThread();

		/* �����߳� */
		bool Start(bool __suspended = false, bool __waitstart = false, unsigned long __stacksize = 0);
		/* �����߳� */
		void Resume();
		/* �����߳� */
		void Suspend();
		bool Stop(unsigned long __timeout = INFINITE);

		/* �ȴ��߳��˳� */
		long WaitForTerminate(unsigned long __timeout = MAX_WAIT_FOR_TIME);
        void Sleep(unsigned int __milliseconds);

		bool IsStarted()
		{
			return (bool)(threadid != 0);
		}
		bool IsSuspend()
		{
			return suspend;
		}
		bool IsTerminated()
		{
			return terminated;
		}
		pthread_t GetThreadID()
		{
			return threadid;
		}

	public:
		virtual ~CThread();
		/* �߳̿�ʼ */
		virtual void Begin();
		/* �߳̽��� */
		virtual void End();
		/* �߳��������� */
		virtual void Run();
		/* ��ֹ�߳� */
		virtual void Terminate();
	private:
		pthread_t threadid;
		bool suspend;
		/* �߳���ֹ�ı�־ */
		bool terminated;
		CEvent suspend_event, term_event;

		static void* ThreadProc(void* arg);

		DISALLOW_COPY_AND_ASSIGN(CThread);
	};
}

#endif //__THREAD_H_
