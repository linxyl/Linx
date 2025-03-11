#include <iostream>
#ifdef _WIN32
#include <WinSock2.h>
#endif
#include "UdpSocket.h"

using namespace Linx;

UdpSocket::UdpSocket()
{
	Recreate();
}

bool UdpSocket::Recreate() noexcept
{
	Super::Recreate();

	Sock = socket(AF_INET, SOCK_DGRAM, 0);
	return (int)Sock > 0;
}

int UdpSocket::Recv(char* buf, size_t bufsize) const noexcept
{
	int ret;
	if (bRecvAddr)
	{
#ifdef _WIN32
		int addr_len = sizeof(TargetAddr);
#else
		socklen_t addr_len = sizeof(TargetAddr);
#endif
		ret = recvfrom(Sock, buf, bufsize, bRecvAll ? MSG_WAITALL : 0, (sockaddr*)&TargetAddr, &addr_len);
	}
	else
	{
		ret = recv(Sock, buf, bufsize, bRecvAll ? MSG_WAITALL : 0);
	}

	return ret;
}

int UdpSocket::Send(const char* buf, size_t bufsize) const noexcept
{
	int ret = sendto(Sock, buf, bufsize, 0, (sockaddr*)&TargetAddr, sizeof TargetAddr);

	return ret;
}
