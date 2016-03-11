#ifndef __CONNECT_H_
#define __CONNECT_H_

#include "csocket.h"
#include "cselect.h"

namespace OFC
{
	#define DEF_NETBUFFER_BLOCKSIZE			(4*1024)					//默认块大小为4K字节
	#define MIN_NETBUFFER_ALIGNSIZE 		(1024)						//缓存区用1024字节块对齐
	#define MAX_NETBUFFER_RECVSIZE			(64*1024)					//最大接收缓存数据大小

	class CNetBuffer
	{
	public:
		CNetBuffer();
		~CNetBuffer();

		char* GetBuffer()
		{
			return buffer_;
		}
		void AddOffset(unsigned int offset)
		{
			offset_ += offset;
		}
		unsigned int GetSize()
		{
			return offset_;
		}
		char* GetFreeBuffer()
		{
			return buffer_ ? &buffer_[offset_] : NULL;
		}
		unsigned int GetFreeSize()
		{
			return size_ - offset_;
		}

		void SetBlockSize(unsigned int block_size, unsigned int min_alignsize = MIN_NETBUFFER_ALIGNSIZE);
		bool Expand(unsigned int len);
		char* Remove(unsigned int len);
	private:
		unsigned int block_size_;			// 自增长块大小
		unsigned int size_;					// 缓冲区大小
		unsigned int offset_;				// 当前偏移位置
		char* buffer_;						// 缓冲区

		DISALLOW_COPY_AND_ASSIGN(CNetBuffer);
	};


	#ifndef DEF_DNS_TIMEOUT
	#define DEF_DNS_TIMEOUT		(5*1000)	// Dns解析超时时间:默认5秒
	#endif

	#define CS_INIT				(0x00000000)// 初始化状态
	#define CS_CLOSED			(0x00000001)// 已关闭
	#define CS_CLOSING			(0x00000002)// 正在关闭
	#define CS_PARSEDOMAIN		(0x00000003)// 正在解析域名
	#define CS_CONNECTING		(0x00000004)// 正在连接
	#define CS_CONNECTED		(0x00000005)// 连接成功
	#define CS_RESET			(0x00010000)// 断连后重置，即断连后会依据该状态来重置最后状态为CS_INIT，否则设置为CS_CLOSED
	#define CS_MASK				(0x0000FFFF)// 连接状态掩码

	//错误码宏定义
	#define NET_ERROR_SUCCEED		0

	class CConnect : public CSocket
	{
	public:
		CConnect();
		explicit CConnect(unsigned int connect_type);

		void SetConnectType(unsigned int connect_type)
		{
			connect_type_ = connect_type;
		}
		unsigned int GetConnectType()
		{
			return connect_type_;
		}
		unsigned int GetErrorCode()
		{
			return error_code_;
		}
		void SetConnectState(unsigned int state)
		{
			connect_state_ = state;
		}
		unsigned int GetConnectState()
		{
			return connect_state_;
		}
		time_t GetLastRecvTime()
		{
			return last_recv_time_;
		}
		time_t GetLastSendTime()
		{
			return last_send_time_;
		}
		time_t GetLastConnectTime()
		{
			return last_connect_time_;
		}
		time_t GetConnectTimeout()
		{
			return connect_timeout_;
		}
		void SetConnectTimeout(time_t connect_timeout)
		{
			connect_timeout_ = connect_timeout;
		}
		void SetReconnectTimeout(time_t reconnect_timeout)
		{

		}
		time_t GetKeepliveTimeout()
		{
			return keeplive_timeout_;
		}
		void SetKeepliveTimeout(time_t keeplive_timeout)
		{
			keeplive_timeout_ = keeplive_timeout;
		}
		time_t GetKeepliveInterval()
		{
			return keeplive_interval_;
		}
		void SetKeepliveInterval(time_t keeplive_interval)
		{
			keeplive_interval_ = keeplive_interval;
		}
		void SetSocketBufferSize(unsigned int send_socket_size, unsigned int recv_socket_size)
		{
			recv_socket_size_ = recv_socket_size;
			send_socket_size_ = send_socket_size;
			if (send_socket_size_)
				SetSendBufferSize(send_socket_size_);
			if (recv_socket_size_)
				SetRecvBufferSize(recv_socket_size_);
		}
		void SetMaxRecvSize(unsigned int max_recv_size)
		{
			max_recv_size_ = max_recv_size;
		}
		void SetSelect(CSelect* net_select)
		{
			net_select_ = net_select;
		}
		CSelect* GetSelect()
		{
			return net_select_;
		}

		bool Connect(unsigned int ip, unsigned short port);
		bool Connect(const char* ip_addr, unsigned short port);
		bool Disconnect(bool grace = false);
		void RecvData();

	public:
		virtual ~CConnect();
		virtual int Send(const char* buf, int len);
		virtual void SendKeepLive();

		virtual void OnBeforeParseDomain();
		virtual void OnBeforeConnect();
		virtual void OnConnect(bool connect_result);
		virtual void OnDisconnect();
		virtual int OnRecv(const char* buf, unsigned int len);
		virtual void OnIdle();

	private:
		void Construct();
		void SetErrorCode(unsigned int error_code);

	protected:
		time_t last_recv_time_;				// 最后接受时间 单位秒
		time_t last_send_time_;				// 最后发送时间 单位秒
		time_t last_connect_time_;			// 最后连接时间 单位秒
		time_t last_keeplive_time_;			// 最后心跳发送时间 单位秒
		time_t begin_connect_time_;			// 开始连接时间 单位秒
		time_t connect_timeout_;			// 连接超时时间 单位秒
		time_t keeplive_timeout_;			// 心跳保活超时 单位秒
		time_t keeplive_interval_;			// 发送心跳间隔时间 单位秒
		unsigned int connect_state_;		// 状态
		unsigned int error_code_;			// 错误码
		unsigned int connect_type_;			// 连接类型
		unsigned int recv_socket_size_;		// 接收SOCKET缓冲区大小
		unsigned int send_socket_size_;		// 发送SOCKET缓冲区大小
		unsigned int max_recv_size_;		// 最大接收缓冲区大小
		CNetBuffer send_buffer_;			// 发送缓冲
		CNetBuffer recv_buffer_;			// 接收缓冲区
		CSelect* net_select_;				// 网络模式类：select模式
	};
}

#endif //__CONNECT_H_

