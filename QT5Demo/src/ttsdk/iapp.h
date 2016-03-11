#ifndef __IAPP_H_
#define __IAPP_H_

#include "audio/iaudio.h"
#include "connect/iconnect.h"
#include "connect/istreamconnect.h"

#ifndef TTSDK_API
#ifdef TTSDK_EXPORT
#ifndef OFC_EXPORT
# error need using OFC_EXPORT define, because the module is a dll
#endif
#define TTSDK_API OFC_API
#else
#define TTSDK_API
#endif
#endif

//回调返回值宏定义
#define TTAPP_RESULT_ERROR					(-1)							//未知错误
#define TTAPP_RESULT_OK						(0)								//成功
//具体错误返回值务必大于0
#define TTAPP_RESULT_NOTCONNECTDISP			(1)								//不能连接dispatch服务器
#define TTAPP_RESULT_NOTCONNECTCM			(2)								//不能连接CM服务器
#define TTAPP_RESULT_NOTCONNECTSTREAM		(3)								//不能连接流服务器
#define TTAPP_RESULT_TRANSTIMEOUT			(4)								//事务超时
#define TTAPP_RESULT_DISCONNECTCM			(5)								//断连CM服务器
#define TTAPP_RESULT_REJECTDICONNECTCM		(6)								//被拒绝连CM服务器
#define TTAPP_RESULT_DISCONNECTSTREAM		(7)								//断连流服务器

#define TTAPP_RESULT_LOGIN_NEEDSETAPPINFO	(599)							//用户登录成功但是需要设置应用信息
#define TTAPP_RESULT_LOGIN_BASE				(600)							//登录有关起始错误ID
#define TTAPP_RESULT_LOGIN_NEWUSER			(TTAPP_RESULT_LOGIN_BASE)		//新用户登录
#define TTAPP_RESULT_LOGIN_FAILTURE			(TTAPP_RESULT_LOGIN_BASE + 3)	//登录失败
#define TTAPP_RESULT_LOGIN_MACHINE_CHANGE	(TTAPP_RESULT_LOGIN_BASE + 4)	//机器改变
#define TTAPP_RESULT_LOGIN_OLD_VER_ONLINE	(TTAPP_RESULT_LOGIN_BASE + 5)	//老版本用户在另一台设备上登录
#define TTAPP_RESULT_LOGIN_VALID_PASSWD		(TTAPP_RESULT_LOGIN_BASE + 6)	//密码错误
#define TTAPP_RESULT_LOGIN_TIMEOUT			(TTAPP_RESULT_LOGIN_BASE + 7)	//登录超时
#define TTAPP_RESULT_ACCOUNT_BASE			(700)							//账号信息有关起始错误ID
#define TTAPP_RESULT_ACCOUNT_ERROR			(TTAPP_RESULT_ACCOUNT_BASE)		//账号信息应答失败
#define TTAPP_RESULT_MSG_BASE				(800)							//消息有关起始错误ID
#define TTAPP_RESULT_MSG_ERROR				(TTAPP_RESULT_MSG_BASE)			//消息未知错误
#define TTAPP_RESULT_MSG_STREAM_BASE		(850)							//流消息有关起始错误ID
#define TTAPP_RESULT_MSG_CANCELSTREAM		(TTAPP_RESULT_MSG_BASE + 1)		//取消流消息
#define TTAPP_RESULT_MSG_GETMSGIDFAILED		(TTAPP_RESULT_MSG_BASE + 2)		//获取消息ID错误
#define TTAPP_RESULT_MSG_SENDSTREAMFAILED	(TTAPP_RESULT_MSG_BASE + 3)		//发送流消息错误
#define TTAPP_RESULT_GROUP_BASE				(1000)							//群有关起始错误ID
#define TTAPP_RESULT_GROUP_ERROR			(TTAPP_RESULT_GROUP_BASE)		//群未知错误
#define TTAPP_RESULT_GROUP_INVALID_MEMBER	(TTAPP_RESULT_GROUP_BASE + 1)	//非群成员
#define TTAPP_RESULT_GROUP_GROUPFULL		(TTAPP_RESULT_GROUP_BASE + 2)	//群满了
#define TTAPP_RESULT_GROUP_ALLMSG			(TTAPP_RESULT_GROUP_BASE + 3)	//消息结束标记
#define TTAPP_RESULT_GROUP_SLICE			(TTAPP_RESULT_GROUP_BASE + 4)	//结果分片
#define TTAPP_RESULT_GROUP_OVERDUE			(TTAPP_RESULT_GROUP_BASE + 5)	//邀请过期
#define TTAPP_RESULT_GROUP_DISBANDED		(TTAPP_RESULT_GROUP_BASE + 6)	//群已经解散(群成员为0时认为群已经解散)
////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct IAppCallback
{
	virtual void OnRelease() = 0;
	virtual void OnConnect(int result, IConnect* context) = 0;
	virtual void OnDisconnect(int result, IConnect* context) = 0;
	virtual void OnStreamConnect(int result, IStreamConnect* context) = 0;
	virtual void OnStreamDisconnect(int result, IStreamConnect* context) = 0;
	virtual void OnNotifyPacketEvent(int events) = 0;
};

typedef enum eNetAppFlag
{
	NAF_NOMARL = 0x0000,			// 正常应用,获取通通账户信息
	NAF_GAME_SETAPPACCOUNT = 0x0001,// 游戏应用,并应用客户端需设置应用账号信息(数值类型)或获取之
	NAF_GAME = 0x0002,				// 游戏应用,不用获取相应应用信息和通通账号信息（即只有通通账号ID）
	NAF_GAME_GETAPPACCOUNT = 0x0003 // 游戏应用,并获取应用账号信息（字符串类型，而其是通过服务端之间设置的）
}NET_APP_FALG;

struct IApp
{
	virtual ~IApp(){}
	virtual long Release() = 0;
	virtual void SetAppFlag(uint32 app_flag) = 0;
	virtual void SetGuidString(const char* guid_string) = 0;
	virtual void SetSaveDataPath(const char* datapath) = 0;
	//设置回调接口
	virtual void SetCallback(IAppCallback* app_callback) = 0;
	//全局设置线程超时时间 单位毫秒
	virtual void SetThreadWaitTime(unsigned int waittime) = 0;
	//全局设置所有连接的连接超时时间 单位毫秒
	virtual void SetConnectTimeout(unsigned int timeout) = 0;
	//全局设置所有连接的重连超时时间 单位毫秒
	virtual void SetReconnectTimeout(unsigned int timeout) = 0;
	//全局设置所有连接的心跳保活超时时间 单位毫秒
	virtual void SetKeepliveTimeout(unsigned int timeout) = 0;
	//设置设置所有连接的心跳间隔时间 单位毫秒
	virtual void SetKeepliveInterval(unsigned int interval) = 0;
	//全局设置所有连接的发送接收缓冲大小
	virtual void SetBufferSize(unsigned int send_socket_size, unsigned int recv_socket_size) = 0;
	//设置处理接收包回调事件的线程是内部私有线程还是外部线程。
	//当处理接收包回调事件的线程为外部线程时，会回调OnNotifyPacketEvent通知到外部一接收包回调事件已产生并置入内部的事件队列中；
	//外部接收到该通知时，需要转换到ＵＩ线程来调用ProcessPacketEvent函数来处理接收包回调事件队列中的事件；
	//这样处理接收包回调事件队列中的相应回调事件就都处于ＵＩ线程上下文环境中，否则就是就处于内部私有线程上下文环境中。
	virtual void SetProcessPacketThread(bool private_thread) = 0;
	//设置处理接收包回调事件的线程为外部线程时，外部线程需要调用该函数来处理接收包回调事件队列中的事件
	virtual void ProcessPacketEvent() = 0;
	//连接
	virtual bool Connect(const char* remote_address, unsigned short port, const char* device_info) = 0;
	//断开连接
	virtual void Disconnect() = 0;
	//恢复连接
	virtual void RestoreConnect() = 0;
	//获取主连接（即消息连接）
	virtual IConnect* GetConnectClass() = 0;
	//获取消息cm的连接
	virtual IStreamConnect* GetStreamConnectClass() = 0;
	virtual IAudio* GetAudioClass(uint32 id_type, uint64 id) = 0;
};

#ifdef __cplusplus
extern "C" {
#endif

	TTSDK_API IApp* FCAPI CreateApp(const char* datapath);
	TTSDK_API void FCAPI DestoryApp(IApp* pApp);
#ifdef __cplusplus
}
#endif
#endif //__IAPP_H_
