#pragma once

#include "Base/TcpSocket.h"

namespace linx
{
	/**
	 * A tcp client socket used to connect to the tcp server
	 */
	class TcpClientSocket : public TcpSocket
	{
		using Super = TcpSocket;

	public:
		TcpClientSocket();

	public:
		/** Connects to the tcp server. */
		bool Connect() noexcept;
		bool Connect(const char* IP, int Port) noexcept;

	public:
		/** Set the connection timeout period. */
		void SetConnectTimeout(float InTimeoutSeconds) noexcept	{ ConnectionTimeoutSeconds = InTimeoutSeconds * 1000; }

	private:
		/** Connection timeout period */
		int ConnectionTimeoutSeconds = 1000;
	};
}
