#pragma once

#include "SocketBase.h"

namespace linx
{
	/**
	 * A base class of tcp socket
	 */
	class TcpSocket : public SocketBase
	{
		using Super = SocketBase;

	public:
		TcpSocket();

	public:
		// Begin SocketBase Interface.
		virtual int Recv(char* buf, size_t bufsize) noexcept override;
		virtual int Send(const char* buf, size_t bufsize) noexcept override;
		// End SocketBase Interface.

	protected:
		/** Server socket */
		socket_type* pServerSock;
		/** Client socket */
		socket_type* pClientSock;
	};
}
