#include "Linx/TestConfig.h"
#ifdef TEST_ARRAY
#include "Array.h"
#include <stdio.h>

using namespace std;
using namespace Linx;

int main()
{
	char buf[8];
	Array<char>aa(buf);
	getchar();
}

#endif