#include "TestConfig.h"
#ifdef RING_BUFFER
#include <vector>
#include <iostream>
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

	cout << "RB1:\n";
	for (const auto it: RB1)
	{
		cout << it << ' ';
	}
	cout << endl;

	// Copy RB1 to RB2.
	RingBuffer<short> RB2;
	RB2 = RB1;

	cout << "RB2:\n";
	for (const auto it: RB2)
	{
		cout << it << ' ';
	}
	cout << endl;

	// Test const operator.
	RB2[1] = 4;
	RB2[2] = 6;

	const RingBuffer<short> RB3(4);
	//RB3[3] = 2; // Not allowed.
	short a = RB3[3]; // Call const function.
}

void TestReadWrite()
{
	cout << "\nTestReadWrite\n";

	RingBuffer<short> RB1(4);
	RingBuffer<short> RB2(6);

	// Write arr1 to RB1, and read from RB1 to arr2.
	short arr1[6] = { 3, 7, 5 };
	short arr2[6] = { 0 };
	RB1.Write(arr1, 3);
	RB1.Read(arr2, 4);

	cout << "arr2:\n";
	for (const auto it: arr2)
	{
		cout << it << ' ';
	}
	cout << endl;

	// Write arr2 to RB1.
	RB1.Write(arr2, 3);
	// Write RB1 to RB2.
	RB2.Write(RB1, 4);

	// Modify RB2.
	RB2[0] = 6;
	RB2[1] = 7;
	RB2[2] = 8;
	// Read from RB2 to RB1.
	RB2.Read(RB1, 3);

	cout << "RB1:\n";
	for (const auto it: RB1)
	{
		cout << it << ' ';
	}
	cout << endl;
}
#endif
