#ifndef __THREAD_H_
#define __THREAD_H_

#include "event.h"


#define MAX_WAIT_FOR_TIME           6000  //最大超时

namespace OFC
{
	class CThread
	{
	public:
		CThread();

		/* 启动线程 */
		bool Start(bool __suspended = false, bool __waitstart = false, unsigned long __stacksize = 0);
		/* 继续线程 */
		void Resume();
		/* 挂起线程 */
		void Suspend();
		bool Stop(unsigned long __timeout = INFINITE);

		/* 等待线程退出 */
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
		unsigned long GetThreadID()
		{
			return threadid;
		}

	public:
		virtual ~CThread();
		/* 线程开始 */
		virtual void Begin();
		/* 线程结束 */
		virtual void End();
		/* 线程主运行体 */
		virtual void Run();
		/* 终止线程 */
		virtual void Terminate();
	private:
		unsigned long threadid;
		bool suspend;
		/* 线程终止的标志 */
		bool terminated;
		CEvent suspend_event, term_event;

		static DWORD WINAPI ThreadProc(LPVOID arg);

		DISALLOW_COPY_AND_ASSIGN(CThread);
	};
}

#endif //__THREAD_H_
