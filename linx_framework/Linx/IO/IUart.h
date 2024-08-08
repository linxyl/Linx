#pragma once

#include "Uart.h"

namespace Linx
{
	class IUart
	{
	public:
		inline IUart(Uart& InUart) noexcept : UartRef(InUart) {};

	public:
		/** Open Uart. */
		inline bool Open(const char* PortName, UartConfig InConfig = UartConfig()) { return UartRef.Open(PortName, InConfig); }
		inline bool Open(const std::string& PortName, UartConfig InConfig = UartConfig())
		{
			return Open(PortName.c_str(), InConfig);
		}

		/** Close Uart. */
		inline void Close() { UartRef.Close(); }

		/** Read data from Uart. */
		inline size_t Read(void* Buf, size_t Size) { return UartRef.Read(Buf, Size); }

		/** Write data to Uart. */
		inline size_t Write(void* Buf, size_t Size) { return UartRef.Write(Buf, Size); }

		/**
		 *  Set the size of the buffer.
		 *  @note Invalid in Linux.
		 */
		inline bool SetBufSize(uint32_t ReadSize, uint32_t WriteSize) const { return UartRef.SetBufSize(ReadSize, WriteSize); }

		/** Returns the Uart configuration. */
		inline UartConfig GetConfig() const noexcept { return UartRef.GetConfig(); }

		/** Returns all available Uart names. */
		static inline std::vector<std::string> GetAllUartNames() { return Uart::GetAllUartNames(); }

		inline UartTimeout GetTimeout() const noexcept { return UartRef.Timeout; }

		inline void SetTimeout(const UartTimeout& InTimeout) noexcept { UartRef.Timeout = InTimeout; }

	private:
		Uart& UartRef;
	};
}
