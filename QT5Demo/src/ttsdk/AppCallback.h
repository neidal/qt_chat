#pragma once
#include <QObject>
#include "iapp.h"

class CAppCallback : public QObject,public IAppCallback
{
	Q_OBJECT

public:
	CAppCallback();
	IConnect* GetConnect() {return m_pConnect;}
	IStreamConnect* GetStreamConnect() {return m_pStreamConnect;}

private:
	void OnRelease();
	void OnConnect(int result, IConnect* context);
	void OnDisconnect(int result, IConnect* context);
	void OnStreamConnect(int result, IStreamConnect* context);
	void OnStreamDisconnect(int result, IStreamConnect* context);
	void OnNotifyPacketEvent(int events);

	IConnect* m_pConnect;
	IStreamConnect* m_pStreamConnect;
};
