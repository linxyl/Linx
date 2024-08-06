#pragma once

#include "SocketBase.h"

namespace Linx
{
	/**
	 * Interface of SocketBase.
	 */
	class ISocketBase
	{
	public:
		using socket_type = SocketBase::socket_type;

		ISocketBase(SocketBase& InSocket) : Socket(InSocket)
		{};

	public:
		/** Initialize and create socket. */
		inline void Init() { Socket.Init(); }

		/** Close this socket */
		inline void Close() noexcept { Socket.Close(); }

		/** Set the IP and port of the target address to communicate with. */
		inline void SetTargetAddr(const char* IP, int Port) noexcept { Socket.SetTargetAddr(IP, Port); }

		/** Select a port to receive remote message. */
		inline bool Bind(int Port) noexcept { return Socket.Bind(Port); }

		/**
		 * Receive data from remote address.
		 * @return	Number of bytes received. If failed, returns -1.
		 */
		inline int Recv(char* buf, size_t bufsize) noexcept { return Socket.Recv(buf, bufsize); }

		/**
		 * Send data to remote address.
		 * @return	Number of bytes sent. If failed, returns -1.
		 */
		inline int Send(const char* buf, size_t bufsize) noexcept { return Socket.Send(buf, bufsize); }

		/** Same as Recv. */
		inline int Read(char* buf, size_t bufsize) noexcept { return Recv(buf, bufsize); }

		/** Sample as Send. */
		inline int Write(const char* buf, size_t bufsize) noexcept { return Send(buf, bufsize); }

	public:
		/** Set the timeout period for receiving. */
		inline void SetRecvTimeout(int mseconds) noexcept { Socket.SetRecvTimeout(mseconds); }

		/** Set the timeout period for sending. */
		inline void SetSendTimeout(int mseconds) noexcept { Socket.SetSendTimeout(mseconds); }

		/** Set the size of the receiving buffer. */
		inline void SetRecvBufSize(int size) noexcept { Socket.SetRecvBufSize(size); }

		/** Set the size of the sending buffer. */
		inline void SetSendBufSize(int size) noexcept { Socket.SetSendBufSize(size); }

	public:
		/** Get the number of the last error. */
		inline unsigned int GetLastError() noexcept { return Socket.GetLastError(); }

	private:
		SocketBase& Socket;
	};
}
