#include <iostream>
#include "TcpSocket.h"

using namespace Linx;

void TcpSocket::Init()
{
	Super::Init();

	Sock = socket(AF_INET, SOCK_STREAM, 0);
	if(Sock < 0)
	{
		throw std::logic_error("Create socket failed\n");
	}
}

int TcpSocket::Recv(char* buf, size_t bufsize) noexcept
{
	int ret = recv(*pClientSock, buf, bufsize, 0);

	return ret;
}

int TcpSocket::Send(const char* buf, size_t bufsize) noexcept
{
	int ret = send(*pClientSock, buf, bufsize, 0);

	return ret;
}
