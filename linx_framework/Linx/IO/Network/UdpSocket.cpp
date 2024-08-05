#include <iostream>
#include "UdpSocket.h"

using namespace Linx;

UdpSocket::UdpSocket()
{
	Init();
}

void UdpSocket::Init()
{
	Super::Init();

	Sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(Sock < 0)
	{
		throw std::logic_error("Create socket failed\n");
	}
}

int UdpSocket::Recv(char* buf, size_t bufsize) noexcept
{
#ifdef _WIN32
	int addr_len = sizeof(TargetAddr);
#else
	socklen_t addr_len = sizeof(TargetAddr);
#endif
	int ret = recvfrom(Sock, buf, bufsize, 0, (sockaddr*)&TargetAddr, &addr_len);

	return ret;
}

int UdpSocket::Send(const char* buf, size_t bufsize) noexcept
{
	int ret = sendto(Sock, buf, bufsize, 0, (sockaddr*)&TargetAddr, sizeof TargetAddr);

	return ret;
}
