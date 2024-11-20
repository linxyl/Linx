#pragma once

#include "SocketBase.h"

namespace Linx
{
	/**
	 * A base class of tcp socket
	 */
	class TcpSocket : public SocketBase
	{
	public:
		using Super = SocketBase;

		TcpSocket() = default;
		TcpSocket(TcpSocket&& InSocket) = default;

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

		bool KeepAlive(bool Val = true, int Idle = 1, int Interval = 1, int Count = 3);

	protected:
		/** Server socket */
		socket_type* pServerSock;
		/** Client socket */
		socket_type* pClientSock;
	};
}
