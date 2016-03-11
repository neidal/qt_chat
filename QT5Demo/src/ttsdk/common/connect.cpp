#include "connect.h"
//#include "cselect.h"
#include "fcsystem.h"
#include "fcstring.h"

namespace OFC
{
	CNetBuffer::CNetBuffer()
	{
		block_size_ = DEF_NETBUFFER_BLOCKSIZE;
		offset_ = 0;
		size_ = 0;
		buffer_ = NULL;
	}

	CNetBuffer::~CNetBuffer()
	{
		offset_ = 0;
		size_ = 0;
		if (buffer_)
		{
			delete[] buffer_;
			buffer_ = NULL;
		}
	}

	void CNetBuffer::SetBlockSize(unsigned int block_size, unsigned int min_alignsize)
	{
		if (!min_alignsize)
			min_alignsize = MIN_NETBUFFER_ALIGNSIZE;
		block_size_ = (block_size / min_alignsize) * min_alignsize + ((block_size % min_alignsize)  ? min_alignsize : 0);
	}

	bool CNetBuffer::Expand(unsigned int len)
	{
		bool result = false;
		if (!size_ || !buffer_)
		{
			size_ = (len / block_size_ + ((len % block_size_) ? 1 : 0)) * block_size_;
			offset_ = 0;
			buffer_ = new char[size_];
			if (buffer_)
				result = true;
		}
		else
		{
			if (len > (size_ - offset_))
			{
				size_ += (len / block_size_ + ((len % block_size_) ? 1 : 0)) * block_size_;
				char* old_buffer = buffer_;
				buffer_ = new char[size_];
				if (buffer_)
				{
					if (old_buffer)
					{
						if (offset_)
							memcpy(buffer_, old_buffer, offset_);
						delete[] old_buffer;
					}
					else
					{
						offset_ = 0;
					}
					result = true;
				}
				else
				{
					buffer_ = old_buffer;
				}
			}
		}

		return result;
	}

	char* CNetBuffer::Remove(unsigned int len)
	{
		if (len >= offset_)
		{
			offset_ = 0;
			size_ = 0;
			if (buffer_)
			{
				delete[] buffer_;
				buffer_ = NULL;
			}
			return NULL;
		}

		offset_ -= len;
		memmove(buffer_, &buffer_[len], offset_);
		return &buffer_[offset_];
	}

//////////////////////////////////////////////////////////////
	CConnect::CConnect()
		:connect_type_(0)
	{
		Construct();
	}

	CConnect::CConnect(unsigned int connect_type)
		:connect_type_(connect_type)
	{
		Construct();
	}

	CConnect::~CConnect()
	{
		net_select_ = NULL;
	}

	void CConnect::Construct()
	{
		last_recv_time_ = 0;
		last_send_time_ = 0;
		last_connect_time_ = 0;
		last_keeplive_time_ = 0;
		begin_connect_time_ = 0;
		connect_timeout_ = 8;
		keeplive_timeout_ = 90;
		keeplive_interval_ = 60;
		connect_state_ = CS_INIT;
		error_code_ = NET_ERROR_SUCCEED;
		recv_socket_size_ = 0;
		send_socket_size_ = 0;
		max_recv_size_ = MAX_NETBUFFER_RECVSIZE;
		net_select_ = NULL;
	}

	bool CConnect::Connect(unsigned int ip, unsigned short port)
	{
		bool bret = false;
		if (!CheckValid() && SOCKET_SUCCEED == OpenStream())
		{
			SetNonBlock();
			SetTcpNoDelay();

			//reason:尝试解决有的手机的连接问题
			//用close来关闭时，即刻终止并丢弃未发送的缓冲数据
			struct linger linger;
			linger.l_onoff = 1;
			linger.l_linger = 0;
			::setsockopt(GetSocket(), SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(struct linger));

			if (send_socket_size_)
				SetSendBufferSize(send_socket_size_);
			if (recv_socket_size_)
				SetRecvBufferSize(recv_socket_size_);

			struct sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.s_addr = ip;

			OnBeforeConnect();

			int ret = ::connect(GetSocket(), (struct sockaddr *)&addr, sizeof(addr));
#ifdef WIN32
			if (ret == -1 && WSAEWOULDBLOCK == GetLastError())
#else
			if (ret < 0 && (GETSOCKETERROR() == EINPROGRESS))
#endif
			{
				bret = true;
			}
			else if (!ret)
			{
				OnConnect(true);
			}
			else
			{
				OnConnect(false);
			}
		}

		return bret;
	}

	bool CConnect::Connect(const char* ip_addr, unsigned short port)
	{
		if (!CheckValid())
		{
			OnBeforeParseDomain();

			unsigned int ip = 0;
			hostent* hptr = ::gethostbyname(ip_addr);
			if (hptr != NULL)
			{
				if (GETSOCKETERROR() != ETIMEDOUT)
				{
					ip = ((struct in_addr *)*hptr->h_addr_list)->s_addr;
				}
			}

			if (!ip)
				ip = inet_addr(ip_addr);

			if (ip)
				return Connect(ip, port);

			OnConnect(false);
		}
		return false;
	}

	void CConnect::OnBeforeParseDomain()
	{
		last_connect_time_ = CSystem::Now();
		begin_connect_time_ = last_connect_time_;
		connect_state_ = CS_PARSEDOMAIN;
	}

	void CConnect::OnBeforeConnect()
	{
		last_connect_time_ = CSystem::Now();
		begin_connect_time_ = last_connect_time_;
		connect_state_ = CS_CONNECTING;
		if (net_select_)
			net_select_->Set(GetSocket(), (ULONG_PTR)this, FCET_WRITE);
	}

	void CConnect::OnConnect(bool connect_result)
	{
		if (connect_result)
		{
			last_connect_time_ = CSystem::Now();
			begin_connect_time_ = last_connect_time_;
			last_recv_time_ = last_connect_time_;
			last_send_time_ = last_connect_time_;
			last_keeplive_time_ = last_connect_time_;
			connect_state_ = CS_CONNECTED;
			if (net_select_)
				net_select_->Set(GetSocket(), (ULONG_PTR)this, FCET_READ);
		}
		else
		{
			connect_state_ |= CS_RESET;
			OnDisconnect();
		}
	}

	 //断开连接: grace 是否优雅关闭
	bool CConnect::Disconnect(bool grace)
	{
		if (!CheckValid() || CS_CLOSED == connect_state_)
			return false;

		if (CS_CLOSING == connect_state_)
			return true;

		if (CS_RESET != (connect_state_ & (~CS_MASK)))
			connect_state_ = (connect_state_ & (~CS_MASK)) | CS_CLOSING;

		if (CS_CONNECTED == connect_state_ && grace)
		{
			ShutDown();
		}
		else
		{
			OnDisconnect();
		}
		return true;
	}

	//收到连接断开
	void CConnect::OnDisconnect()
	{
		last_connect_time_ = CSystem::Now();
		last_send_time_ = last_connect_time_;
		last_recv_time_ = last_connect_time_;
		last_keeplive_time_ = last_connect_time_;
		if (net_select_)
			net_select_->Del(GetSocket());
		Close();

		if (CS_RESET != (connect_state_ & (~CS_MASK)))
			connect_state_ = (connect_state_ & (~CS_MASK)) | CS_CLOSED;
		else
			connect_state_ = (connect_state_ & (~CS_MASK)) | CS_INIT;
	}

	//收到数据
	int CConnect::OnRecv(const char* buf, unsigned int len)
	{
		last_recv_time_ = CSystem::Now();
		return 0;
	}

	void CConnect::OnIdle()
	{
	}

	//发送数据
	int CConnect::Send(const char* buf, int len)
	{
		if (CS_CONNECTED == connect_state_)
		{
			last_send_time_ = CSystem::Now();
			return CSocket::Send(buf, len);
		}
		return -1;
	}

	//接收数据
	void CConnect::RecvData()
	{
		char* free_buffer = NULL, * read_buffer = NULL;
		int freesize = 0, read_size = 0;
		int totalrecv = -1, realrecv = -1, useread = -1;
		while (true)
		{
			totalrecv = GetToReadBytes();
			/**
			*可读数据<0
			*关闭连接
			*/
			if (totalrecv < 0)
			{
				//LOGE("handle_read_event toread %d:%p %d:%p", iSocket, pConn, toread, pBuffPool);
				//pConn->DisConnect();
				break;
			}

			if (totalrecv >= max_recv_size_)
			{
				Disconnect();
				break;
			}

			/**
			*尝试读取1字节数据
			*/
			if (!totalrecv)
			{
				totalrecv = 1;
			}

			recv_buffer_.Expand(totalrecv);
			freesize = recv_buffer_.GetFreeSize();
			if (!freesize)
			{
				Disconnect();
				break;
			}

			if (freesize < totalrecv)
				totalrecv = freesize;

			free_buffer = recv_buffer_.GetFreeBuffer();
			if (!free_buffer)
			{
				Disconnect();
				break;
			}

			realrecv = Recv(free_buffer, totalrecv);
			if (realrecv > 0)
			{
				recv_buffer_.AddOffset(realrecv);
				read_buffer = recv_buffer_.GetBuffer();
				read_size = recv_buffer_.GetSize();
				while (read_buffer && read_size > 0)
				{
					useread = OnRecv(read_buffer, read_size);
					if (useread > 0)
					{
						recv_buffer_.Remove(useread);
					}
					else if (useread < 0)
					{
#ifndef _WIN32
						Disconnect();
#endif
						break;
					}
					else// if (0 == useread)
					{
						break;
					}

					read_buffer = recv_buffer_.GetBuffer();
					read_size = recv_buffer_.GetSize();
				}
			}
			else if (0 == realrecv)
			{
				int errorcode = GETSOCKETERROR();
				OnDisconnect();
				break;
			}
			else// if (realrecv < 0)
			{
				/*********************************************
				修改：过滤掉 errno == EPERM这个错误码
				原因：华为c8112e会出现这个错误，导致登录失败；该错误的解释：Operation not permitted errno
				标记：zhangtao 2013/01/30
				***************************************************/
				int errorcode = GETSOCKETERROR();
				if (errorcode != EAGAIN && errorcode != EINTR && errorcode != ENOENT && errorcode != EPERM)
				{
#ifndef _WIN32
					Disconnect();
#endif
					//break;
				}
				break;
			}
		}
	}

	//发送心跳包
	void CConnect::SendKeepLive()
	{
		last_keeplive_time_ = CSystem::Now();
	}
}
