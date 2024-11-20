#include <string.h>
#include <stdexcept>

#include "ModbusMasterUDP.h"
#include "ModbusLibrary.h"

using namespace Linx;
using namespace std;
using namespace Modbus;

ModbusMasterUDP::ExceptionCodeType ModbusMasterUDP::Read(
	uint8_t SlaveId,
	EFunctionCode::ReadType FunctionCode,
	uint16_t Address,
	uint16_t DataLength,
	vector<uint16_t>& ReadData)
{
	uint8_t SendBuf[Bufsize];
	auto SendSize = CalculateSendLength(FunctionCode);
	if (SendSize > sizeof(SendBuf))
	{
		throw std::runtime_error("ModbusMasterUDP: CalculateSendLength greater than Bufsize");
	}
	MakePDU(SendBuf, SendSize, SlaveId, FunctionCode, Address, DataLength);

	UdpSocket_Inst.Send((char*)SendBuf, SendSize);

	uint8_t RecvBuf[Bufsize];
	auto RecvSize = UdpSocket_Inst.Recv((char*)RecvBuf, sizeof(RecvBuf));
	if (RecvSize <= 0)
	{
		throw std::runtime_error("ModbusMasterUDP: Recv error");
	}

	return ParseReadPDU(RecvBuf, RecvSize, (QueryPDU*)SendBuf, ReadData);
}

Linx::ModbusMasterUDP::ExceptionCodeType ModbusMasterUDP::Read32(uint8_t SlaveId, ReadType FunctionCode, uint16_t Address, uint16_t DataLength, std::vector<uint32_t>& ReadData)
{
	vector<uint16_t> TempData;
	auto Ret = Read(SlaveId, FunctionCode, Address * sizeof(uint32_t) / sizeof(uint16_t), DataLength * sizeof(uint32_t) / sizeof(uint16_t), TempData);
	if (EExceptionCode::Success == Ret)
	{
		ReadData.resize(TempData.size() * sizeof(uint16_t) / sizeof(uint32_t));
		memcpy(ReadData.data(), TempData.data(), ReadData.size() * sizeof(ReadData[0]));
	}
	return Ret;
}

ModbusMasterUDP::ExceptionCodeType ModbusMasterUDP::Write(uint8_t SlaveId, WriteType FunctionCode, uint16_t Address, uint16_t WriteData)
{
	uint8_t SendBuf[Bufsize];
	auto SendSize = CalculateSendLength(FunctionCode);
	if (SendSize > sizeof(SendBuf))
	{
		throw std::runtime_error("ModbusMasterUDP: CalculateSendLength greater than Bufsize");
	}
	MakeWriteOnePDU(SendBuf, SendSize, SlaveId, FunctionCode, Address, WriteData);

	UdpSocket_Inst.Send((char*)SendBuf, SendSize);

	uint8_t RecvBuf[Bufsize];
	auto RecvSize = UdpSocket_Inst.Recv((char*)RecvBuf, sizeof(RecvBuf));
	if (RecvSize <= 0)
	{
		throw std::runtime_error("ModbusMasterUDP: Recv error");
	}

	return ParseWritePDU(RecvBuf, RecvSize, (QueryPDU*)SendBuf);
}

Linx::ModbusMasterUDP::ExceptionCodeType ModbusMasterUDP::Write32(uint8_t SlaveId, WriteType FunctionCode, uint16_t Address, uint32_t WriteData)
{
	vector<uint16_t> TempData(2);
	TempData[0] = WriteData & 0xffff;
	TempData[1] = WriteData >> 16;
	return Write(SlaveId, WriteType::WriteMultiHoldingRegister, Address * sizeof(uint32_t) / sizeof(uint16_t), TempData);
	//auto Ret = Write(SlaveId, FunctionCode, Address * sizeof(uint32_t) / sizeof(uint16_t), WriteData & 0xffff);
	//if (EExceptionCode::Success != Ret)
	//{
	//	return Ret;
	//}
	//return Write(SlaveId, FunctionCode, Address * sizeof(uint32_t) / sizeof(uint16_t) + 1, WriteData >> 16);
}

ModbusMasterUDP::ExceptionCodeType ModbusMasterUDP::Write(uint8_t SlaveId, WriteType FunctionCode, uint16_t Address, const std::vector<uint16_t>& WriteData)
{
	uint8_t SendBuf[Bufsize];
	auto SendSize = CalculateSendLength(FunctionCode, WriteData.size());
	if (SendSize > sizeof(SendBuf))
	{
		throw std::runtime_error("ModbusMasterUDP: CalculateSendLength greater than Bufsize");
	}
	MakeWriteMultiPDU(SendBuf, SendSize, SlaveId, FunctionCode, Address, WriteData);

	UdpSocket_Inst.Send((char*)SendBuf, SendSize);

	uint8_t RecvBuf[Bufsize];
	auto RecvSize = UdpSocket_Inst.Recv((char*)RecvBuf, sizeof(RecvBuf));
	if (RecvSize <= 0)
	{
		throw std::runtime_error("ModbusMasterUDP: Recv error");
	}

	return ParseWritePDU(RecvBuf, RecvSize, (QueryPDU*)SendBuf);
}

Linx::ModbusMasterUDP::ExceptionCodeType ModbusMasterUDP::Write32(uint8_t SlaveId, WriteType FunctionCode, uint16_t Address, const std::vector<uint32_t>& WriteData)
{
	vector<uint16_t> TempData(WriteData.size() * sizeof(WriteData[0]) / sizeof(uint16_t));
	memcpy(TempData.data(), WriteData.data(), TempData.size() * TempData[0]);
	return Write(SlaveId, FunctionCode, Address, TempData);
}

void ModbusMasterUDP::MakePDU(uint8_t* Buf, size_t SendSize, uint8_t UnitId, ReadType FunctionCode, uint16_t Address, uint16_t DataLength)
{
	QueryPDU* pPDU = (QueryPDU*)Buf;
	pPDU->Head.SetTransactionId(TransactionId++);
	pPDU->Head.SetProtocolId(0);
	pPDU->Head.SetLength(SendSize - offsetof(MBAPHead, UnitId));
	pPDU->Head.UnitId = UnitId;
	pPDU->RFunctionCode = FunctionCode;
	pPDU->SetAddress(Address);
	pPDU->SetDataCount(DataLength);
}

void ModbusMasterUDP::MakeWriteOnePDU(uint8_t* Buf, size_t SendSize, uint8_t UnitId, WriteType FunctionCode, uint16_t Address, uint16_t Value)
{
	QueryPDU* pPDU = (QueryPDU*)Buf;
	pPDU->Head.SetTransactionId(TransactionId++);
	pPDU->Head.SetProtocolId(0);
	pPDU->Head.SetLength(SendSize - offsetof(MBAPHead, UnitId));
	pPDU->Head.UnitId = UnitId;
	pPDU->WFunctionCode = FunctionCode;
	pPDU->SetAddress(Address);
	pPDU->SetDataValue(Value);
}

void ModbusMasterUDP::MakeWriteMultiPDU(uint8_t* Buf, size_t SendSize, uint8_t UnitId, WriteType FunctionCode, uint16_t Address, const std::vector<uint16_t>& Data)
{
	QueryPDU_WriteMulti* pPDU = (QueryPDU_WriteMulti*)Buf;
	pPDU->Head.SetTransactionId(TransactionId++);
	pPDU->Head.SetProtocolId(0);
	pPDU->Head.SetLength(SendSize - offsetof(MBAPHead, UnitId));
	pPDU->Head.UnitId = UnitId;
	pPDU->WFunctionCode = FunctionCode;
	pPDU->SetAddress(Address);
	pPDU->SetDataCount(Data.size());
	pPDU->DataBytes = Data.size() * sizeof(uint16_t);

	ConvertEndianCopy(Data.data(), Data.size(), (uint16_t*)(Buf + sizeof(QueryPDU_WriteMulti)));
}

ModbusMasterUDP::ExceptionCodeType ModbusMasterUDP::ParseReadPDU(
	uint8_t* Buf,
	size_t RecvSize,
	const QueryPDU* QueryPDU,
	std::vector<uint16_t>& ReadData)
{
	ResponseReadPDU* pPDU = (ResponseReadPDU*)Buf;
	if (!pPDU->Check(*QueryPDU))
	{
		throw runtime_error("Check response false");
	}

	if (pPDU->RFunctionCode & 0x80)
	{
		return ((ResponseExceptionPDU*)pPDU)->ExceptionCode;
	}

	ReadData.resize(pPDU->DataBytes / sizeof(uint16_t));
	ConvertEndianCopy((uint16_t*)(Buf + sizeof(*pPDU)), ReadData.size(), ReadData.data());

	return EExceptionCode::Success;
}

ModbusMasterUDP::ExceptionCodeType ModbusMasterUDP::ParseWritePDU(uint8_t* Buf, size_t RecvSize, const QueryPDU* QueryPDU)
{
	ResponsePDU* pPDU = (ResponsePDU*)Buf;
	if (!pPDU->Check(*QueryPDU))
	{
		throw runtime_error("Check response false");
	}

	if (pPDU->RFunctionCode & 0x80)
	{
		return ((ResponseExceptionPDU*)pPDU)->ExceptionCode;
	}

	return EExceptionCode::Success;
}
