#include "Linx/TestConfig.h"
#ifdef TEST_MODBUS_UDP
#include <thread>
#include <iostream>
#include <array>

#include "Linx/Utils/Debug.h"
#include "ModbusSlaveUDP.h"
#include "ModbusMasterUDP.h"
#include "ModbusLibrary.h"

using namespace std;
using namespace Linx;
using namespace Modbus;

#define MASTER_COUT cout << "Master : "
#define SLAVE_COUT cout << "Slave : "

#define TARGET_ADDR	"127.0.0.1"
#define PORT 502

void ThreadMMU()
{
	ModbusMasterUDP MMU;
	MMU.SetTargetAddr(TARGET_ADDR, PORT);

	cout << "FunctionCode Table:\n"
		"1\tReadCoil\n"
		"2\tReadDiscreteInput\n"
		"3\tReadHoldingRegister\n"
		"4\tReadInputRegister\n"
		"5\tWriteCoil\n"
		"6\tWriteHoldingRegister\n"
		"15\tWriteMultiCoilRegister\n"
		"16\tWriteMultiHoldingRegister\n";

	while (true)
	{
		cout << endl;

		// Slave Id
		cout << "Enter SlaveId : ";
		int SlaveId;
		cin >> SlaveId;

		// Read or write
		cout << "Enter read or write(r/w) : ";
		char RW;
		cin >> RW;
		if (RW != 'r' && RW != 'w')
		{
			MASTER_COUT << "Invalid input, please enter r or w" << endl;
			continue;
		}

		// Function code
		cout << "Enter FunctionCode : ";
		int FunctionCodeTemp;
		cin >> FunctionCodeTemp;

		// Address
		cout << "Enter Address : ";
		uint16_t Address;
		cin >> Address;

		try
		{
			if ('r' == RW)
			{
				// Length
				cout << "Enter Length : ";
				uint16_t Length;
				cin >> Length;

				// Send Query
				EFunctionCode::ReadType FunctionCode = (EFunctionCode::ReadType)FunctionCodeTemp;
				switch (FunctionCode)
				{
				case EFunctionCode::ReadCoil:
				case EFunctionCode::ReadDiscreteInput:
				case EFunctionCode::ReadHoldingRegister:
				case EFunctionCode::ReadInputRegister:
				default:
				{
					vector<uint16_t> ReadData;
					auto Ret = MMU.Read(SlaveId, FunctionCode, Address, Length, ReadData);
					if (EExceptionCode::Success != Ret)
					{
						MASTER_COUT << "Exception : " << ToString(Ret) << endl;
					}
					else
					{
						DebugContainerEx(ReadData);
					}
					break;
				}
				}
			}
			else
			{
				EFunctionCode::WriteType FunctionCode = (EFunctionCode::WriteType)FunctionCodeTemp;
				switch (FunctionCode)
				{
				case EFunctionCode::WriteCoil:
				case EFunctionCode::WriteHoldingRegister:
				{
					cout << "Enter Data : ";
					uint16_t Data;
					cin >> Data;

					auto Ret = MMU.Write(SlaveId, FunctionCode, Address, Data);
					if (EExceptionCode::Success != Ret)
					{
						MASTER_COUT << "Exception : " << ToString(Ret) << endl;
					}
					break;
				}
				case EFunctionCode::WriteMultiCoil:
				case EFunctionCode::WriteMultiHoldingRegister:
				{
					// Length
					cout << "Enter Length : ";
					uint16_t Length;
					cin >> Length;

					// Write data
					cout << "Enter Data : ";
					std::vector<uint16_t> WriteData;
					for (size_t i = 0; i < Length; i++)
					{
						uint16_t Val;
						cin >> Val;
						WriteData.emplace_back(Val);
					}

					// Send Query
					auto Ret = MMU.Write(SlaveId, FunctionCode, Address, WriteData);
					if (EExceptionCode::Success != Ret)
					{
						MASTER_COUT << "Exception : " << ToString(Ret) << endl;
					}
					break;
				}
				}
			}
		}
		catch (const std::exception& e)
		{
			MASTER_COUT << e.what() << endl;
			continue;
		}
	}
}

void ThreadMSU()
{
	ModbusSlaveUDP MSU;
	if (!MSU.Bind())
	{
		cout << "Bind failed" << endl;
		return;
	}

	try
	{
		array<uint16_t, 16> Arr1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
		array<uint16_t, 16> Arr2{100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115};
		ModbusSlaveDevice Dev1(1);
		Dev1.AddRegisterGroup(ERegisterType::HoldingRegister, Arr1.data(), 0, Arr1.size());
		Dev1.AddRegisterGroup(ERegisterType::InputRegister, Arr2.data(), 0, Arr2.size());
		MSU.AddDevice(Dev1);
	}
	catch (const exception& e)
	{
		SLAVE_COUT << e.what() << endl;
	}

	while (1)
	{
		try
		{
			MSU.ResponseQuery();
		}
		catch (const exception& e)
		{
			SLAVE_COUT << e.what() << endl;
		}
	}
}

int main()
{
	thread tMMU(ThreadMMU);
	thread tMSU(ThreadMSU);

	tMSU.join();
	tMMU.join();

	return 0;
}
#endif