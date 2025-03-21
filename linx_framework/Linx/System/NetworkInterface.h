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

	/** Returns the network information of the interface. */
#ifdef _WIN32
	NetworkInfo GetNetworkInfo(const std::wstring& InterfaceName);
#else
	NetworkInfo GetNetworkInfo(const std::string& InterfaceName);
#endif

	/**
	 * Configure network.
	 * @param InterfaceName	Name of network interface.
	 * @param IP			IP address.
	 * @param Netmask		Subnet mask, if is nullptr, it won't be configured.
	 * @param Gateway		Gateway, if is nullptr, it won't be configured.
	 */
	bool ConfigNetwork(const std::string& InterfaceName, const char* IP, const char* Netmask = nullptr, const char* Gateway = nullptr);
}
