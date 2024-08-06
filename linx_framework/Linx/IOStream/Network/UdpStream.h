#pragma once

#include "Linx/IOStream/StreambufBase.h"
#include "Linx/IO/Network/IUdpSocket.h"

namespace Linx
{
	class UdpBuf : public StreambufBase<UdpSocket>
	{
		friend class UdpStream;
	};

	/**
	 * A stream with UdpSocket function.
	 */
	class UdpStream : public std::iostream, public IUdpSocket
	{
	public:
		UdpStream() : std::iostream(&Buf), IUdpSocket(Buf.IO)
		{};

	protected:
		UdpBuf Buf;
	};
}
