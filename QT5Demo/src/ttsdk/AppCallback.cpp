#include "StdAfx.h"
#include "AppCallback.h"

CAppCallback::CAppCallback():m_pConnect(NULL), m_pStreamConnect(NULL)
{
}

void CAppCallback::OnRelease()
{
	m_pConnect = NULL;
}

void CAppCallback::OnConnect(int result, IConnect* context)
{
	if (0 == result)
		m_pConnect = context;
}

void CAppCallback::OnDisconnect(int result, IConnect* context)
{
	m_pConnect = NULL;
}

void CAppCallback::OnStreamConnect(int result, IStreamConnect* context)
{
	if (0 == result)
		m_pStreamConnect = context;
}

void CAppCallback::OnStreamDisconnect(int result, IStreamConnect* context)
{
	m_pStreamConnect = NULL;
}

void CAppCallback::OnNotifyPacketEvent(int events)
{
	int a = 9;
}
