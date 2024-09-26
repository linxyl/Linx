#pragma once

#ifdef _WIN32
#include <Windows.h>
#else
#include <termios.h>
#endif
#include <stdint.h>

#include <vector>
#include <string>

#include "IOBase.h"

namespace Linx
{
	namespace UartParam
	{
#ifdef _WIN32
		enum EBaudRate : uint32_t
		{
			BR_0    = 0,
			BR_50   = 50,
			BR_75   = 75,
			BR_110  = 110,
			BR_134  = 134,
			BR_150  = 150,
			BR_200  = 200,
			BR_300  = 300,
			BR_600  = 600,
			BR_1200 = 1200,
			BR_1800 = 1800,
			BR_2400 = 2400,
			BR_4800 = 4800,
			BR_9600 = 9600,
			BR_14400 = 14400,
			BR_19200 = 19200,
			BR_38400 = 38400,
			BR_56000 = 56000,
			BR_57600 = 57600,
			BR_115200 = 115200,
			BR_230400 = 230400,
			BR_460800 = 460800,
			BR_500000 = 500000,
			BR_576000 = 576000,
			BR_921600 = 921600,
			BR_1000000 = 1000000,
			BR_1152000 = 1152000,
			BR_1500000 = 1500000,
			BR_2000000 = 2000000,
			BR_2500000 = 2500000,
			BR_3000000 = 3000000,
			BR_3500000 = 3500000,
			BR_4000000 = 4000000
		};

		enum EDataBits : uint8_t
		{
			FiveDataBits = 5,
			SixDataBits,
			SevenDataBits,
			EightDataBits
		};

		enum EStopBits : uint8_t
		{
			OneStopBit = 0,
			One5StopBits,
			TwoStopBits
		};

		enum EParity : uint8_t
		{
			NoParity = 0,
			OddParity,
			EvenParity,
			MarkParity,
			SpaceParity
		};
#else // _WIN32
		enum EBaudRate : uint32_t
		{
			BR_0 = B0,
			BR_50 = B50,
			BR_75 = B75,
			BR_110 = B110,
			BR_134 = B134,
			BR_150 = B150,
			BR_200 = B200,
			BR_300 = B300,
			BR_600 = B600,
			BR_1200 = B1200,
			BR_1800 = B1800,
			BR_2400 = B2400,
			BR_4800 = B4800,
			BR_9600 = B9600,
//			BR_14400 = B14400,
			BR_19200 = B19200,
			BR_38400 = B38400,
//			BR_56000 = B56000,
			BR_57600 = B57600,
			BR_115200 = B115200,
			BR_230400 = B230400,
			BR_460800 = B460800,
			BR_500000 = B500000,
			BR_576000 = B576000,
			BR_921600 = B921600,
			BR_1000000 = B1000000,
			BR_1152000 = B1152000,
			BR_1500000 = B1500000,
			BR_2000000 = B2000000,
			BR_2500000 = B2500000,
			BR_3000000 = B3000000,
			BR_3500000 = B3500000,
			BR_4000000 = B4000000
		};

		enum EDataBits : uint8_t
		{
			FiveDataBits = CS5,
			SixDataBits = CS6,
			SevenDataBits = CS7,
			EightDataBits = CS8
		};

		enum EStopBits : uint8_t
		{
			OneStopBit = 0,
//			One5StopBits,
			TwoStopBits
		};

		enum EParity : uint8_t
		{
			NoParity = 0,
			OddParity,
			EvenParity
//			MarkParity,
//			SpaceParity
		};
#endif // _WIN32
	}

	struct UartConfig
	{
		UartParam::EBaudRate Baudrate = UartParam::BR_115200;
		UartParam::EParity Parity = UartParam::NoParity;
		UartParam::EDataBits DataBits = UartParam::EightDataBits;
		UartParam::EStopBits StopBits = UartParam::OneStopBit;
		bool bSync = true;
	};

	struct UartTimeout
	{
		uint32_t ReadIntervalTimeout = -1;			/* Maximum time between read chars. */
		uint32_t ReadTotalTimeoutMultiplier = 0;	/* Multiplier of characters.        */
		uint32_t ReadTotalTimeoutConstant = 0;		/* Constant in milliseconds.        */
		uint32_t WriteTotalTimeoutMultiplier = 0;	/* Multiplier of characters.        */
		uint32_t WriteTotalTimeoutConstant = 0;		/* Constant in milliseconds.        */
	};

	class Uart : public IOBase
	{
	public:
		Uart() {};

		/** Open Uart. */
		Uart(const char* PortName, UartConfig InConfig = UartConfig());
		inline Uart(const std::string& PortName, UartConfig InConfig = UartConfig()) :
			Uart(PortName.c_str(), InConfig)
		{};

		virtual ~Uart() { Close(); }

	public:
		/** Open Uart. */
		bool Open(const char* PortName, UartConfig InConfig = UartConfig());
		inline bool Open(const std::string& PortName, UartConfig InConfig = UartConfig())
		{
			return Open(PortName.c_str(), InConfig);
		}

		/** Close Uart. */
		void Close();

		/** Read data from Uart. */
		size_t Read(void* Buf, size_t Size);

		/** Write data to Uart. */
		size_t Write(void* Buf, size_t Size);

		/**
		 *  Set the size of the buffer.
		 *  @note Invalid in Linux.
		 */
		bool SetBufSize(uint32_t ReadSize, uint32_t WriteSize) const;

		/** Returns the Uart configuration. */
		inline UartConfig GetConfig() const noexcept { return Config; }

		/** Returns all available Uart names. */
		static std::vector<std::string> GetAllUartNames();

	public:
		UartTimeout Timeout;

	private:
		UartConfig Config;
	};
}
