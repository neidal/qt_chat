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
	//设置回调接口
	virtual void SetCallback(IStreamConnectCallback* conn_callback) = 0;
	//设置当前连接的连接超时时间 单位毫秒
	virtual void SetConnectTimeout(unsigned int timeout) = 0;
	//设置当前连接的重连超时时间 单位毫秒
	virtual void SetReconnectTimeout(unsigned int timeout) = 0;
	//设置当前连接的心跳保活超时时间 单位毫秒
	virtual void SetKeepliveTimeout(unsigned int timeout) = 0;
	//设置当前连接的发送接收缓冲大小
	virtual void SetBufferSize(unsigned int send_socket_size, unsigned int recv_socket_size) = 0;
	//直接发送数据
	virtual int SendData(const char* buf, int len) = 0;
	//登录
	virtual bool Login(unsigned long long userid, unsigned long long token, unsigned long long dev1, unsigned long long dev2) = 0;
};

#endif //__ISTREAMCONNECT_H_
