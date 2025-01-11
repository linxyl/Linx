#pragma once

#include <stdint.h>
#ifdef _WIN32
#include <windows.h>
#ifdef _MSC_VER
#pragma comment(lib, "ws2_32.lib")
#endif
#else
#include <arpa/inet.h>
#include <netdb.h>
#endif
#include <string>

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
		using HANDLE = int;
		using socket_type = int;
#endif

	public:
		SocketBase();
		SocketBase(SocketBase&& InSocket);
		virtual ~SocketBase();

	public:
		/** Initialize and create socket. */
		virtual void Init() {};

		/** Close this socket */
		virtual void Close() noexcept;

		/** Set the IP or domain name and port of the target address to communicate with. */
		hostent* SetTargetAddr(const std::string& Target, int Port) noexcept;

		/** Select a port to receive remote message. */
		bool Bind(int Port) noexcept;

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

		/** Same as Recv, but returns 0 if failed. */
		inline long Read(char* buf, size_t bufsize) noexcept
		{
			return Recv(buf, bufsize);
		}

		/** Sample as Send, but returns 0 if failed. */
		inline long Write(const char* buf, size_t bufsize) noexcept
		{
			return Send(buf, bufsize);
		}

		/** Returns the socket. */
		inline socket_type GetSocket() const noexcept { return Sock; }

	public:
		/** Set the timeout period for receiving. */
		void SetRecvTimeout(int mseconds) noexcept;

		/** Set the timeout period for sending. */
		void SetSendTimeout(int mseconds) noexcept;

		/** Set the size of the receiving buffer. */
		void SetRecvBufSize(int size) noexcept;

		/** Set the size of the sending buffer. */
		void SetSendBufSize(int size) noexcept;

		/** Set whether to receive all the data of the requested length before returning. */
		inline void SetRecvAll(bool Val) noexcept { bRecvAll = Val; }

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

		uint8_t bRecvAll : 1;
	};
}
