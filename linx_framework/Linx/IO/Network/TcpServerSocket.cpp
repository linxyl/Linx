#ifdef __linux__
#include <unistd.h>
#endif

#include "TcpServerSocket.h"

using namespace Linx;

TcpServerSocket::TcpServerSocket()
{
	Init();

	pServerSock = &Sock;
	pClientSock = &TargetSock;
}

bool TcpServerSocket::Listen(size_t Num) noexcept
{
    if(listen(Sock, Num) < 0)
    {
		return false;
    }

	return true;
}

bool TcpServerSocket::Accept(bool bSwitchClient) noexcept
{
#ifdef _WIN32
	int len = sizeof(TargetAddr);
#else
	socklen_t len = sizeof(TargetAddr);
#endif

	socket_type AcceptSocket;
	if((AcceptSocket = accept(Sock,(sockaddr*)&TargetAddr, &len)) == -1)
	{
		return false;
	}

	if (bSwitchClient)
	{
		TargetSock = AcceptSocket;
	}

	ClientSockets.insert(TargetSock);

	return true;
}

void TcpServerSocket::SetTargetClient(socket_type ClientSocket) noexcept
{
	TargetSock = ClientSocket;
}

void TcpServerSocket::CloseClient(socket_type ClientSocket) noexcept
{
	if (ClientSocket)
	{
#ifdef _WIN32
		closesocket(ClientSocket);
#else
		close(ClientSocket);
#endif
		ClientSockets.erase(ClientSocket);
	}
}

void TcpServerSocket::Init()
{
	Super::Init();

	int optval = 1;  
	if (setsockopt(Sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) < 0)
	{  
		throw "setsockopt failed\n";
	}
}

void TcpServerSocket::Close() noexcept
{
	Super::Close();

	for (auto it : ClientSockets)
	{
		if (it)
		{
	#ifdef _WIN32
			closesocket(it);
	#else
			close(it);
	#endif
		}
	}

	ClientSockets = decltype(ClientSockets)();
}
