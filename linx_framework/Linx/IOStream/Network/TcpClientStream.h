#pragma once

#include "Linx/IOStream/StreambufBase.h"
#include "Linx/IO/Network/ITcpClientSocket.h"

namespace Linx
{
	class TcpClientBuf : public StreambufBase<TcpClientSocket>
	{
		friend class TcpClientStream;
	};

	/**
	 * A stream with TcpClientSocket function.
	 */
	class TcpClientStream : public std::iostream, public ITcpClientSocket
	{
	public:
		TcpClientStream() : std::iostream(&Buf), ITcpClientSocket(Buf.IO)
		{};

	protected:
		TcpClientBuf Buf;
	};
}
