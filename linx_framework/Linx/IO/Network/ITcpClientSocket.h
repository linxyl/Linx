#pragma once

#include "Base/ISocketBase.h"
#include "TcpClientSocket.h"

namespace Linx
{
	/**
	 * Interface of TcpClientSocket.
	 */
	class ITcpClientSocket : public ISocketBase
	{
	public:
		using Super = ISocketBase;

		ITcpClientSocket(TcpClientSocket& InSocket) : Super(InSocket), Socket(InSocket)
		{};

	public:
		/** Connects to the tcp server. */
		inline bool Connect() noexcept { return Socket.Connect(); }
		inline bool Connect(const char* IP, int Port) noexcept { return Socket.Connect(IP, Port); }

	public:
		/** Set the connection timeout period. */
		inline void SetConnectTimeout(float InTimeoutSeconds) noexcept { return Socket.SetConnectTimeout(InTimeoutSeconds); }

	private:
		TcpClientSocket& Socket;
	};
}
