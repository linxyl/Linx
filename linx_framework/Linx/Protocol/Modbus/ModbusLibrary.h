#include "ModbusDef.h"

namespace Linx
{
	namespace Modbus
	{
		static constexpr const char* ToString(EExceptionCode ExceptionCode)
		{
			switch (ExceptionCode)
			{
			case EExceptionCode::Success: return "Success";
			case EExceptionCode::IllegalFunction: return "IllegalFunction";
			case EExceptionCode::IllegalDataAddress: return "IllegalDataAddress";
			case EExceptionCode::IllegalDataValue: return "IllegalDataValue";
			case EExceptionCode::SlaveDeviceFailure: return "SlaveDeviceFailure";
			case EExceptionCode::Acknowledge: return "Acknowledge";
			case EExceptionCode::laveBusy: return "laveBusy";
			case EExceptionCode::MemoryParityError: return "MemoryParityError";
			default: return "Invalid exception";
			}
		}

		static ERegisterType FunctionCodeToRegisterType(uint8_t InType)
		{
			switch (InType)
			{
			case EFunctionCode::ReadCoil:
				return ERegisterType::Coil;
			case EFunctionCode::ReadDiscreteInput:
				return ERegisterType::DiscreteInput;
			case EFunctionCode::ReadHoldingRegister:
				return ERegisterType::HoldingRegister;
			case EFunctionCode::ReadInputRegister:
				return ERegisterType::InputRegister;
			default:
				break;
			}

			switch (InType)
			{
			case EFunctionCode::WriteCoil:
			case EFunctionCode::WriteMultiCoil:
				return ERegisterType::Coil;
			case EFunctionCode::WriteHoldingRegister:
			case EFunctionCode::WriteMultiHoldingRegister:
				return ERegisterType::HoldingRegister;
			default:
				break;
			}
		}

		constexpr size_t CalculateSendLength(EFunctionCode::ReadType FunctionCode)
		{
			return sizeof(QueryPDU);
		}

		constexpr size_t CalculateSendLength(EFunctionCode::WriteType FunctionCode)
		{
			return sizeof(QueryPDU);
		}

		constexpr size_t CalculateSendLength(EFunctionCode::WriteType FunctionCode, size_t DataLength)
		{
			return sizeof(QueryPDU_WriteMulti) + DataLength * sizeof(uint16_t);
		}
	}
}
