#ifdef __linux__
#include <unistd.h>
#endif

#include "TcpServerSocket.h"

using namespace Linx;

TcpServerSocket::TcpServerSocket()
{
	Recreate();
}

TcpSocket TcpServerSocket::Accept() const noexcept
{
#ifdef _WIN32
	int len = sizeof(TargetAddr);
#else
	socklen_t len = sizeof(TargetAddr);
#endif

	socket_type AcceptSocket;
	if((AcceptSocket = accept(Sock,(sockaddr*)&TargetAddr, &len)) == (socket_type)-1)
	{
		return TcpSocket();
	}
	return TcpSocket(AcceptSocket);
}

bool TcpServerSocket::Recreate() noexcept
{
	Super::Recreate();

	int optval = 1;  
	if (setsockopt(Sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) < 0)
	{  
		return false;
	}
	return true;
}