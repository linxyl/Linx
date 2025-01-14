#include "Linx/TestConfig.h"
#ifdef TEST_RING_BUFFER
#include <iostream>
#include <thread>

#include "RingBuffer.h"
#include "Linx/Utils/Debug.h"

using namespace std;
using namespace Linx;

void TestConstruct();
void TestOperator();
void TestReadWrite();
void TestThreadReadWrite();

void ThreadRead0(RingBuffer<int>& RB);
void ThreadWrite0(RingBuffer<int>& RB);
void ThreadRead1(RingBuffer<int>& RB);
void ThreadWrite1(RingBuffer<int>& RB);
void ThreadRead2(RingBuffer<int>& RB);
void ThreadWrite2(RingBuffer<int>& RB);

#define THREAD_LINE_NUM		16U
#define THREAD_BLOCK_SIZE	32U
#define THREAD_TEST_SIZE	1000000000U

int main()
{
	TestConstruct();
	TestOperator();
	TestReadWrite();
	TestThreadReadWrite();

	getchar();
}

void TestConstruct()
{
	RingBuffer<short> RB1;
	RingBuffer<short> RB2(4);
	RingBuffer<short> RB3(RB2);
	RingBuffer<short> RB4 = std::move(RingBuffer<short>(4));
}

void TestOperator()
{
	cout << "\nTestOperator\n";

	// Write array to the RingBuffer.
	RingBuffer<short> RB1(4);
	short arr[3]{ 0,1,2 };
	RB1.Write(arr, 3);

	DebugContainerEx(RB1);

	// Copy RB1 to RB2.
	RingBuffer<short> RB2;
	RB2 = RB1;

	DebugContainerEx(RB2);

	// Test const operator.
	RB2[1] = 4;
	RB2[2] = 6;

	const RingBuffer<short> RB3(4);
	//RB3[3] = 2; // Not allowed.
	short a = RB3[3]; // Call const function.
	(void)a;
}

void TestReadWrite()
{
	cout << "\nTestReadWrite\n";

	RingBuffer<short> RB1(4);
	RB1.ReadMode = ERingBufferReadMode::ReadAll;
	RingBuffer<short> RB2(6);

	// Write arr1 to RB1, and read from RB1 to arr2.
	short arr1[6] = { 3, 7, 5 };
	short arr2[6] = { 0 };
	RB1.Write(arr1, 3);
	RB1.Read(arr2, 4);

	DebugContainerEx(arr2);

	// Write arr2 to RB1.
	RB1.Write(arr2, 3);

	DebugContainerEx(RB1);

	// Write RB1 to RB2.
	RB2.Write(RB1, 4);

	DebugContainerEx(RB2);

	// Modify RB2.
	RB2[0] = 6;
	RB2[1] = 7;
	RB2[2] = 8;
	// Read from RB2 to RB1.
	RB2.Read(RB1, 3);

	DebugContainerEx(RB1);
}

void TestThreadReadWrite()
{
	cout << "\nTestThreadReadWrite\n";

	RingBuffer<int> RB(128);

	cout << "Test0:\n";
	thread TR(ThreadRead0, ref(RB));
	thread TW(ThreadWrite0, ref(RB));

	TW.join();
	TR.join();

	cout << "Test1:\n";
	RB.ReadMode = ERingBufferReadMode::ReadAll;
	RB.WriteMode = ERingBufferWriteMode::WriteFill;
	TR = thread(ThreadRead1, ref(RB));
	TW = thread(ThreadWrite1, ref(RB));

	TW.join();
	TR.join();

	cout << "Test2:\n";
	TR = thread(ThreadRead2, ref(RB));
	TW = thread(ThreadWrite2, ref(RB));

	TW.join();
	TR.join();
}

void ThreadRead0(RingBuffer<int>& RB)
{
	size_t ErrNum = 0;
	size_t ReadSize = 0;
	while (true)
	{
		auto Size = THREAD_BLOCK_SIZE;
		std::remove_reference_t<decltype(RB)>::value_type Buf[THREAD_BLOCK_SIZE];
		ReadSize += RB.Read(Buf, Size);
		for (int i = 0; i < Size; i++)
		{
			static int LastIndex = 0;
			if (Buf[i] - LastIndex != 1)
			{
				ErrNum++;
			}
			LastIndex = Buf[i];
		}

		if (ReadSize >= THREAD_TEST_SIZE)
		{
			break;
		}
	}

	cout << "Error number: " << ErrNum << endl;
}

void ThreadWrite0(RingBuffer<int>& RB)
{
	size_t WrittenSize = 0;
	while (true)
	{
		auto Size = THREAD_BLOCK_SIZE;
		std::remove_reference_t<decltype(RB)>::value_type Buf[THREAD_BLOCK_SIZE];
		for (int i = 0; i < Size; i++)
		{
			static int index = 1;
			Buf[i] = index++;
		}

		WrittenSize += RB.Write(Buf, Size);
		if (WrittenSize >= THREAD_TEST_SIZE)
		{
			break;
		}
	}
}

void ThreadRead1(RingBuffer<int>& RB)
{
	size_t ErrNum = 0;
	size_t ReadSize = 0;
	while (true)
	{
		unsigned int DataLen = RB.GetDataLen();
		auto Size = std::min(DataLen, THREAD_BLOCK_SIZE);
		std::remove_reference_t<decltype(RB)>::value_type Buf[THREAD_BLOCK_SIZE];
		ReadSize += RB.Read(Buf, Size);
		for (int i = 0; i < Size; i++)
		{
			static int LastIndex = 0;
			if (Buf[i] - LastIndex != 1)
			{
				ErrNum++;
			}
			LastIndex = Buf[i];
		}

		if (ReadSize >= THREAD_TEST_SIZE)
		{
			break;
		}
	}

	cout << "Error number: " << ErrNum << endl;
}

void ThreadWrite1(RingBuffer<int>& RB)
{
	size_t WrittenSize = 0;
	while (true)
	{
		unsigned int RemainLen = RB.GetRemainLen();
		auto Size = std::min(RemainLen, THREAD_BLOCK_SIZE);
		std::remove_reference_t<decltype(RB)>::value_type Buf[THREAD_BLOCK_SIZE];
		for (int i = 0; i < Size; i++)
		{
			static int index = 1;
			Buf[i] = index++;
		}

		WrittenSize += RB.Write(Buf, Size);
		if (WrittenSize >= THREAD_TEST_SIZE)
		{
			break;
		}
	}
}

void ThreadRead2(RingBuffer<int>& RB)
{
	size_t ErrNum = 0;
	size_t ReadSize = 0;
	while (true)
	{
		ReadSize += RB.Read([&](int* Ptr, size_t Size) {
			for (int i = 0; i < Size; i++)
			{
				static int LastIndex = 0;
				if (Ptr[i] - LastIndex != 1)
				{
					ErrNum++;
				}
				LastIndex = Ptr[i];
			}
		}, THREAD_BLOCK_SIZE);

		if (ReadSize >= THREAD_TEST_SIZE)
		{
			break;
		}
	}

	cout << "Error number: " << ErrNum << endl;
}

void ThreadWrite2(RingBuffer<int>& RB)
{
	size_t WrittenSize = 0;
	while (true)
	{
		WrittenSize += RB.Write([](int* Ptr, size_t Size) {
			for (int i = 0; i < Size; i++)
			{
				static int index = 1;
				Ptr[i] = index++;
			}
		}, THREAD_BLOCK_SIZE);

		if (WrittenSize >= THREAD_TEST_SIZE)
		{
			break;
		}
	}
}
#endif
