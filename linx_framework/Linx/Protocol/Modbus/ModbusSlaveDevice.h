#pragma once

#include <stdint.h>
#include <stddef.h>
#include <vector>

#include "ModbusDef.h"

namespace Linx
{
	class ModbusSlaveDevice
	{
	public:
		using RegisterType = Modbus::ERegisterType;
		using ExceptionCodeType = Modbus::EExceptionCode;

		struct GroupInfo
		{
			uint16_t* DataPtr = nullptr;
			uint16_t Address;
			size_t Length;
		};

		ModbusSlaveDevice() = default;
		ModbusSlaveDevice(uint8_t InDeviceId) : Id(InDeviceId)
		{};
		virtual ~ModbusSlaveDevice() = default;

		bool AddRegisterGroup(RegisterType InRegType, uint16_t* DataPtr, uint16_t DataAddr, size_t DataLength);

		GroupInfo FindRegisterGroup(const Modbus::QueryPDU* InPDU) const;

		ExceptionCodeType ProcessWriteQuery(const uint8_t* Buf) const;
		ExceptionCodeType ProcessReadQuery(const uint8_t* Buf, std::vector<uint16_t>& OutData) const;

		uint8_t Id = 1;

	private:
		std::vector<GroupInfo> RegisterGroups[RegisterType::Count];
	};
}
