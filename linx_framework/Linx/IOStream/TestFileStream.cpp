#include "Linx/TestConfig.h"
#ifdef TEST_FILE_STREAM
#include "FileStream.h"
#include <thread>

using namespace std;
using namespace Linx;

FileStream f("test_%Y-%m-%d_%H-%M-%S.txt");

void f1()
{
	char buf[4096];
	while (1)
	{
		f << 123 << endl;
		f.Write(buf, sizeof(buf));
	}
}

void f2()
{
	char buf[4096];
	while (1)
	{
		f << 123 << endl;
		f.Write(buf, sizeof(buf));
	}
}

int main()
{
	thread t1(f1);
	thread t2(f2);

	t2.join();
	t1.join();
}

#endif
