#include "Linx/TestConfig.h"
#ifdef LOGGER
#include "Logger.h"
#include <iostream>

using namespace std;
using namespace Linx;

int main()
{
	auto Log = Singleton<Logger>::Instance();
	Log->Open("test");
	Log->SetLogLevel(ELogLevel::LevelWarning);

	Log->SetSplitBySeconds(3);
	//Log->SetSplitBySizeK(300);

	while (1)
	{
		Debug << "Test content : " << 1234 << endl;
		Info << 432423 << ",Other" << endl;
		Warning << "hello" << endl;
		Error << 33333333 << endl;
		Fatal << "Critical" << endl;
	}

	getchar();
}
#endif
