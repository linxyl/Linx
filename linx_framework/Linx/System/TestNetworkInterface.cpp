#include "Linx/TestConfig.h"
#ifdef TEST_NETWORK_INTERFACE
#include "NetworkInterface.h"
#include <iostream>

using namespace std;
using namespace Linx;

int main()
{
#ifdef _WIN32
	auto ret = ConfigNetwork("ÒÔÌ«Íø","192.168.31.99","255.255.255.0", "192.168.31.1");
	auto info = GetNetworkInfo(L"ÒÔÌ«Íø");
#else
	auto ret = ConfigNetwork("ens33","192.168.31.30","255.255.255.0", "192.168.31.1");
	auto info = GetNetworkInfo("ens33");
#endif

	cout << ret << endl;
	cout << info.IP << endl;
	cout << info.Netmask << endl;
	cout << info.Gateway << endl;
	cout << info.MAC << endl;

	getchar();
}

#endif
