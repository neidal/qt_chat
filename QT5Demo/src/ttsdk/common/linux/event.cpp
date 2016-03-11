#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include "event.h"

namespace OFC
{

CEvent::CEvent()
{
    iscreated = false;
    waitCount = 0;
    manual_reset = false;
    state = false;
    memset(&mutex, 0, sizeof(mutex));
    memset(&cond, 0, sizeof(cond));
}

CEvent::~CEvent()
{
    Close();
}

bool CEvent::Create(bool __manual_reset, bool __initial_state)
{
//    pthread_mutexattr_t mutex_attr;
//    pthread_condattr_t con_attr;
//
//    pthread_mutexattr_init(&mutex_attr);
//    pthread_mutex_init(&mutex, &mutex_attr);
//    pthread_mutexattr_destroy(&mutex_attr);
//
//    pthread_condattr_init(&con_attr);
//    pthread_cond_init(&cond, &con_attr);
//    pthread_condattr_destroy(&con_attr);

    if (!iscreated)
    {
        if (pthread_mutex_init(&mutex, NULL) == 0)
        {
            if (pthread_cond_init(&cond, NULL) == 0)
            {
                manual_reset = __manual_reset;
                state = __initial_state;
                iscreated = true;
            }
            else
            {
                pthread_mutex_destroy(&mutex);
            }
        }
    }
    return iscreated;
}

void CEvent::Close()
{
    if (iscreated)
    {
		iscreated = false;
		pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
    }
}

long CEvent::WaitForEvent(unsigned long __milliseconds)
{
    if (!iscreated)
		return WAIT_ERROR;

    if (INFINITE == __milliseconds)
    {
        if (pthread_mutex_lock(&mutex) != 0)
			return WAIT_ERROR;

        while(!state)
        {
            waitCount ++;
            pthread_cond_wait(&cond, &mutex);
            waitCount --;
        }

        if(!manual_reset)
        {
            state = false;
        }

        if (pthread_mutex_unlock(&mutex) != 0)
			return WAIT_ERROR;

		return WAIT_OBJECT;
    }

    // Lock
    if (pthread_mutex_lock(&mutex) != 0)
		return WAIT_ERROR;

	long ret = WAIT_ERROR;
    int rc = 0;
    struct timeval now;      /*time when we started waiting*/
    struct timespec timeout; /*timeout value for the wait function */

    //取当前时间
    gettimeofday(&now, NULL);
    //准备时间间隔值
    timeout.tv_sec = now.tv_sec + __milliseconds / 1000;
    timeout.tv_nsec = now.tv_usec * 1000 + (__milliseconds % 1000) * 1000000;//timeout.tv_nsec = now.tv_usec + dwMilliseconds % 1000;
    if (timeout.tv_nsec >= 1000000000)
    {
        timeout.tv_nsec -= 1000000000;
        timeout.tv_sec ++;
    }

    if (!state)
    {
        rc = pthread_cond_timedwait(&cond, &mutex, &timeout);
    }

    if (rc != 0)
    {
        if (ETIMEDOUT == rc)
        {
			ret = WAIT_TIMEOUT;
        }
    }
    else
    {
        if(!manual_reset)
        {
            state = false;
        }
		ret = WAIT_OBJECT;
    }

    if (pthread_mutex_unlock(&mutex) != 0)
		ret = WAIT_ERROR;

    return ret;
}

bool CEvent::SetEvent()
{
    if (!iscreated)
        return false;

	int rc = 0;
	pthread_mutex_lock(&mutex);
    state = true;
    if (manual_reset)
        rc = pthread_cond_broadcast(&cond);
    else
        rc = pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return (bool)(0 == rc);
}

void CEvent::ResetEvent()
{
	pthread_mutex_lock(&mutex);
    if (iscreated)
        state = false;
	pthread_mutex_unlock(&mutex);
}

} //OFC
