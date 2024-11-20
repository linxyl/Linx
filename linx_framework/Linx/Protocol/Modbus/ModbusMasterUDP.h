#pragma once

#include <stdint.h>
#include <stddef.h>
#include <vector>

#include "Linx/IO/Network/UdpSocket.h"
#include "ModbusDef.h"

namespace Linx
{
	class ModbusMasterUDP
	{
	public:
		using ExceptionCodeType = Modbus::EExceptionCode;
		using ReadType = Modbus::EFunctionCode::ReadType;
		using WriteType = Modbus::EFunctionCode::WriteType;

		static constexpr size_t Bufsize = 1024;

		inline ModbusMasterUDP() noexcept { SetTimeout(1000); }

	public:
		/** Returns the udp socket. */
		inline UdpSocket& GetSocket() noexcept { return UdpSocket_Inst; }

		/** Set target address and port, generic port is 502. */
		inline hostent* SetTargetAddr(const std::string& Target, int Port = 502) noexcept
		{
			return UdpSocket_Inst.SetTargetAddr(Target, Port);
		}

		/** Set timeout, in milliseconds. */
		inline void SetTimeout(int mseconds) noexcept { UdpSocket_Inst.SetRecvTimeout(mseconds); }

		/**
		 * Read data from the slave.
		 * @param SlaveId		Identity of the device, if is 0, broadcast.
		 * @param FunctionCode	Function code, must be read type.
		 * @param Address		Address in the device.
		 * @param DataLength	Read length.
		 * @param ReadData		Store the read data.
		 * @return exception code.
		 */
		ExceptionCodeType Read(
			uint8_t SlaveId,
			ReadType FunctionCode,
			uint16_t Address,
			uint16_t DataLength,
			std::vector<uint16_t>& ReadData);
		ExceptionCodeType Read32(
			uint8_t SlaveId,
			ReadType FunctionCode,
			uint16_t Address,
			uint16_t DataLength,
			std::vector<uint32_t>& ReadData);

		/**
		 * Write data to the slave.
		 * @param SlaveId		Identity of the device, if is 0, broadcast.
		 * @param FunctionCode	Function code, must be write type.
		 * @param Address		Address in the device.
		 * @param WriteData		Data to be write.
		 * @return exception code.
		 */
		ExceptionCodeType Write(uint8_t SlaveId, WriteType FunctionCode, uint16_t Address, uint16_t WriteData);
		ExceptionCodeType Write32(uint8_t SlaveId, WriteType FunctionCode, uint16_t Address, uint32_t WriteData);
		ExceptionCodeType Write(
			uint8_t SlaveId,
			WriteType FunctionCode,
			uint16_t Address,
			const std::vector<uint16_t>& WriteData);
		ExceptionCodeType Write32(
			uint8_t SlaveId,
			WriteType FunctionCode,
			uint16_t Address,
			const std::vector<uint32_t>& WriteData);

	private:
		void MakePDU(
			uint8_t* Buf,
			size_t SendSize,
			uint8_t UnitId,
			ReadType FunctionCode,
			uint16_t Address,
			uint16_t DataLength);
		void MakeWriteOnePDU(
			uint8_t* Buf,
			size_t SendSize,
			uint8_t UnitId,
			WriteType FunctionCode,
			uint16_t Address,
			uint16_t Value);
		void MakeWriteMultiPDU(
			uint8_t* Buf,
			size_t SendSize,
			uint8_t UnitId,
			WriteType FunctionCode,
			uint16_t Address,
			const std::vector<uint16_t>& Data);
		
		ExceptionCodeType ParseReadPDU(
			uint8_t* Buf,
			size_t RecvSize,
			const Modbus::QueryPDU* QueryPDU,
			std::vector<uint16_t>& ReadData);
		ExceptionCodeType ParseWritePDU(uint8_t* Buf, size_t RecvSize, const Modbus::QueryPDU* QueryPDU);

	private:
		UdpSocket UdpSocket_Inst;

		uint16_t TransactionId = 0;
	};
}