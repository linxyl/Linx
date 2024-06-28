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
#elif __linux__
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
#elif __linux__
		close(ClientSocket);
#endif
		ClientSockets.erase(ClientSocket);
	}
}

void TcpServerSocket::Close() noexcept
{
	Super::Close();

	for (auto it : ClientSockets)
	{
		if (TargetSock)
		{
	#ifdef _WIN32
			closesocket(TargetSock);
	#elif __linux__
			close(TargetSock);
	#endif
		}
	}

	ClientSockets = decltype(ClientSockets)();
}
