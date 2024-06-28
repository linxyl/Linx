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

#ifdef _WIN32
		/** Server socket */
		SOCKET* pServerSock;
		/** Client socket */
		SOCKET* pClientSock;

#elif __linux__
		/** Server socket */
		int* pServerSock;
		/** Client socket */
		int* pClientSock;
#endif
	};
}
