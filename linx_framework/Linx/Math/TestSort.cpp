#include "Linx/TestConfig.h"
#ifdef TEST_SORT
#include <vector>
#include <list>

#include "Sort.h"
#include "Linx/Utils/Debug.h"

using namespace std;
using namespace Linx;

int main()
{
	vector<int> v{2, 4, 3, 8, 41, 6, 4};
	DebugContainerEx(v);
	InsertionSort(v.begin(), v.end());
	DebugContainerEx(v);

	getchar();
}

#endif