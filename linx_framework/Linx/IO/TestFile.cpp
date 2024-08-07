#include "Linx/TestConfig.h"
#ifdef TEST_FILE
#include "File.h"

using namespace std;
using namespace Linx;

int main()
{
	File f("test.txt", EFileFlag::EWrite | EFileFlag::ECreate | EFileFlag::EOpen);
	char buf[4096];
	while (1)
	{
		auto a = f.Write(buf, sizeof(buf));
	}
	f.Close();
}

#endif
