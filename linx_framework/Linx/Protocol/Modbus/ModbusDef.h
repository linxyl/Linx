#pragma once

namespace Linx
{
	namespace EModbusFunctionCode
	{
		enum Type : uint8_t
		{
			ReadCoil = 1,
			ReadDiscreteInput,
			ReadHoldRegister,
			ReadInputRegister,
			WriteCoil,
			WriteHoldRegister,
			WriteMultCoil = 15,
			WriteMultHoldRegister
		};
	}

#pragma pack(push, 1)
	/**
	 * Modbus Application Protocol Header
	 */
	struct MBAP_Head
	{
		uint16_t TransactionId;

		uint16_t ProtocolId;

		uint16_t Length;

		uint8_t UnitId = 255;
	};

	/**
	 * Protocol Data Unit
	 */
	struct ModbusPDU
	{
		EModbusFunctionCode::Type FunctionCode;
	};
#pragma pack(pop)
}