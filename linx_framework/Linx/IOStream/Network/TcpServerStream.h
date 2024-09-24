#pragma once

#include "Linx/IOStream/StreambufBase.h"
#include "Linx/IO/Network/TcpServerSocket.h"

namespace Linx
{
	/**
	 * A stream with TcpServerSocket function.
	 */
	class TcpServerStream : public std::iostream, public TcpServerSocket
	{
	public:
		TcpServerStream() : std::iostream(&Buf), Buf(this)
		{};

	protected:
		StreambufBase<TcpServerSocket> Buf;
	};
}
