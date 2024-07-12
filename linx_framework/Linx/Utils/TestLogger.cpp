#include "Linx/TestConfig.h"
#ifdef TEST_LOGGER
#include "Logger.h"
#include <iostream>
#include <thread>

using namespace std;
using namespace Linx;

void f1()
{
	while (1)
	{
		LOG_ERROR << 11111111 << endl;
	}
}
void f2()
{
	while (1)
	{
		LOG_FATAL << 22222222 << endl;
	}
}

int main()
{
	auto Log = Singleton<Logger>::Instance();
	Log->Open("test");
	Log->SetLogLevel(ELogLevel::LevelWarn);
	Log->SetSplitBySeconds(3);
	//Log->SetSplitBySizeK(300);

#define TEST_LOGGER_THREAD
#ifdef TEST_LOGGER_THREAD
	thread t1(f1);
	thread t2(f2);

	t2.join();
	t1.join();
#else
	while (1)
	{
		LOG_DEBUG << "Test content : " << 1234 << endl;
		LOG_INFO << 432423 << ",Other" << endl;
		LOG_WARN << "hello" << endl;
		LOG_ERROR << 33333333 << endl;
		LOG_FATAL << "Critical" << endl;
	}
#endif

	getchar();
}
#endif
