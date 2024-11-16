#include "Linx/TestConfig.h"
#ifdef TEST_FILE
#include "File.h"

using namespace std;
using namespace Linx;

int main()
{
	File f("test.txt");
	char buf[4096] = "abcd";
	for (int i = 0; i < 1024; i++)
	{
		f.Write(buf, sizeof(buf));
	}

	char* p = f.MemMap(1024);
	p[1] = '2';

	f.Close();
}

#endif
