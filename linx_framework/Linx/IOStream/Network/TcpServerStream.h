#pragma once

#include "Linx/IOStream/StreambufBase.h"
#include "Linx/IO/Network/ITcpServerSocket.h"

namespace Linx
{
	class TcpServerBuf : public StreambufBase<TcpServerSocket>
	{
		friend class TcpServerStream;
	};

	/**
	 * A stream with TcpServerSocket function.
	 */
	class TcpServerStream : public std::iostream, public ITcpServerSocket
	{
	public:
		TcpServerStream() : std::iostream(&Buf), ITcpServerSocket(Buf.IO)
		{};

	protected:
		TcpServerBuf Buf;
	};
}
