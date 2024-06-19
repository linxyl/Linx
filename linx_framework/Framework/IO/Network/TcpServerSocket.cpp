#ifdef __linux__
#include <unistd.h>
#endif

#include "TcpServerSocket.h"

using namespace linx;

TcpServerSocket::TcpServerSocket()
{
	pServerSock = &Sock;
	pClientSock = &TargetSock;

	int optval = 1;  
	if (setsockopt(Sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) < 0)
	{  
		throw "setsockopt failed\n";
	}
}

bool TcpServerSocket::Listen() noexcept
{
    if(listen(Sock, 1) < 0)
    {
		return false;
    }

	return true;
}

bool TcpServerSocket::Accept() noexcept
{
#ifdef _WIN32
	int len = sizeof(TargetAddr);
#elif __linux__
	socklen_t len = sizeof(TargetAddr);
#endif

	if((TargetSock = accept(Sock,(sockaddr*)&TargetAddr, &len)) == -1)
	{
		return false;
	}

	return true;
}

void TcpServerSocket::Close() noexcept
{
	Super::Close();

	if (TargetSock)
	{
#ifdef _WIN32
		closesocket(TargetSock);
#elif __linux__
		close(TargetSock);
#endif
		TargetSock = 0;
	}
}
