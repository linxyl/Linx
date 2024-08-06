#include "Linx/TestConfig.h"
#ifdef TEST_FILE_STREAM
#include "Linx/IO/FileStream.h"

using namespace std;
using namespace Linx;

FileStream f("test_%Y-%m-%d_%H-%M-%S.txt");

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
