#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif
#include <winsock2.h>  
#else
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>
#endif

#include "TcpClientSocket.h"

using namespace Linx;

TcpClientSocket::TcpClientSocket()
{
	Init();

	pServerSock = &TargetSock;
	pClientSock = &Sock;
}

bool TcpClientSocket::Connect() noexcept
{
#ifdef _WIN32
	unsigned long mode = 1;
	if (SOCKET_ERROR == ioctlsocket(Sock, FIONBIO, (unsigned long*)&mode))
	{
		return false;
	}

	int n = connect(Sock, (struct sockaddr*)&TargetAddr, sizeof(TargetAddr));
	if (n == SOCKET_ERROR) {  
        int error = WSAGetLastError();  
        if (error != WSAEWOULDBLOCK) {  
            return false;  
        }  
    }

	pollfd fds;
	fds.fd = Sock;  
    fds.events = POLLOUT;  
  
    WSAPoll(&fds, 1, ConnectionTimeoutSeconds);
	if (fds.revents != POLLOUT)
	{
		return false;
	}

	mode = 0;
	if (SOCKET_ERROR == ioctlsocket(Sock, FIONBIO, (unsigned long*)&mode))
	{
		return false;
	}

	return true;
#else
	int flags = fcntl(Sock, F_GETFL, 0);
	if(-1 == flags)
	{
		flags = 0;
	}
	fcntl(Sock, F_SETFL, flags | O_NONBLOCK);

	int n = ::connect(Sock, (struct sockaddr*)&TargetAddr, sizeof(TargetAddr));
	if (n < 0)
	{
		if (errno != EINPROGRESS && errno != EWOULDBLOCK)
			return false;

		pollfd fds;
		fds.fd = Sock;
		fds.events = POLLOUT;
		n = poll(&fds, 1, ConnectionTimeoutSeconds);
		if(fds.revents != POLLOUT)
		{
			return false;
		}
		// fd_set wset;
		// FD_ZERO(&wset);
		// FD_SET(Sock, &wset);
		// n = select(Sock + 1, NULL, &wset, NULL, &ConnectTimeout);
		// if (n < 0)
		// {
		// 	return false;
		// }
		// else if (0 == n)
		// {
		// 	// 超时 				
		// 	return false;
		// };

		int so_error;  
		socklen_t len = sizeof(so_error);  
		if (getsockopt(Sock, SOL_SOCKET, SO_ERROR, &so_error, &len) < 0)
		{
			return false;  
		}  
		if (so_error)
		{
			return false;  
		} 
	}
	fcntl(Sock, F_SETFL, flags & ~O_NONBLOCK); 

	return true;
#endif
}

bool TcpClientSocket::Connect(const char * IP, int Port) noexcept
{
	SetTargetAddr(IP, Port);

	return Connect();
}
