#pragma once

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#elif __linux__
#include <arpa/inet.h>
#endif

namespace linx
{
	/**
	 * A base class of the socket class
	 */
	class SocketBase
	{
	public:
		SocketBase();
		virtual ~SocketBase();

	public:
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

		/** Close this socket */
		virtual void Close() noexcept;

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
#ifdef _WIN32
		/** An ID of the self socket */
		SOCKET Sock = 0;
		/** An ID of the target socket */
		SOCKET TargetSock = 0;
#elif __linux__
		/** The ID of self socket */
		int Sock = 0;
		/** The ID of the target socket */
		int TargetSock = 0;
#endif

		/** Stores self socket information */
		sockaddr_in Addr;
		/** Stores the target socket information */
		sockaddr_in TargetAddr;
	};
}
