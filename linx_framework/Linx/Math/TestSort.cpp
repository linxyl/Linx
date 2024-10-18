#include "Linx/TestConfig.h"
#ifdef TEST_SORT
#include <vector>
#include <list>

#include "Sort.h"
#include "Linx/Utils/Debug.h"

#define TEST_SORT_FUNC(Func)\
{\
	cout<<#Func<<':'<<endl;\
	vector<int> v1{2, 4, 3, 8, 41, 6, 4};\
	vector<int> v2{1, 2, 3, 4, 5, 6, 7};\
	cout<<"Origin ";\
	DebugContainerEx(v1);\
	Func(v1.begin(), v1.end());\
	cout<<"Sorted ";\
	DebugContainerEx(v1);\
	cout<<"Origin ";\
	DebugContainerEx(v2);\
	Func(v2.begin(), v2.end(), greater<>{});\
	cout<<"Sorted ";\
	DebugContainerEx(v2);\
	cout<<endl;\
}

using namespace std;
using namespace Linx;

int main()
{
	TEST_SORT_FUNC(InsertionSort);
	TEST_SORT_FUNC(MergeSort);
	TEST_SORT_FUNC(HeapSort);

	getchar();
}

#endif