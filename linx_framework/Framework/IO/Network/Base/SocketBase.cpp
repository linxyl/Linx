#include <string.h>
#ifdef __linux__
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#endif

#include "SocketBase.h"

using namespace linx;

SocketBase::SocketBase()
{
#ifdef _WIN32
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		throw "WSAStartup failed\n";
	}
#elif __linux__
	signal(SIGPIPE, SIG_IGN);
#endif
}

SocketBase::~SocketBase()
{
	Close();
}

void SocketBase::SetTargetAddr(const char* IP, int Port) noexcept
{
	memset(&TargetAddr, 0, sizeof(TargetAddr));

	TargetAddr.sin_family = AF_INET;
	TargetAddr.sin_port = htons(Port);
	TargetAddr.sin_addr.s_addr = inet_addr(IP);
}

bool SocketBase::Bind(int Port) noexcept
{
	memset(&Addr, 0, sizeof(Addr));
	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(Port);
	Addr.sin_addr.s_addr = htonl(INADDR_ANY);

	return (bind(Sock,(const sockaddr*)&Addr,sizeof(Addr)) >= 0);
}

void SocketBase::Close() noexcept
{
	if (Sock)
	{
#ifdef _WIN32
		closesocket(Sock);
#elif __linux__
		close(Sock);
#endif
		Sock = 0;
	}
}

void linx::SocketBase::SetRecvTimeout(int mseconds) noexcept
{
	::setsockopt(Sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&mseconds, sizeof(mseconds));
}

void linx::SocketBase::SetSendTimeout(int mseconds) noexcept
{
	::setsockopt(Sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&mseconds, sizeof(mseconds));
}

void linx::SocketBase::SetRecvBufSize(int size) noexcept
{
	::setsockopt(Sock, SOL_SOCKET, SO_RCVBUF, (char*)&size, sizeof(size));
}

void linx::SocketBase::SetSendBufSize(int size) noexcept
{
	::setsockopt(Sock, SOL_SOCKET, SO_SNDBUF, (char*)&size, sizeof(size));
}

unsigned int SocketBase::GetLastError() noexcept
{
#ifdef _WIN32
	return WSAGetLastError();
#elif __linux__
	return errno;
#endif
}
