#pragma once

#include "Base/SocketBase.h"

namespace Linx
{
	class UdpSocket : public SocketBase
	{
	public:
		using Super = SocketBase;

		UdpSocket();
		UdpSocket(UdpSocket&& InSocket):
			Super(std::move(InSocket)),
			bRecvAddr(InSocket.bRecvAddr)
		{}

	public:
		bool bRecvAddr = true;

		// Begin SocketBase Interface.
		virtual void Init() override;
		virtual int Recv(char* buf, size_t bufsize) noexcept override;
		virtual int Send(const char* buf, size_t bufsize) noexcept override;
		// End SocketBase Interface.
	};
}
