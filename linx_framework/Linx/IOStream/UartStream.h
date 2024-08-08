#pragma once

#include "Linx/IOStream/StreambufBase.h"
#include "Linx/IO/IUart.h"

namespace Linx
{
	class UartBuf : public StreambufBase<Uart>
	{
		friend class UartStream;
	public:
		UartBuf() : StreambufBase<Uart>() {};
	};

	/**
	 * A file stream that can rotate depending on time or size.
	 */
	class UartStream : public std::iostream, public IUart
	{
	public:
		UartStream() : std::iostream(&Buf), IUart(Buf.IO)
		{};

		/** Open Uart. */
		inline UartStream(const char* PortName, UartConfig InConfig = UartConfig()) : UartStream()
		{
			Buf.IO.Open(PortName, InConfig);
		}
		inline UartStream(const std::string& PortName, UartConfig InConfig = UartConfig()) :
			UartStream(PortName.c_str(), InConfig)
		{};

	protected:
		UartBuf Buf;
	};
}
