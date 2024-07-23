#include "Linx/IO/Uart.h"

#include <stdint.h>

struct UartConfig 
{
	int Baudrate;
	uint8_t Parity;
	uint8_t DataBit;
	uint8_t StopBit;
};

Linx::Uart::Uart(const char* PortName)
{
	Open(PortName);
}

bool Linx::Uart::Open(const char* PortName)
{
	return true;
}
