/*
 * Select ·â×°
 * Created by nizhaorong 2011-04-18
 */
#ifndef __SELECT_H_
#define __SELECT_H_

#include "fcsocket.h"
#include "fcevent.h"

namespace OFC
{
	//EVENTS_TYPE macro define
	#define FCET_NONE		0x00000000
	#define FCET_ACCEPT		0x00000001
	#define FCET_READ		0x00000002
	#define FCET_WRITE		0x00000004
	#define FCET_ERROR		0x00000008

	typedef struct NetEvent
	{
		fd_set select_set_read;
		fd_set select_set_write;
		fd_set select_set_error;
		select_fd_set select_set_fds;

		fd_set select_read;
		fd_set select_write;
		fd_set select_error;
		select_fd_set select_fds;

		void init()
		{
			FD_ZERO(&select_set_read);
			FD_ZERO(&select_set_write);
			FD_ZERO(&select_set_error);
			SELECT_FD_ZERO(&select_set_fds);
			select_max_fd = 0;
		}

		void fini()
		{
			FD_ZERO(&select_set_read);
			FD_ZERO(&select_set_write);
			FD_ZERO(&select_set_error);
			SELECT_FD_ZERO(&select_set_fds);
		}

		int select_max_fd;
	}NET_EVENT, * PNET_EVENT;

	typedef void(* PNET_CALLBACK)(SOCKET s, unsigned int events_type, ULONG_PTR param);

	class CSelect
	{
	public:
		CSelect();
		virtual ~CSelect();

		void SetNetCallback(PNET_CALLBACK callback)
		{
			net_callback_ = callback;
		}

		bool Set(SOCKET s, ULONG_PTR param, unsigned int events_type);
		void Del(SOCKET s);
		int Dispatch(unsigned int millseconds);

		//static CSelect* GetInstance();
		//static void Destroy();
	private:
		NET_EVENT net_event_;
		PNET_CALLBACK net_callback_;
		//static CSelect* _select;

		DISALLOW_COPY_AND_ASSIGN(CSelect);
	};
}

#endif //__SELECT_H_

