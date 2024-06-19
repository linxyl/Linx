#pragma once

#include "Base/TcpSocket.h"

namespace linx
{
	/**
	 * Listen for connections from tcp clients and transfer data
	 */
	class TcpServerSocket : public TcpSocket
	{
		using Super = TcpSocket;

	public:
		TcpServerSocket();

	public:
		/** Set self socket to listen and enables the server to receive connection requests from clients. */
		bool Listen() noexcept;

		/** Accepts connection requests from clients. */
		bool Accept() noexcept;

	public:
		// Begin SocketBase Interface.
		virtual void Close() noexcept override;
		// End SocketBase Interface.

	private:
		/** @note Should not be called. */
		virtual void SetTargetAddr(const char* IP, int port) noexcept {}
	};
}
