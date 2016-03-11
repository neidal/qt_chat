#ifndef __CSOCKET_H_
#define __CSOCKET_H_

#include "fcdefs.h"
#include "fcsocket.h"

namespace OFC
{
	class CSocket
	{
	public:
		CSocket();

		void SetSocket(SOCKET s)
		{
			s_ = s;
		}
		SOCKET GetSocket()
		{
			return s_;
		}
		bool CheckValid()
		{
			return (bool)(s_ != INVALID_SOCKET);
		}

		int OpenStream();
		int ShutDown();
		void Close();

		bool SetNonBlock();
		bool SetTcpNoDelay();
		bool SetSendBufferSize(int buffer_size);
		bool SetRecvBufferSize(int buffer_size);
		int GetSendBufferSize();
		int GetRecvBufferSize();
		int GetToReadBytes();
		int GetError();
	public:
		virtual ~CSocket();
		virtual int Send(const char* buf, int len);
		virtual int Recv(char* buf, int len);
	private:
		SOCKET s_;

		DISALLOW_COPY_AND_ASSIGN(CSocket);
	};
}

#endif //__CSOCKET_H_

