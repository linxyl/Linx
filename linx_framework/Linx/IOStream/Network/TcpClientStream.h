#pragma once

#include "Linx/IOStream/StreambufBase.h"
#include "Linx/IO/Network/TcpClientSocket.h"

namespace Linx
{
	/**
	 * A stream with TcpClientSocket function.
	 */
	class TcpClientStream : public std::iostream, public TcpClientSocket
	{
	public:
		TcpClientStream() : std::iostream(&Buf), Buf(this)
		{};

	protected:
		StreambufBase<TcpClientSocket> Buf;
	};
}
