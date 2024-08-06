#pragma once

#include "Base/ISocketBase.h"
#include "UdpSocket.h"

namespace Linx
{
	/**
	 * Interface of UdpSocket.
	 */
	class IUdpSocket : public ISocketBase
	{
	public:
		using Super = ISocketBase;

		IUdpSocket(UdpSocket& InSocket) : Super(InSocket), Socket(InSocket)
		{};

	private:
		UdpSocket& Socket;
	};
}
