#pragma once

#include <set>
#include "Base/TcpSocket.h"

namespace Linx
{
	/**
	 * Listen for connections from tcp clients and transfer data
	 */
	class TcpServerSocket : private TcpSocket
	{
	public:
		using Super = TcpSocket;

		TcpServerSocket();
		TcpServerSocket(TcpServerSocket&& InSocket) = default;

	public:
		/** Select a port to receive remote message. */
		bool Bind(int Port) const noexcept { return Super::Bind(Port); }

		/** Listen for incoming connection requests. */
		bool Listen(size_t Num = 1) const noexcept { return 0 == listen(Sock, Num); }

		/** Accept a connection on a socket. */
		TcpSocket Accept() const noexcept;

		// Begin SocketBase Interface.
		virtual bool Recreate() noexcept override;
		// End SocketBase Interface.
	};
}
