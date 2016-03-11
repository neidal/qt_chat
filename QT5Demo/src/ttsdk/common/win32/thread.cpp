#include "thread.h"
//#include <unistd.h>

namespace OFC
{

CThread::CThread()
{
	suspend = false;
	terminated = false;
	threadid = 0;
}

CThread::~CThread()
{
	if (threadid)
	{
		Terminate();
		WaitForTerminate();
		threadid = 0;
	}
}

void CThread::Begin()
{
    terminated = false;
}

void CThread::End()
{
    term_event.SetEvent();
    threadid = 0;
}

void CThread::Run()
{
    //TODO: nothing
}

struct ThreadParam
{
    CThread* _thread;
    CEvent* _event;
};

bool CThread::Start(bool __suspended, bool __waitstart, unsigned long __stackSize)
{
	if (IsStarted())
		return true;

	bool ret = false;

	CEvent* event = NULL;
	ThreadParam* param = new ThreadParam;
	param->_thread = this;
	if (__waitstart)
	{
		CEvent* event_ = new CEvent();
		event_->Create(false, false);
		param->_event = event_;
		//event.Create(false, false);
		//param->_event = &event;
	}
	else
	{
		param->_event = NULL;
	}

	if (__suspended)
		Suspend();

	if (CreateThread(NULL, __stackSize, ThreadProc, param, 0, &threadid))
		ret = true;

	if (event)
	{
		if (ret)
			event->WaitForEvent();
		delete event;
		event = NULL;
	}

	return ret;
}

void CThread::Resume()
{
    suspend = false;
 	suspend_event.SetEvent();
}

void CThread::Suspend()
{
 	suspend = true;
	suspend_event.Create(false, false);
}

void CThread::Terminate()
{
	term_event.Create(false, false);
	terminated = true;
	Resume();
}

bool CThread::Stop(unsigned long __timeout)
{
	if (IsStarted())
	{
		Terminate();
		if (WAIT_OBJECT == WaitForTerminate(__timeout))
		{
			threadid = 0;
			return true;
		}
	}
	return false;
}

long CThread::WaitForTerminate(unsigned long __timeout)
{
	long ret = WAIT_ERROR;
	if (IsStarted() && IsTerminated())
	{
	    ret = term_event.WaitForEvent(__timeout);
	}
	return ret;
}

//线程休眠
void CThread::Sleep(unsigned int __milliseconds)
{
	::Sleep(__milliseconds);
}

DWORD WINAPI CThread::ThreadProc(LPVOID arg)
{
	ThreadParam* param = (ThreadParam*)arg;
	if (!param)
		return NULL;

	CThread* thread = param->_thread;
	CEvent* event_ = param->_event;
	delete param;

	thread->Begin();
	if (event_)
    {
        event_->SetEvent();
		delete event_;
        event_ = NULL;
    }

	/* 线程运行中 */
	while (!thread->IsTerminated())
	{
		/* running ... */
		thread->Run();
	}
    thread->End();
	return NULL;
}

}//namespace OFC
