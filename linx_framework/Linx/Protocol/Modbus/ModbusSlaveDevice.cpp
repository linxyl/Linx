#include <string.h>

#include "ModbusSlaveDevice.h"
#include "ModbusLibrary.h"

using namespace Linx;
using namespace std;
using namespace Modbus;

bool ModbusSlaveDevice::AddRegisterGroup(ERegisterType InRegType, uint16_t* DataPtr, uint16_t DataAddr, size_t DataLength)
{
	uint16_t InDataBegin = DataAddr;
	uint16_t InDataLast = DataAddr + DataLength;

	for (auto& it : RegisterGroups[InRegType])
	{
		uint16_t DataBegin = it.Address;
		uint16_t DataLast = it.Address + it.Length;

		if (!(InDataLast <= DataBegin || InDataBegin >= DataLast))
		{
			return false;
		}
	}
	RegisterGroups[InRegType].emplace_back(GroupInfo{ DataPtr, DataAddr, DataLength });
	return true;
}

ModbusSlaveDevice::GroupInfo ModbusSlaveDevice::FindRegisterGroup(const QueryPDU* InPDU) const
{
	uint16_t DataCount;
	if (EFunctionCode::WriteCoil == InPDU->WFunctionCode ||
		EFunctionCode::WriteHoldingRegister == InPDU->WFunctionCode)
	{
		DataCount = 1;
	}
	else
	{
		DataCount = InPDU->GetDataCount();
	}
	uint16_t InBeginAddr = InPDU->GetAddress();
	uint16_t InLastAddr = InPDU->GetAddress() + DataCount;

	for (auto& it : RegisterGroups[FunctionCodeToRegisterType(InPDU->RFunctionCode)])
	{
		uint16_t BeginAddr = it.Address;
		uint16_t LastAddr = BeginAddr + it.Length;

		if (InBeginAddr >= BeginAddr && InLastAddr <= LastAddr)
		{
			return it;
		}
	}
	return GroupInfo{};
}

ModbusSlaveDevice::ExceptionCodeType ModbusSlaveDevice::ProcessWriteQuery(const uint8_t* Buf) const
{
	const QueryPDU* InPDU = (const QueryPDU*)Buf;

	if (RegisterGroups[FunctionCodeToRegisterType(InPDU->RFunctionCode)].empty())
	{
		return EExceptionCode::SlaveDeviceFailure;
	}

	auto Group = FindRegisterGroup(InPDU);
	if (!Group.DataPtr)
	{
		return EExceptionCode::IllegalDataAddress;
	}

	switch (InPDU->WFunctionCode)
	{
	case EFunctionCode::WriteCoil:
	case EFunctionCode::WriteHoldingRegister:
		Group.DataPtr[InPDU->GetAddress()] = InPDU->GetDataValue();
		break;
	case EFunctionCode::WriteMultiCoil:
	case EFunctionCode::WriteMultiHoldingRegister:
		if(InPDU->GetDataCount() % 2)
		{
			ConvertEndianCopy((uint16_t*)(Buf + sizeof(QueryPDU_WriteMulti)), InPDU->GetDataCount(), Group.DataPtr + InPDU->GetAddress());
		}
		else
		{
			size_t Size32 = InPDU->GetDataCount() / 2;
			for(size_t i = 0; i < Size32; i++)
			{
				uint32_t Temp = ConvertEndian(*(uint16_t*)(Buf + sizeof(QueryPDU_WriteMulti) + i * sizeof(uint32_t)));
				Temp |= ConvertEndian(*((uint16_t*)(Buf + sizeof(QueryPDU_WriteMulti) + i * sizeof(uint32_t)) + 1)) << 16;
				((uint32_t*)(Group.DataPtr + InPDU->GetAddress()))[i] = Temp;
			}
		}
		break;
	default:
		return EExceptionCode::IllegalFunction;
	}
	return EExceptionCode::Success;
}

ModbusSlaveDevice::ExceptionCodeType ModbusSlaveDevice::ProcessReadQuery(const uint8_t* Buf, std::vector<uint16_t>& OutData) const
{
	const QueryPDU* InPDU = (const QueryPDU*)Buf;

	if (RegisterGroups[FunctionCodeToRegisterType(InPDU->RFunctionCode)].empty())
	{
		return EExceptionCode::SlaveDeviceFailure;
	}

	auto Group = FindRegisterGroup(InPDU);
	if (!Group.DataPtr)
	{
		return EExceptionCode::IllegalDataAddress;
	}

	switch (InPDU->RFunctionCode)
	{
	case EFunctionCode::ReadCoil:
		break;
	case EFunctionCode::ReadDiscreteInput:
		break;
	case EFunctionCode::ReadHoldingRegister:
	case EFunctionCode::ReadInputRegister:
		OutData.resize(InPDU->GetDataCount());
		for (size_t i = 0; i < OutData.size(); i++)
		{
			OutData[i] = (Group.DataPtr + InPDU->GetAddress())[i];
		}
		break;
	default:
		return EExceptionCode::IllegalFunction;
	}
	return EExceptionCode::Success;
}
