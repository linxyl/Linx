#pragma once

#include "Linx/IOStream/StreambufBase.h"
#include "Linx/IO/Serial.h"

namespace Linx
{
	/**
	 * A file stream that can rotate depending on time or size.
	 */
	class SerialStream : public std::iostream, public Serial
	{
	public:
		SerialStream() : std::iostream(&Buf), Buf(this)
		{};
		SerialStream(const char* PortName, SerialConfig InConfig = SerialConfig())
			:std::iostream(&Buf), Buf(this), Serial(PortName, InConfig)
		{}
		inline SerialStream(const std::string& PortName, SerialConfig InConfig = SerialConfig())
			:std::iostream(&Buf), Buf(this), Serial(PortName, InConfig)
		{};

	protected:
		StreambufBase<Serial> Buf;
	};
}
