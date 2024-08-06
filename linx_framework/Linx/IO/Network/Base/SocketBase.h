#pragma once

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#endif

namespace Linx
{
	/**
	 * A base class of the socket class
	 */
	class SocketBase
	{
	public:
#ifdef _WIN32
		using socket_type = SOCKET;
#else
		using socket_type = int;
#endif

	public:
		SocketBase();
		virtual ~SocketBase();

	public:
		/** Initialize and create socket. */
		virtual void Init() {};

		/** Close this socket */
		virtual void Close() noexcept;

		/** Set the IP and port of the target address to communicate with. */
		virtual void SetTargetAddr(const char* IP, int Port) noexcept;

		/** Select a port to receive remote message. */
		virtual bool Bind(int Port) noexcept;

		/**
		 * Receive data from remote address.
		 * @return	Number of bytes received. If failed, returns -1.
		 */
		virtual int Recv(char* buf, size_t bufsize) noexcept = 0;

		/**
		 * Send data to remote address.
		 * @return	Number of bytes sent. If failed, returns -1.
		 */
		virtual int Send(const char* buf, size_t bufsize) noexcept = 0;

		/** Same as Recv. */
		inline int Read(char* buf, size_t bufsize) noexcept { return Recv(buf, bufsize); }

		/** Sample as Send. */
		inline int Write(const char* buf, size_t bufsize) noexcept { return Send(buf, bufsize); }

	public:
		/** Set the timeout period for receiving. */
		void SetRecvTimeout(int mseconds) noexcept;

		/** Set the timeout period for sending. */
		void SetSendTimeout(int mseconds) noexcept;

		/** Set the size of the receiving buffer. */
		void SetRecvBufSize(int size) noexcept;

		/** Set the size of the sending buffer. */
		void SetSendBufSize(int size) noexcept;

	public:
		/** Get the number of the last error. */
		unsigned int GetLastError() noexcept;

	protected:
		/** An ID of the self socket */
		socket_type Sock = 0;
		/** An ID of the target socket */
		socket_type TargetSock = 0;

		/** Stores self socket information */
		sockaddr_in Addr;
		/** Stores the target socket information */
		sockaddr_in TargetAddr;
	};
}
