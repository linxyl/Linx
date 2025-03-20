#include <string.h>
#include <stdexcept>
#ifdef __linux__
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#endif

#include "SocketBase.h"

using namespace Linx;

SocketBase::SocketBase() noexcept :
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

SocketBase::SocketBase(SocketBase&& InSocket) noexcept :
	Sock(InSocket.Sock),
	TargetAddr(InSocket.TargetAddr),
	bRecvAll(false)
{
	InSocket.Sock = 0;
}

SocketBase::~SocketBase() noexcept
{
	Close();
}

hostent* SocketBase::SetTargetAddr(const std::string& Target, int Port) noexcept
{
    auto* host = gethostbyname(Target.c_str());
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

bool SocketBase::Bind(int Port) const noexcept
{
	sockaddr_in Addr{ 0 };
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

bool SocketBase::IsValid() const noexcept
{
	int error = 0;
#ifdef _WIN32
	int len = sizeof(error);
#else
	socklen_t len = sizeof(error);
#endif
	int ret = getsockopt(Sock, SOL_SOCKET, SO_ERROR, (char*)&error, &len);
	return ret == 0 && error == 0;
}

bool Linx::SocketBase::SetRecvTimeout(int mseconds) const noexcept
{
	return 0 == ::setsockopt(Sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&mseconds, sizeof(mseconds));
}

bool Linx::SocketBase::SetSendTimeout(int mseconds) const noexcept
{
	return 0 == ::setsockopt(Sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&mseconds, sizeof(mseconds));
}

bool Linx::SocketBase::SetRecvBufSize(int size) const noexcept
{
	return 0 == ::setsockopt(Sock, SOL_SOCKET, SO_RCVBUF, (char*)&size, sizeof(size));
}

bool Linx::SocketBase::SetSendBufSize(int size) const noexcept
{
	return 0 == ::setsockopt(Sock, SOL_SOCKET, SO_SNDBUF, (char*)&size, sizeof(size));
}

unsigned int SocketBase::GetLastError() const noexcept
{
#ifdef _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}
