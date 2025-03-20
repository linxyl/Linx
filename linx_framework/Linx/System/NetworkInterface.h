#pragma once

#include <string>

namespace Linx
{
	struct NetworkInfo
	{
		std::string IP;
		std::string Netmask;
		std::string Gateway;
		std::string MAC;
	};

#ifdef _WIN32
	NetworkInfo GetNetworkInfo(const std::wstring& InterfaceName);

#else
	NetworkInfo GetNetworkInfo(const std::string& InterfaceName);
#endif

	bool ConfigNetwork(const std::string& InterfaceName, const char* IP, const char* Netmask = nullptr, const char* Gateway = nullptr);
}
