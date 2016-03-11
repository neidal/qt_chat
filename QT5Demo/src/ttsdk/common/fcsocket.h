#ifndef __FCSOCKET_H_
#define __FCSOCKET_H_

#include <fcntl.h>
#include "fctypes.h"

typedef unsigned int UIP;
typedef unsigned short UIP16;
typedef unsigned short UPORT;

typedef struct {
	union {
		struct {
			UPORT port;
			UIP16 ip2;
			UIP ip;
		}sip;
		UINT64 session;
	};
}SOCKET_SESSION, *PSOCKET_SESSION;

#include <errno.h>
#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#ifndef socklen_t
#define socklen_t int
#ifndef ETIMEDOUT
#define ETIMEDOUT       138
#endif
#endif
//shutdown how macro defined
#define SHUT_RD   0
#define SHUT_WR   1
#define SHUT_RDWR 2
#define GETSOCKETERROR() WSAGetLastError()
#define SETSOCKETERROR(errcode)	WSASetLastError(errcode)
#define CLOSESOCKET(s) closesocket(s);
#else
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#ifdef __APPLE__
#include <netinet/tcp.h>
#else
#include <linux/tcp.h>
#endif
#ifdef __linux_epoll_
#include <sys/epoll.h>
#endif

#define SOCKET int

#define SD_SEND     (SHUT_WR)
#define SD_RECEIVE  (SHUT_RD)
#define SD_BOTH     (SHUT_RDWR)
#define GETSOCKETERROR() errno
#define SETSOCKETERROR(errcode)	errno=(errcode)
#define CLOSESOCKET(s) close(s);
#define ioctlsocket ioctl 

#ifndef SOCKADDR
#define SOCKADDR sockaddr
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET  ((SOCKET)(~0))
#endif //INVALID_SOCKET

#define WSAEWOULDBLOCK EWOULDBLOCK
#define WSAEINPROGRESS EINPROGRESS
#define WSAENOBUFS ENOBUFS
#endif //!WIN32

#ifndef SOCKET_ERROR
#define SOCKET_ERROR            (-1)
#endif

#ifndef SOCKET_SUCCEED
#define SOCKET_SUCCEED			(0)
#endif

#pragma pack(push, 1)
typedef struct in_addrinfo {
	union {
		struct { BYTE s_b1,s_b2,s_b3,s_b4; } S_un_b;
		struct { WORD s_w1,s_w2; } S_un_w;
		UINT S_addr;
	} S_un;
#define s_iaddr S_un.S_addr /* can be used for most tcp & ip code */
#define s_host  S_un.S_un_b.s_b2    // host on imp
#define s_net   S_un.S_un_b.s_b1    // network
#define s_imp   S_un.S_un_w.s_w2    // imp
#define s_impno S_un.S_un_b.s_b4    // imp #
#define s_lh    S_un.S_un_b.s_b3    // logical host
} IN_ADDRINFO, *PIN_ADDRINFO;

#pragma pack(pop)
#define INET_ADDRINFO(x) (*((PIN_ADDRINFO)(&(x))))


//#ifdef _WIN32
//#define select_fd_set fd_set
//#define SELECT_FD_CLR FD_CLR
//#define SELECT_FD_ZERO FD_ZERO
//#else
#ifndef FD_SETSIZE
#define FD_SETSIZE      __FD_SETSIZE
#endif

typedef struct {
	SOCKET fd;
	ULONG_PTR param;
}fd_param;

typedef struct {
	unsigned int fd_count;				/* how many are SET? */
	fd_param  fd_array[FD_SETSIZE];		/* an array of SOCKETs */
}select_fd_set;

#define SELECT_FD_CLR(s, set) do { \
	unsigned int __i; \
	for (__i = 0; __i < ((select_fd_set *)(set))->fd_count; __i++) { \
		if (((select_fd_set *)(set))->fd_array[__i].fd == s) { \
					while (__i < ((select_fd_set *)(set))->fd_count - 1) { \
				((select_fd_set *)(set))->fd_array[__i] = \
				((select_fd_set *)(set))->fd_array[__i + 1]; \
				__i++; \
								} \
			((select_fd_set *)(set))->fd_count--; \
			break; \
				} \
		} \
} while (0)

#define SELECT_FD_ZERO(set) (((select_fd_set *)(set))->fd_count=0)
//#endif

#define SELECT_FD_SET(s, p, set) do { \
	unsigned int __i; \
	for (__i = 0; __i < ((select_fd_set *)(set))->fd_count; __i++) { \
			if (((select_fd_set *)(set))->fd_array[__i].fd == (s)) { \
				break; \
						}\
		}\
	if (__i == ((select_fd_set *)(set))->fd_count) { \
		if (((select_fd_set *)(set))->fd_count < FD_SETSIZE) { \
			((select_fd_set *)(set))->fd_array[__i].fd = (s); \
			((select_fd_set *)(set))->fd_array[__i].param = (ULONG_PTR)(p); \
			((select_fd_set *)(set))->fd_count++; \
				}\
		}\
} while (0)

#define SELECT_FD_CLOSE(set) do { \
	unsigned int __i; \
	for (__i = 0; __i < ((select_fd_set *)(set))->fd_count; __i++) { \
		CLOSESOCKET(((select_fd_set *)(set))->fd_array[__i].fd); \
		((select_fd_set *)(set))->fd_array[__i].fd = INVALID_SOCKET; \
		((select_fd_set *)(set))->fd_array[__i].param = 0; \
	} \
	SELECT_FD_ZERO(set); \
} while (0)

#endif // __FCSOCKET_H_
