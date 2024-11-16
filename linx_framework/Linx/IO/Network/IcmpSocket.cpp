#include <string.h>
#include <stdexcept>
#include <memory>
#include <iostream>
#ifdef _WIN32
#include <WinSock2.h>
#endif
#include "IcmpSocket.h"
#include "Linx/Core.h"

using namespace Linx;
using namespace std;

void IcmpSocket::Init()
{
	Super::Init();

	Sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if((int)Sock < 0)
	{
		throw std::logic_error("Create socket failed, you need add sudo to run\n");
	}

	SetRecvTimeout(5000);
}

int IcmpSocket::Ping(PingReply* Reply, int InSeq, int DataSize) noexcept
{
    int SendSize = DataSize + sizeof(ICMPPackage);

	unique_ptr<ICMPPackage, decltype(&free)> upICMP((ICMPPackage*)malloc(SendSize), free);
    ICMPPackage* pICMPPackage = upICMP.get();
    pICMPPackage->MakeRequest(InSeq, DataSize);

    Send((char*)pICMPPackage, SendSize);
	auto SendTime = GetTotalMilliSeconds();

	int nRecvSize = DataSize + 64;
	unique_ptr<char[]> upRecvbuf(new char[nRecvSize]);
	char* const recvbuf = upRecvbuf.get();
	auto nPacketSize = Recv(recvbuf, nRecvSize);
	if (-1 == nPacketSize)
	{
		return -1;
	}

	auto RecvTime = GetTotalMilliSeconds();
	IPHeader* pIPHeader = (IPHeader*)recvbuf;
	unsigned short usIPHeaderLen = (unsigned short)((pIPHeader->VerHLen & 0x0f) * 4);
	pICMPPackage = (ICMPPackage*)(recvbuf + usIPHeaderLen);

	if (!pICMPPackage->CheckReply(InSeq))
	{
		return -2;
	}

	if (Reply)
	{
		Reply->Seq = InSeq;
		Reply->RoundTripTime = RecvTime - SendTime;
		Reply->Bytes = nPacketSize - usIPHeaderLen - sizeof(ICMPPackage);
		Reply->TTL = pIPHeader->TTL;
		in_addr TempAddr;
		TempAddr.s_addr = pIPHeader->SrcIP;
		strcpy(Reply->SrcIP, inet_ntoa(TempAddr));
	}
	return 0;
}

int IcmpSocket::Recv(char* buf, size_t bufsize) noexcept
{
#ifdef _WIN32
	int addr_len = sizeof(TargetAddr);
#else
	socklen_t addr_len = sizeof(TargetAddr);
#endif
	int ret = recv(Sock, buf, bufsize, bRecvAll ? MSG_WAITALL : 0);

	return ret;
}

int IcmpSocket::Send(const char* buf, size_t bufsize) noexcept
{
	int ret = sendto(Sock, buf, bufsize, 0, (sockaddr*)&TargetAddr, sizeof TargetAddr);

	return ret;
}

bool IcmpSocket::PingTest(const char* Target, int Count /*= 4*/, int Size /*= 32*/, int Interval /*=1000*/) noexcept
{
	IcmpSocket is;
    auto host = is.SetTargetAddr(Target, 0);
	PingReply Reply;
	size_t SuccessNum = 0;
	
	if (!host)
	{
		return false;
	}

	cout << "PING " << host->h_name << " (" << inet_ntoa(*(in_addr*)*host->h_addr_list) << ") " << Size << " bytes of data" << endl;
	for (int i = 1; i <= Count; i++)
	{
		auto Ret = is.Ping(&Reply, i, Size);
		if (0 == Ret)
		{
			++SuccessNum;
			cout << Reply.Bytes <<
				" bytes from " <<
				Reply.SrcIP <<
				": icmp_seq=" <<
				Reply.Seq <<
				" ttl=" <<
				Reply.TTL <<
				" time=" <<
				Reply.RoundTripTime <<
				" ms" << endl;
		}
		else if (-1 == Ret)
		{
			cout << "transmit timeout." << endl;
		}
		else if (-2 == Ret)
		{
			cout << "Destination Host Unreachable." << endl;
		}

		if (i != Count)
		{
			SleepM(Interval);
		}
	}
	cout << "\n--- " << host->h_name << " ping statistics ---\n";
	cout << Count <<
		" packets transmitted, " <<
		SuccessNum <<
		" received, " <<
		100 * (1 - (float)SuccessNum / Count) <<
		" % packet loss" << endl;

	return SuccessNum > 0;
}

