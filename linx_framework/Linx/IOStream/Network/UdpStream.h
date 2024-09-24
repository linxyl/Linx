#pragma once

#include "Linx/IOStream/StreambufBase.h"
#include "Linx/IO/Network/UdpSocket.h"

namespace Linx
{
	/**
	 * A stream with UdpSocket function.
	 */
	class UdpStream : public std::iostream, public UdpSocket
	{
	public:
		UdpStream() : std::iostream(&Buf), Buf(this)
		{};

	protected:
		StreambufBase<UdpSocket> Buf;
	};
}
