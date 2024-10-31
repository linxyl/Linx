#include <string.h>
#include <stdexcept>
#ifdef __linux__
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#endif

#include "SocketBase.h"

using namespace Linx;

SocketBase::SocketBase() :
	bRecvAll(0)
{
#ifdef _WIN32
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		throw "WSAStartup failed\n";
	}
#else
	signal(SIGPIPE, SIG_IGN);
#endif
}

SocketBase::~SocketBase()
{
	Close();
}

hostent* SocketBase::SetTargetAddr(const char* Target, int Port) noexcept
{
    auto* host = gethostbyname(Target);
    if (host == nullptr)
	{
        return nullptr;
    }
	char cp[16];
	strcpy(cp, inet_ntoa(*(in_addr*)*host->h_addr_list));

	memset(&TargetAddr, 0, sizeof(TargetAddr));

	TargetAddr.sin_family = AF_INET;
	TargetAddr.sin_port = htons(Port);
	TargetAddr.sin_addr.s_addr = inet_addr(cp);

	return host;
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
#else
		close(Sock);
#endif
		Sock = 0;
	}
}

void Linx::SocketBase::SetRecvTimeout(int mseconds) noexcept
{
	::setsockopt(Sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&mseconds, sizeof(mseconds));
}

void Linx::SocketBase::SetSendTimeout(int mseconds) noexcept
{
	::setsockopt(Sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&mseconds, sizeof(mseconds));
}

void Linx::SocketBase::SetRecvBufSize(int size) noexcept
{
	::setsockopt(Sock, SOL_SOCKET, SO_RCVBUF, (char*)&size, sizeof(size));
}

void Linx::SocketBase::SetSendBufSize(int size) noexcept
{
	::setsockopt(Sock, SOL_SOCKET, SO_SNDBUF, (char*)&size, sizeof(size));
}

unsigned int SocketBase::GetLastError() noexcept
{
#ifdef _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}
