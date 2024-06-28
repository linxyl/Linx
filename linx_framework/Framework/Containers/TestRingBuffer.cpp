//#define OPEN_TEST
#ifdef OPEN_TEST
#include <vector>
#include "RingBuffer.h"

using namespace std;
using namespace linx;

void TestConstruct();
void TestOperator();
void TestReadWrite();

int main()
{
	TestConstruct();
	TestOperator();
	TestReadWrite();
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
	RingBuffer<short> RB1(4);
	short arr[3]{ 0,1,2 };
	RB1.Write(arr, 3);

	RingBuffer<short> RB2;
	RB2 = RB1;

	RB2[1] = 4;
	RB2[2] = 6;

	const RingBuffer<short> RB3;
	//RB3[3] = 2; // Not allowed.
	short a = RB3[3]; // Call const function.
}

void TestReadWrite()
{
	RingBuffer<short> RB1(4);
	RingBuffer<short> RB2(6);

	short arr1[6] = { 0,1,2 };
	short arr2[6] = { 0 };
	RB1.Write(arr1, 3);
	RB1.Read(arr2, 4);

	RB1.Write(arr2, 3);
	RB2.Write(RB1, 4);

	RB2[0] = 6;
	RB2[1] = 7;
	RB2[2] = 8;
	RB2.Read(RB1, 3);
}
#endif
