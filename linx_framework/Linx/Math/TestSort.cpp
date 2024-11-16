#include "Linx/TestConfig.h"
#ifdef TEST_SORT
#include <vector>
#include <list>

#include "Sort.h"
#include "Linx/Utils/Debug.h"
#include "Linx/Math/Random.h"

#define TEST_SORT_FUNC(Func)\
{\
	cout<<#Func<<':'<<endl;\
	vector<int> v1{2, 4, 3, 8, 41, 6, 4};\
	vector<int> v2{1, 2, 3, 4, 5, 6, 7};\
	auto v3 = v;\
	cout<<"Origin ";\
	DebugContainerEx(v1);\
	Func(v1.begin(), v1.end());\
	cout<<"Sorted ";\
	DebugContainerEx(v1);\
	cout<<"Origin ";\
	\
	/** Use pred to sort in descending order */\
	DebugContainerEx(v2);\
	Func(v2.begin(), v2.end(), greater<>{});\
	cout<<"Sorted ";\
	DebugContainerEx(v2);\
	\
	cout << "Begin timing" << endl;\
	DEBUG_TIME_BEGIN(Beg);\
	Func(v3.begin(), v3.end());\
	DEBUG_TIME_END(Beg, t);\
	cout << t << endl;\
	cout<<endl;\
}

using namespace std;
using namespace Linx;

vector<int> v(1024 * 4);

int main()
{
	int Size = v.size();
	RandomSequence(0, Size, v.begin(), v.end());

	TEST_SORT_FUNC(InsertionSort);
	TEST_SORT_FUNC(MergeSort);
	TEST_SORT_FUNC(HeapSort);
	TEST_SORT_FUNC(QuickSort);

	getchar();
}

#endif