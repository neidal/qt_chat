#include "csocket.h"

namespace OFC
{
#ifdef  _WIN32
	class CIniWinSocket
	{
	public:
		CIniWinSocket()
		{
			init_socket = false;
			// initialize Winsock library
			WSADATA wsa_data = { 0 };
			WORD socket_version = MAKEWORD(1, 1);
			int result = WSAStartup(socket_version, &wsa_data);
			if (SOCKET_SUCCEED == result)
			{
				init_socket = true;
			}
			else
			{
				WSACleanup();
				WSASetLastError(WSAVERNOTSUPPORTED);
			}
		}

		~CIniWinSocket()
		{
			WSACleanup();
			init_socket = false;
		}

	private:
		bool init_socket;
	};

	static CIniWinSocket g_winsocket_init;
#endif

	CSocket::CSocket()
		:s_(INVALID_SOCKET)
	{
	}

	CSocket::~CSocket()
	{
		Close();
	}

	int CSocket::OpenStream()
	{
		int errorcode = SOCKET_SUCCEED;
		Close();
		s_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (SOCKET_ERROR == s_)
		{
			errorcode = GETSOCKETERROR();
		}
		return errorcode;
	}

	void CSocket::Close()
	{
		if (INVALID_SOCKET != s_)
		{
			CLOSESOCKET(s_);
			s_ = INVALID_SOCKET;
		}
	}

	bool CSocket::SetNonBlock()
	{	
		if (CheckValid())
		{
#ifdef _WIN32
			ULONG nonblock = 1;
			if (::ioctlsocket(s_, FIONBIO, &nonblock) == SOCKET_SUCCEED)
				return true;
#else
			int flags = ::fcntl(s_, F_GETFL, 0);

			if (flags < 0)
			{
				//LOGE("Socket setNonBlock %p %d:%s", this, errno, strerror(errno));
				return false;
			}
		
			flags = ::fcntl(s_, F_SETFL, flags | O_NONBLOCK);
			if (flags != 0)
				return true;
#endif
		}

		return false;
	}

	bool CSocket::SetSendBufferSize(int buffer_size)
	{
		if (CheckValid())
		{
			return !::setsockopt(s_, SOL_SOCKET, SO_SNDBUF, (const char*)&buffer_size, sizeof(buffer_size));
		}

		return false;
	}

	bool CSocket::SetRecvBufferSize(int buffer_size)
	{
		if (CheckValid())
		{
			return !::setsockopt(s_, SOL_SOCKET, SO_RCVBUF, (const char*)&buffer_size, sizeof(buffer_size));
		}

		return false;
	}

	int CSocket::GetSendBufferSize()
	{
		int buffer_size = 0;	
		if (CheckValid())
		{	
			::getsockopt(s_, SOL_SOCKET, SO_SNDBUF, (char *)&buffer_size, (socklen_t *)&buffer_size);
		}
		return buffer_size;
	}

	int CSocket::GetRecvBufferSize()
	{
		int buffer_size = 0;
		if (CheckValid())
		{
			::getsockopt(s_, SOL_SOCKET, SO_RCVBUF, (char *)&buffer_size, (socklen_t *)&buffer_size);
		}
		return buffer_size;
	}

	bool CSocket::SetTcpNoDelay()
	{
		if (CheckValid())
		{
			unsigned int state = 1;
			return !::setsockopt(s_, IPPROTO_TCP, TCP_NODELAY, (const char*)&state, sizeof(state));
		}
		return false;
	}

	int CSocket::GetToReadBytes()
	{
		ULONG toread = -1;
		ioctlsocket(s_, FIONREAD, &toread);
		return toread;
	}

	int CSocket::Send(const char* buf, int len)
	{
		int ret = -1;
		if (CheckValid())
		{
			ret = ::send(s_, buf, len, 0);		
			if (ret < 0)
			{
				int err = GETSOCKETERROR();
				if (EAGAIN == err || EINTR == err)
				{
					return len;
				}
			}
		}

		return ret;
	}

	int CSocket::Recv(char* buf, int len)
	{
		return ::recv(s_, buf, len, 0);
	}

	int CSocket::ShutDown()
	{
		return ::shutdown(s_, SHUT_WR);
	}

	int CSocket::GetError()
	{
		int err = -1;
		if (CheckValid())
		{
			socklen_t len = sizeof(err);
	 		if (::getsockopt(s_, SOL_SOCKET, SO_ERROR, (char*)&err, &len) == -1)
	 		{
				return SOCKET_ERROR;
	 		}
		}

		return err;
	}

}
