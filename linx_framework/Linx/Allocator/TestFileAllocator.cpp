#include "Linx/TestConfig.h"
#ifdef TEST_FILE_ALLOCATOR
#include "FileAllocator.h"
#include <vector>

using namespace std;
using namespace Linx;

int main()
{
	FileAllocator<char> a("test.txt");
	vector<char, FileAllocator<char>> v(a);
	int i = 0;
	while (1) {
		v.push_back('b');
	}
	v.push_back('c');
	v.push_back('7');

	getchar();
}

#endif