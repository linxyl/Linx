#include <iostream>
#ifdef _WIN32
#include <WinSock2.h>
#include <mswsock.h>
#include <ws2ipdef.h>
#pragma comment(lib, "Mswsock.lib")
#else
#include <sys/sendfile.h>
#include <netinet/tcp.h>
#endif
#include "TcpSocket.h"
#include "Linx/IO/File.h"

using namespace Linx;

void TcpSocket::Init()
{
	Super::Init();

	Sock = socket(AF_INET, SOCK_STREAM, 0);
	if ((int)Sock < 0)
	{
		throw std::logic_error("Create socket failed\n");
	}
}

int TcpSocket::Recv(char* buf, size_t bufsize) noexcept
{
	int ret = recv(*pClientSock, buf, bufsize, bRecvAll ? MSG_WAITALL : 0);

	return ret;
}

int TcpSocket::Send(const char* buf, size_t bufsize) noexcept
{
	int ret = send(*pClientSock, buf, bufsize, 0);

	return ret;
}

long TcpSocket::SendFile(HANDLE InHandle, size_t Size)
{
#ifdef _WIN32
	auto Ret = TransmitFile(*pClientSock,
		InHandle,
		Size,
		0,
		nullptr,
		nullptr,
		TF_USE_DEFAULT_WORKER);
	return Ret ? Size : -1;
#else
	off_t offset = 0;
	return sendfile(*pClientSock, InHandle, &offset, Size);
#endif
}

long TcpSocket::SendFile(const class File& InFile, size_t Size)
{
	return SendFile(InFile.GetHandle(), Size);
}

bool TcpSocket::KeepAlive(bool Val, int Idle, int Interval, int Count)
{
	int optval = (int)Val;
	int optlen = sizeof(optval);
	if (setsockopt(Sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&optval, optlen) < 0)
	{
		return false;
	}
	if (setsockopt(Sock, IPPROTO_TCP, TCP_KEEPIDLE, (char*)&Idle, sizeof(Idle)) < 0)
	{
		return false;
    }
    if (setsockopt(Sock, IPPROTO_TCP, TCP_KEEPINTVL, (char*)&Interval, sizeof(Interval)) < 0)
	{
		return false;
    }
    if (setsockopt(Sock, IPPROTO_TCP, TCP_KEEPCNT, (char*)&Count, sizeof(Count)) < 0)
	{
		return false;
    }
	return true;
}
