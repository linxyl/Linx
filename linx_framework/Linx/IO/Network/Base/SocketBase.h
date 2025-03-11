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
		SocketBase() noexcept;
		SocketBase(SocketBase&& InSocket) noexcept;
		SocketBase(socket_type InSocket) noexcept : Sock(InSocket) {};
		virtual ~SocketBase() noexcept;

	public:
		/** Initialize and create socket. */
		virtual bool Recreate() noexcept { Close(); return true; }

		/** Close this socket */
		void Close() noexcept;

		/** Set the IP or domain name and port of the target address to communicate with. */
		hostent* SetTargetAddr(const std::string& Target, int Port) noexcept;

		/** Select a port to receive remote message. */
		bool Bind(int Port) const noexcept;

		/**
		 * Receive data from remote address.
		 * @return	Number of bytes received. If failed, returns -1.
		 */
		virtual int Recv(char* buf, size_t bufsize) const noexcept = 0;

		/**
		 * Send data to remote address.
		 * @return	Number of bytes sent. If failed, returns -1.
		 */
		virtual int Send(const char* buf, size_t bufsize) const noexcept = 0;

		/** Same as Recv, but returns 0 if failed. */
		inline long Read(char* buf, size_t bufsize) const noexcept
		{
			return Recv(buf, bufsize);
		}

		/** Sample as Send, but returns 0 if failed. */
		inline long Write(const char* buf, size_t bufsize) const noexcept
		{
			return Send(buf, bufsize);
		}

		/** Returns the socket. */
		inline socket_type GetSocket() const noexcept { return Sock; }

	public:
		bool IsValid() const noexcept;

		/** Set the timeout period for receiving. */
		bool SetRecvTimeout(int mseconds) const noexcept;

		/** Set the timeout period for sending. */
		bool SetSendTimeout(int mseconds) const noexcept;

		/** Set the size of the receiving buffer. */
		bool SetRecvBufSize(int size) const noexcept;

		/** Set the size of the sending buffer. */
		bool SetSendBufSize(int size) const noexcept;

		/** Set whether to receive all the data of the requested length before returning. */
		inline void SetRecvAll(bool Val) noexcept { bRecvAll = Val; }

	public:
		/** Get the number of the last error. */
		unsigned int GetLastError() const noexcept;

	protected:
		/** An ID of the self socket */
		socket_type Sock = 0;

		/** Stores the target socket information */
		sockaddr_in TargetAddr;

		uint8_t bRecvAll : 1;
	};
}
