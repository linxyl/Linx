#pragma once

#include "SocketBase.h"

namespace Linx
{
	/**
	 * A base class of tcp socket
	 */
	class TcpSocket : public SocketBase
	{
		using Super = SocketBase;

	public:
		TcpSocket() = default;

	public:
		// Begin SocketBase Interface.
		virtual void Init() override;
		virtual int Recv(char* buf, size_t bufsize) noexcept override;
		virtual int Send(const char* buf, size_t bufsize) noexcept override;
		// End SocketBase Interface.

		/**
		 * Send files directly to the remote socket.
		 * @param Size	The size to send. If it is 0, send the entire file.
		 */
		long SendFile(HANDLE InHandle, size_t Size);
		long SendFile(const class File& InFile, size_t Size);

	protected:
		/** Server socket */
		socket_type* pServerSock;
		/** Client socket */
		socket_type* pClientSock;
	};
}
