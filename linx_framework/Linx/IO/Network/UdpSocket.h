#pragma once

#include "Base/SocketBase.h"

namespace Linx
{
	class UdpSocket : public SocketBase
	{
		using Super = SocketBase;

	public:
		UdpSocket();

	public:
		// Begin SocketBase Interface.
		virtual void Init() override;
		virtual int Recv(char* buf, size_t bufsize) noexcept override;
		virtual int Send(const char* buf, size_t bufsize) noexcept override;
		// End SocketBase Interface.
	};
}
