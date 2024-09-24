#pragma once

#include "Linx/IOStream/StreambufBase.h"
#include "Linx/IO/Uart.h"

namespace Linx
{
	/**
	 * A file stream that can rotate depending on time or size.
	 */
	class UartStream : public std::iostream, public Uart
	{
	public:
		UartStream() : std::iostream(&Buf), Buf(this)
		{};
		UartStream(const char* PortName, UartConfig InConfig = UartConfig())
			:std::iostream(&Buf), Buf(this), Uart(PortName, InConfig)
		{}
		inline UartStream(const std::string& PortName, UartConfig InConfig = UartConfig())
			:std::iostream(&Buf), Buf(this), Uart(PortName, InConfig)
		{};

	protected:
		StreambufBase<Uart> Buf;
	};
}
