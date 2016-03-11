#ifndef __ISTREAMCONNECT_H_
#define __ISTREAMCONNECT_H_

struct IStreamConnectCallback
{
	virtual void OnRelease() = 0;
	virtual void OnLogin(int result) = 0;
};

struct IStreamConnect
{
	virtual long AddRef() = 0;
	virtual long Release() = 0;
	//���ûص��ӿ�
	virtual void SetCallback(IStreamConnectCallback* conn_callback) = 0;
	//���õ�ǰ���ӵ����ӳ�ʱʱ�� ��λ����
	virtual void SetConnectTimeout(unsigned int timeout) = 0;
	//���õ�ǰ���ӵ�������ʱʱ�� ��λ����
	virtual void SetReconnectTimeout(unsigned int timeout) = 0;
	//���õ�ǰ���ӵ��������ʱʱ�� ��λ����
	virtual void SetKeepliveTimeout(unsigned int timeout) = 0;
	//���õ�ǰ���ӵķ��ͽ��ջ����С
	virtual void SetBufferSize(unsigned int send_socket_size, unsigned int recv_socket_size) = 0;
	//ֱ�ӷ�������
	virtual int SendData(const char* buf, int len) = 0;
	//��¼
	virtual bool Login(unsigned long long userid, unsigned long long token, unsigned long long dev1, unsigned long long dev2) = 0;
};

#endif //__ISTREAMCONNECT_H_
