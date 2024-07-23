#include "Linx/TestConfig.h"
#ifdef TEST_FILE
#include "Linx/IO/File.h"
#include <thread>

using namespace std;
using namespace Linx;

File f("test_%Y-%m-%d_%H-%M-%S.txt");

int main()
{
	f.SetSplitBySeconds(3);
	char buf[4096];
	while (1)
	{
		f << 123 << endl;
		f.Write(buf, sizeof(buf));
	}
}

#endif
