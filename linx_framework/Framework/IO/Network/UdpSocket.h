#pragma once

#include "Base/SocketBase.h"

namespace linx
{
	class UdpSocket : public SocketBase
	{
		using Super = SocketBase;

	public:
		UdpSocket();

	public:
		// Begin SocketBase Interface.
		virtual int Recv(char* buf, size_t bufsize) noexcept override;
		virtual int Send(const char* buf, size_t bufsize) noexcept override;
		// End SocketBase Interface.
	};
}
