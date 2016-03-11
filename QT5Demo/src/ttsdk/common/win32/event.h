#ifndef __EVENT_H_
#define __EVENT_H_

#include "../fcdefs.h"
//#include <pthread.h>

namespace OFC
{
//�¼��ȴ����
#define WAIT_OBJECT			(0)
//�¼��ȴ���ʱ
#ifndef _WIN32
#define WAIT_TIMEOUT		(1)
#endif
//�¼��ȴ�����
#define WAIT_ERROR			(-1)

class CEvent
{
public:
	CEvent();
	~CEvent();

public:
	bool Create(bool __manual_reset, bool __initial_state);
    long WaitForEvent(unsigned long __milliseconds = INFINITE);
    bool SetEvent();
    void ResetEvent();
    void Close();
	bool IsCreated()
	{
		return iscreated;
	}

private:
    bool iscreated;
    //pthread_mutex_t mutex;
    //pthread_cond_t cond;
	HANDLE event_;
    bool manual_reset;
    bool state;
	//long waitCount;

	DISALLOW_COPY_AND_ASSIGN(CEvent);
};

} //OFC
#endif // __EVENT_H_
