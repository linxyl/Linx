#include <stdexcept>

#include "ModbusSlaveUDP.h"

using namespace Linx;
using namespace std;
using namespace Modbus;

ModbusSlaveUDP::ModbusSlaveUDP(size_t DevceNum)
{
	for (size_t i = 1; i <= DevceNum; i++)
	{
		AddDevice(ModbusSlaveDevice(i));
	}
}

bool ModbusSlaveUDP::AddDevice(const ModbusSlaveDevice& InDevice)
{
	if (Devices.end() == Devices.find(InDevice.Id))
	{
		Devices.insert({ InDevice.Id, InDevice });
		return true;
	}
	return false;
}

bool ModbusSlaveUDP::ResponseQuery()
{
	auto RecvSize = UdpSocket_Inst.Recv((char*)RecvBuf, sizeof(RecvBuf));
	if (RecvSize <= 0)
	{
		return false;
	}
	auto* PDU = (QueryPDU*)RecvBuf;

	auto It = Devices.find(PDU->Head.UnitId);
	if (Devices.end() == It)
	{
		return false;
	}

	switch (PDU->RFunctionCode)
	{
	case EFunctionCode::ReadCoil:
	case EFunctionCode::ReadDiscreteInput:
	case EFunctionCode::ReadHoldingRegister:
	case EFunctionCode::ReadInputRegister:
	{
		vector<uint16_t> Data;
		EExceptionCode ExceptionCode = It->second.ProcessReadQuery(RecvBuf, Data);
		if (EExceptionCode::Success != ExceptionCode)
		{
			ResponseException(*PDU, ExceptionCode);
			return false;
		}

		Response(*PDU, Data);
		return true;
	}
	default:
		break;
	}

	switch (PDU->WFunctionCode)
	{
	case EFunctionCode::WriteCoil:
	case EFunctionCode::WriteHoldingRegister:
	case EFunctionCode::WriteMultiCoil:
	case EFunctionCode::WriteMultiHoldingRegister:
	{
		EExceptionCode ExceptionCode = It->second.ProcessWriteQuery(RecvBuf);
		if (EExceptionCode::Success != ExceptionCode)
		{
			ResponseException(*PDU, ExceptionCode);
			return false;
		}

		Response(*PDU);
		return true;
	}
	default:
		break;
	}

	ResponseException(*PDU, EExceptionCode::IllegalFunction);
	return false;
}

void ModbusSlaveUDP::Response(const QueryPDU& QueryPDU, const std::vector<uint16_t>& Data)
{
	uint8_t Buf[Bufsize];
	auto Size = MakeResponse(Buf, QueryPDU, Data);
	auto Ret = UdpSocket_Inst.Send((char*)Buf, Size);
}

void ModbusSlaveUDP::ResponseException(const QueryPDU& QueryPDU, EExceptionCode ExceptionCode)
{

	uint8_t Buf[Bufsize];
	auto Size = MakeExceptionResponse(Buf, QueryPDU, ExceptionCode);
	auto Ret = UdpSocket_Inst.Send((char*)Buf, Size);
}

size_t ModbusSlaveUDP::MakeResponse(uint8_t* Buf, const QueryPDU& QueryPDU, const std::vector<uint16_t>& Data)
{
	if (0 != QueryPDU.Head.GetProtocolId())
	{
		return 0;
	}

	switch (QueryPDU.RFunctionCode)
	{
	case EFunctionCode::ReadCoil:
		break;
	case EFunctionCode::ReadDiscreteInput:
		break;
	case EFunctionCode::ReadHoldingRegister:
	case EFunctionCode::ReadInputRegister:
		return MakeResponsePDU(Buf, QueryPDU, Data);
		break;
	default:
		break;
	}

	switch (QueryPDU.WFunctionCode)
	{
	case EFunctionCode::WriteCoil:
		break;
	case EFunctionCode::WriteMultiCoil:
		break;
	case EFunctionCode::WriteHoldingRegister:
	case EFunctionCode::WriteMultiHoldingRegister:
		return MakeResponsePDU(Buf, QueryPDU);
		break;
	}
}

size_t ModbusSlaveUDP::MakeExceptionResponse(uint8_t* Buf, const QueryPDU& QueryPDU, EExceptionCode ExceptionCode)
{
	ResponseExceptionPDU* pPDU = (ResponseExceptionPDU*)Buf;
	pPDU->ConvertFromQueryPDU(QueryPDU);
	pPDU->Head.SetLength(sizeof(ResponseExceptionPDU) - offsetof(MBAPHead, UnitId));
	pPDU->RFunctionCode = (EFunctionCode::ReadType)(0x80 | pPDU->RFunctionCode);
	pPDU->ExceptionCode = ExceptionCode;
	return sizeof(ResponseExceptionPDU);
}

size_t ModbusSlaveUDP::MakeResponsePDU(uint8_t* Buf, const QueryPDU& QueryPDU)
{
	ResponseWritePDU* pPDU = (ResponseWritePDU*)Buf;
	pPDU->ConvertFromQueryPDU(QueryPDU);
	pPDU->Head.SetLength(sizeof(ResponseWritePDU) - offsetof(MBAPHead, UnitId));
	pPDU->SetAddr(QueryPDU.GetAddress());
	if (EFunctionCode::WriteHoldingRegister == pPDU->WFunctionCode)
	{
		pPDU->SetDataValue(QueryPDU.GetDataValue());
	}
	else if (EFunctionCode::WriteMultiHoldingRegister == pPDU->WFunctionCode)
	{
		pPDU->SetDataCount(QueryPDU.GetDataCount());
	}
	return sizeof(ResponseWritePDU);
}

size_t ModbusSlaveUDP::MakeResponsePDU(uint8_t* Buf, const QueryPDU& QueryPDU, const std::vector<uint16_t>& Data)
{
	ResponseReadPDU* pPDU = (ResponseReadPDU*)Buf;
	pPDU->ConvertFromQueryPDU(QueryPDU);
	pPDU->Head.SetLength(sizeof(ResponseReadPDU) - offsetof(MBAPHead, UnitId) + QueryPDU.GetDataCount() * sizeof(uint16_t));
	pPDU->DataBytes = QueryPDU.GetDataCount() * sizeof(uint16_t);
	auto SendSize = sizeof(ResponseReadPDU) + QueryPDU.GetDataCount() * sizeof(uint16_t);

	uint16_t* pData = (uint16_t*)(Buf + sizeof(ResponseReadPDU));
	auto DataLength = QueryPDU.GetDataCount() / sizeof(uint16_t);
	for (const auto it : Data)
	{
		*pData++ = ConvertEndian(it);
	}
	return SendSize;
}
