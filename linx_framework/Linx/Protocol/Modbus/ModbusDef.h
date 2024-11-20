#pragma once

#include "Linx/Core.h"

namespace Linx
{
	namespace Modbus
	{
		enum ERegisterType : uint8_t
		{
			Coil = 1,
			DiscreteInput,
			HoldingRegister,
			InputRegister,
			Count
		};

		namespace EFunctionCode
		{
			enum ReadType : uint8_t
			{
				ReadCoil = 1,
				ReadDiscreteInput = 2,
				ReadHoldingRegister = 3,
				ReadInputRegister = 4,
			};

			enum WriteType : uint8_t
			{
				WriteCoil = 5,
				WriteHoldingRegister = 6,
				WriteMultiCoil = 15,
				WriteMultiHoldingRegister = 16
			};
		}

		/**
		 * Type of exception when process the Modbus query failed.
		 */
		enum EExceptionCode : uint8_t
		{
			/** Succeed to query and response */
			Success = 0,

			/** Unsupported function code */
			IllegalFunction = 1,

			/** Slave does not contain the address */
			IllegalDataAddress = 2,

			/** Data format error */
			IllegalDataValue = 3,

			/** Slave cannot process the query or slave does not contain the register type */
			SlaveDeviceFailure = 4,

			/** Slave can process the query, buf cannot response */
			Acknowledge = 5,

			/** Slave cannot process the query currently */
			laveBusy = 6,

			/** Check error */
			MemoryParityError = 8
		};

#pragma pack(push, 1)
		/**
		 * Modbus Application Protocol Header, in big endian
		 */
		struct MBAPHead
		{
			friend struct QueryPDU;
			friend struct ResponsePDU;

		private:
			uint16_t TransactionId;
			uint16_t ProtocolId;
			uint16_t Length;
		public:
			uint8_t UnitId;

		public:
			inline uint16_t GetTransactionId() const noexcept { return ConvertEndian(TransactionId); }
			inline uint16_t GetProtocolId() const noexcept { return ConvertEndian(ProtocolId); }
			inline uint16_t GetLength() const noexcept { return ConvertEndian(Length); }

			inline void SetTransactionId(uint16_t Val) noexcept { TransactionId = ConvertEndian(Val); }
			inline void SetProtocolId(uint16_t Val) noexcept { ProtocolId = ConvertEndian(Val); }
			inline void SetLength(uint16_t Val) noexcept { Length = ConvertEndian(Val); }
		};

		/**
		 * Query protocol data unit, in big endian
		 */
		struct QueryPDU
		{
			MBAPHead Head;

			union
			{
				EFunctionCode::ReadType RFunctionCode;
				EFunctionCode::WriteType WFunctionCode;
			};

		private:
			uint16_t Address;

			union
			{
				uint16_t DataValue;
				uint16_t DataCount;
			};

		public:
			inline uint16_t GetAddress() const noexcept { return ConvertEndian(Address); }
			inline uint16_t GetDataCount() const noexcept { return ConvertEndian(DataCount); }
			inline uint16_t GetDataValue() const noexcept { return ConvertEndian(DataValue); }

			inline void SetAddress(uint16_t Val) noexcept { Address = ConvertEndian(Val); }
			inline void SetDataCount(uint16_t Val) noexcept { DataCount = ConvertEndian(Val); }
			inline void SetDataValue(uint16_t Val) noexcept { DataValue = ConvertEndian(Val); }
		};

		/**
		 * Query protocol data unit in Write multiple function code
		 */
		struct QueryPDU_WriteMulti : QueryPDU
		{
			uint8_t DataBytes;
		};

		/**
		 * Response protocol data unit, in big endian
		 */
		struct ResponsePDU
		{
			MBAPHead Head;

			union
			{
				EFunctionCode::ReadType RFunctionCode;
				EFunctionCode::WriteType WFunctionCode;
			};

			inline void ConvertFromQueryPDU(const QueryPDU& InPDU) noexcept
			{
				Head = InPDU.Head;
				RFunctionCode = InPDU.RFunctionCode;
			}

			inline bool Check(const QueryPDU& InPDU) const noexcept
			{
				return this->Head.TransactionId == InPDU.Head.TransactionId &&
					this->Head.ProtocolId == InPDU.Head.ProtocolId &&
					this->Head.UnitId == InPDU.Head.UnitId &&
					(this->RFunctionCode & 0x7f) == InPDU.RFunctionCode;
			}
		};

		struct ResponseReadPDU : ResponsePDU
		{
			uint8_t DataBytes;
		};

		struct ResponseWritePDU : ResponsePDU
		{
		private:
			uint16_t Addr;
			union
			{
				uint16_t DataValue;
				uint16_t DataCount;
			};

		public:
			inline uint16_t GetAddr() const noexcept { return ConvertEndian(Addr); }
			inline uint16_t GetDataValue() const noexcept { return ConvertEndian(DataValue); }
			inline uint16_t GetDataCount() const noexcept { return ConvertEndian(DataCount); }

			inline void SetAddr(uint16_t Val) noexcept { Addr = ConvertEndian(Val); }
			inline void SetDataValue(uint16_t Val) noexcept { DataValue = ConvertEndian(Val); }
			inline void SetDataCount(uint16_t Val) noexcept { DataCount = ConvertEndian(Val); }
		};

		struct ResponseExceptionPDU : ResponsePDU
		{
			EExceptionCode ExceptionCode;
		};
#pragma pack(pop)
	}
}