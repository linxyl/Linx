#pragma once

#include <set>
#include "Base/TcpSocket.h"

namespace Linx
{
	/**
	 * Listen for connections from tcp clients and transfer data
	 */
	class TcpServerSocket : public TcpSocket
	{
	public:
		using Super = TcpSocket;

		TcpServerSocket();

	public:
		/** Set self socket to listen and enables the server to receive connection requests from clients. */
		bool Listen(size_t Num = 1) noexcept;

		/**
		 * Whether to switch the target of the communication upon successful acceptance.
		 * @param bSwitch	Whether switch target client socket.
		 */
		bool Accept(bool bSwitchClient = true) noexcept;

		/** Set the target client to communicate with. */
		void SetTargetClient(socket_type ClientSocket) noexcept;

		/** Close the connection to the client. */
		void CloseClient(socket_type ClientSocket) noexcept;

		// Begin SocketBase Interface.
		virtual void Init() override;
		virtual void Close() noexcept override;
		// End SocketBase Interface.

		/** Returns the reference of the connected client sockets. */
		const std::set<socket_type>& GetClientSockets() const noexcept { return ClientSockets; }

	private:
		/** Connected client sockets */
		std::set<socket_type> ClientSockets;
	};
}
