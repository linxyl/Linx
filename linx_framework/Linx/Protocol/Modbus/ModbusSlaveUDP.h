#pragma once

#include <stdint.h>
#include <stddef.h>
#include <map>

#include "Linx/IO/Network/UdpSocket.h"
#include "ModbusSlaveDevice.h"

namespace Linx
{
	class ModbusSlaveUDP
	{
	public:
		using ExceptionCodeType = Modbus::EExceptionCode;
		static constexpr size_t Bufsize = 1024;

		ModbusSlaveUDP() = default;
		ModbusSlaveUDP(size_t DevddceNum);

	public:
		bool AddDevice(const ModbusSlaveDevice& InDevice);

		bool Bind(int Port = 502) noexcept { return UdpSocket_Inst.Bind(Port); }

		bool ResponseQuery();

		inline UdpSocket& GetSocket() noexcept { return UdpSocket_Inst; }

	private:
		void Response(const Modbus::QueryPDU& QueryPDU, const std::vector<uint16_t>& Data = std::vector<uint16_t>());
		void ResponseException(const Modbus::QueryPDU& QueryPDU, ExceptionCodeType ExceptionCode);

		static size_t MakeResponse(uint8_t* Buf, const Modbus::QueryPDU& QueryPDU, const std::vector<uint16_t>& Data);
		static size_t MakeExceptionResponse(uint8_t* Buf, const Modbus::QueryPDU& QueryPDU, ExceptionCodeType ExceptionCode);

		static size_t MakeResponsePDU(uint8_t* Buf, const Modbus::QueryPDU& QueryPDU);
		static size_t MakeResponsePDU(uint8_t* Buf, const Modbus::QueryPDU& QueryPDU, const std::vector<uint16_t>& Data);
		
		UdpSocket UdpSocket_Inst;

		std::map<uint8_t, ModbusSlaveDevice> Devices;

		uint8_t RecvBuf[Bufsize];
	};
}