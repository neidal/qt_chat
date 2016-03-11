#include "event.h"

namespace OFC
{

CEvent::CEvent()
{
    iscreated = false;
    manual_reset = false;
    state = false;
	event_ = NULL;
}

CEvent::~CEvent()
{
    Close();
}

bool CEvent::Create(bool __manual_reset, bool __initial_state)
{
	if (!iscreated)
	{
		event_ = CreateEvent(NULL, __manual_reset, __initial_state, NULL);
		if (event_ != NULL)
		{
			manual_reset = __manual_reset;
			state = __initial_state;
			iscreated = true;
		}
	}
	return iscreated;
}

void CEvent::Close()
{
	if (iscreated)
	{
		iscreated = false;
		CloseHandle(event_);
		event_ = NULL;
	}
}

long CEvent::WaitForEvent(unsigned long __milliseconds)
{
	if (!iscreated)
		return WAIT_ERROR;
	return WaitForSingleObject(event_, __milliseconds);
}

bool CEvent::SetEvent()
{
    return ::SetEvent(event_);
}

void CEvent::ResetEvent()
{
	::ResetEvent(event_);
}

} //OFC
