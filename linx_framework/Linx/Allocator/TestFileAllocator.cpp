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
	v.push_back('6');
	v.push_back('j');
	v.resize(16);

	getchar();
}

#endif