#pragma once

#include "Base/TcpSocket.h"

namespace Linx
{
	/**
	 * A tcp client socket used to connect to the tcp server
	 */
	class TcpClientSocket : public TcpSocket
	{
	public:
		using Super = TcpSocket;

		TcpClientSocket() noexcept;
		TcpClientSocket(TcpClientSocket&& InSocket) noexcept :
			Super(std::move(InSocket)),
			ConnectionTimeoutSeconds(InSocket.ConnectionTimeoutSeconds)
		{}

	public:
		/** Connects to the tcp server. */
		bool Connect() const noexcept;
		bool Connect(const std::string& IP, int Port) noexcept;

	public:
		/** Set the connection timeout period. */
		void SetConnectTimeout(float InTimeoutSeconds) noexcept	{ ConnectionTimeoutSeconds = InTimeoutSeconds * 1000; }

	private:
		/** Connection timeout period */
		int ConnectionTimeoutSeconds = 1000;
	};
}
