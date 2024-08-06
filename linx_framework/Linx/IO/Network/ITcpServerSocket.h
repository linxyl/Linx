#pragma once

#include "Base/ISocketBase.h"
#include "TcpServerSocket.h"

namespace Linx
{
	/**
	 * Interface of TcpServerSocket.
	 */
	class ITcpServerSocket : public ISocketBase
	{
	public:
		using Super = ISocketBase;

		ITcpServerSocket(TcpServerSocket& InSocket) : Super(InSocket), Socket(InSocket)
		{};

	public:
		/** Set self socket to listen and enables the server to receive connection requests from clients. */
		inline bool Listen(size_t Num = 1) noexcept { return Socket.Listen(Num); }

		/**
		 * Whether to switch the target of the communication upon successful acceptance.
		 * @param bSwitch	Whether switch target client socket.
		 */
		inline bool Accept(bool bSwitchClient = true) noexcept { return Socket.Accept(bSwitchClient); }

		/** Set the target client to communicate with. */
		inline void SetTargetClient(socket_type ClientSocket) noexcept { Socket.SetTargetClient(ClientSocket); }

		/** Close the connection to the client. */
		inline void CloseClient(socket_type ClientSocket) noexcept { Socket.CloseClient(ClientSocket); }

		/** Returns the reference of the connected client sockets. */
		inline const std::set<socket_type>& GetClientSockets() const noexcept { return Socket.GetClientSockets(); }

	private:
		TcpServerSocket& Socket;
	};
}
