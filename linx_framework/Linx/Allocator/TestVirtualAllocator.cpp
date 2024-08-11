#include "Linx/TestConfig.h"
#ifdef TEST_VIRTUAL_ALLOCATOR
#include "VirtualAllocator.h"
#include <vector>
#include <stdio.h>

using namespace Linx;
using namespace std;

int main()
{
	vector<int, VirtualAllocator<int>> v;
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	getchar();
}

#endif
