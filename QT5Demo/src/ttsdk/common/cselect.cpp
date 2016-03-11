#include "cselect.h"
#include "fcstring.h"

namespace OFC
{
	//CSelect* CSelect::_select = NULL;
	CSelect::CSelect()
	{
		net_event_.init();
	}

	CSelect::~CSelect()
	{
		net_event_.fini();
	}

	//CSelect* CSelect::GetInstance()
	//{
	//	if (!_select)
	//	{
	//		_select = new CSelect();
	//	}
	//	return _select;
	//}

	//void CSelect::Destroy()
	//{
	//	if (!_select)
	//	{
	//		delete _select;
	//		_select = NULL;
	//	}
	//}

	bool CSelect::Set(SOCKET s, ULONG_PTR param, unsigned int events_type)
	{
		if (s > net_event_.select_max_fd)
			net_event_.select_max_fd = s;

		SELECT_FD_SET(s, param, &(net_event_.select_set_fds));
		FD_SET(s, &net_event_.select_set_error);

		if (events_type & FCET_READ)
		{
			FD_SET(s, &net_event_.select_set_read);
		}
		else
		{
			FD_CLR(s, &net_event_.select_set_read);
		}

		if (events_type & FCET_WRITE)
		{
			FD_SET(s, &net_event_.select_set_write);
		}
		else
		{
			FD_CLR(s, &net_event_.select_set_write);
		}
		return true;
	}

	void CSelect::Del(SOCKET s)
	{
		if (INVALID_SOCKET != s)
		{
			FD_CLR(s, &(net_event_.select_set_read));
			FD_CLR(s, &(net_event_.select_set_write));
			FD_CLR(s, &(net_event_.select_set_error));
			SELECT_FD_CLR(s, &(net_event_.select_set_fds));
		}
	}

	int CSelect::Dispatch(unsigned int millseconds)
	{
		int ret = -1;
		struct timeval tv;
		tv.tv_sec = millseconds / 1000;
		tv.tv_usec = (millseconds % 1000) * 1000;

		int nfds = net_event_.select_max_fd + 1;
		net_event_.select_read = net_event_.select_set_read;
		net_event_.select_write = net_event_.select_set_write;
		net_event_.select_error = net_event_.select_set_error;
		net_event_.select_fds = net_event_.select_set_fds;

		ret = select(nfds, &(net_event_.select_read), &(net_event_.select_write), &(net_event_.select_error), &tv);
		if (ret > 0)
		{
			int fd = -1;
			ULONG_PTR param = 0;
			unsigned int events_type = FCET_NONE;
			for (int i = 0; i < net_event_.select_fds.fd_count; i++)
			{
				events_type = FCET_NONE;
				fd = net_event_.select_fds.fd_array[i].fd;
				param = net_event_.select_fds.fd_array[i].param;
				if (FD_ISSET(fd, &net_event_.select_read))
					events_type |= FCET_READ;
				if (FD_ISSET(fd, &net_event_.select_write))
					events_type |= FCET_WRITE;
				if (FD_ISSET(fd, &net_event_.select_error))
					events_type |= FCET_ERROR;

				if (FCET_NONE == events_type)
					continue;

				net_callback_(fd, events_type, param);
			}
		}
		else if (SOCKET_ERROR == ret)
		{
			if (GETSOCKETERROR() != EINTR)
			{
				return SOCKET_ERROR;
			}
		}

		return 0;
	}
}
