#include "TcpSocket.h"

using namespace Linx;

TcpSocket::TcpSocket()
{
	Sock = socket(AF_INET, SOCK_STREAM, 0);
	if(Sock < 0)
	{
		throw "Create socket failed\n";
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
