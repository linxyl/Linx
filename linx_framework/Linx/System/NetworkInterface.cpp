#include "NetworkInterface.h"
#include <stdexcept>

#ifdef _WIN32
#include <comdef.h>
#include <Wbemidl.h>

#include "Linx/FunctionLibrary/StringLibrary.h"

#pragma comment(lib, "wbemuuid.lib")
#else
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>

#include <fstream>
#include <sstream>

#include "Linx/IO/Network/UdpSocket.h"
#endif

using namespace std;
using namespace Linx;

#ifdef _WIN32
Linx::NetworkInfo Linx::GetNetworkInfo(const std::wstring& InterfaceName)
{

	NetworkInfo Info;
    HRESULT hres;

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres))
    {
        throw logic_error("Failed to initialize COM library");
    }

    hres = CoInitializeSecurity(
        nullptr,
        -1,
        nullptr,
        nullptr,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        nullptr,
        EOAC_NONE,
        nullptr
    );
    if (FAILED(hres))
    {
        CoUninitialize();
        throw logic_error("Failed to initialize security");
    }

    IWbemLocator* pLoc = nullptr;
    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        (LPVOID*)&pLoc
    );
    if (FAILED(hres))
    {
        CoUninitialize();
        throw logic_error("Failed to initialize security");
    }

    IWbemServices* pSvc = nullptr;
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        nullptr,
        nullptr,
        0,
        0,
        0,
        0,
        &pSvc
    );
    if (FAILED(hres))
    {
        pLoc->Release();
        CoUninitialize();
        throw logic_error("Failed to connect to WMI");
    }

    hres = CoSetProxyBlanket(
        pSvc,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        nullptr,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        nullptr,
        EOAC_NONE
    );
    if (FAILED(hres))
    {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        throw logic_error("Failed to set proxy blanket");
    }

    IEnumWbemClassObject* pEnumerator = nullptr;
    std::wstring query = L"SELECT * FROM Win32_NetworkAdapter WHERE NetConnectionID = '" + InterfaceName + L"'";
    hres = pSvc->ExecQuery(
        _bstr_t("WQL"),
        _bstr_t(query.c_str()),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        nullptr,
        &pEnumerator
    );
    if (FAILED(hres))
    {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        throw logic_error("Failed to execute WMI query");
    }

    IWbemClassObject* pclsObj = nullptr;
    ULONG uReturn = 0;
    while (pEnumerator)
    {
        hres = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (uReturn == 0) break;

        VARIANT vtIndex;
        hres = pclsObj->Get(L"Index", 0, &vtIndex, 0, 0);
        if (SUCCEEDED(hres))
        {
            DWORD adapterIndex = vtIndex.uintVal;

            IEnumWbemClassObject* pConfigEnumerator = nullptr;
            std::wstring configQuery = L"SELECT * FROM Win32_NetworkAdapterConfiguration WHERE Index = " + std::to_wstring(adapterIndex);
            hres = pSvc->ExecQuery(
                _bstr_t("WQL"),
                _bstr_t(configQuery.c_str()),
                WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                nullptr,
                &pConfigEnumerator
            );
            if (SUCCEEDED(hres))
            {
                IWbemClassObject* pConfigObj = nullptr;
                ULONG configReturn = 0;
                while (pConfigEnumerator)
                {
                    hres = pConfigEnumerator->Next(WBEM_INFINITE, 1, &pConfigObj, &configReturn);
                    if (configReturn == 0) break;

                    // Get IP address
                    VARIANT vtIP;
                    hres = pConfigObj->Get(L"IPAddress", 0, &vtIP, 0, 0);
                    if (SUCCEEDED(hres) && vtIP.vt == (VT_ARRAY | VT_BSTR))
                    {
                        SAFEARRAY* sa = vtIP.parray;
                        BSTR* ipArray = nullptr;
                        SafeArrayAccessData(sa, (void**)&ipArray);
                        Info.IP = WcharToString(ipArray[0]);
                        SafeArrayUnaccessData(sa);
                    }
                    VariantClear(&vtIP);

                    // Get Netmask
                    VARIANT vtSubnet;
                    hres = pConfigObj->Get(L"IPSubnet", 0, &vtSubnet, 0, 0);
                    if (SUCCEEDED(hres) && vtSubnet.vt == (VT_ARRAY | VT_BSTR))
                    {
                        SAFEARRAY* sa = vtSubnet.parray;
                        BSTR* subnetArray = nullptr;
                        SafeArrayAccessData(sa, (void**)&subnetArray);
                        Info.Netmask = WcharToString(subnetArray[0]);
                        SafeArrayUnaccessData(sa);
                    }
                    VariantClear(&vtSubnet);

                    // Get default gateway
                    VARIANT vtGateway;
                    hres = pConfigObj->Get(L"DefaultIPGateway", 0, &vtGateway, 0, 0);
                    if (SUCCEEDED(hres) && vtGateway.vt == (VT_ARRAY | VT_BSTR))
                    {
                        SAFEARRAY* sa = vtGateway.parray;
                        BSTR* gatewayArray = nullptr;
                        SafeArrayAccessData(sa, (void**)&gatewayArray);
                        Info.Gateway = WcharToString(gatewayArray[0]);
                        SafeArrayUnaccessData(sa);
                    }
                    VariantClear(&vtGateway);

                    // Get MAC
                    VARIANT vtMAC;
                    hres = pConfigObj->Get(L"MACAddress", 0, &vtMAC, 0, 0);
                    if (SUCCEEDED(hres))
                    {
                        Info.MAC = WcharToString(vtMAC.bstrVal);
                    }
                    VariantClear(&vtMAC);

                    pConfigObj->Release();
                }
                pConfigEnumerator->Release();
            }
        }
        VariantClear(&vtIndex);
        pclsObj->Release();
    }

    pSvc->Release();
    pLoc->Release();
    CoUninitialize();

    return Info;
}

bool Linx::ConfigNetwork(const std::string& InterfaceName, const char* IP, const char* Netmask, const char* Gateway)
{
    string cmd = "netsh interface ip set address name = \""s + InterfaceName + "\" static ";
    cmd += IP;
    if (Netmask)
    {
        cmd += " "s + Netmask;
    }
    if (Gateway)
    {
        cmd += " "s + Gateway;
    }

    return 0 == system(cmd.c_str());
}

#else
Linx::NetworkInfo Linx::GetNetworkInfo(const std::string& InterfaceName)
{
    NetworkInfo Info;
    UdpSocket us;

    struct ifreq ifr { 0 };
    strncpy(ifr.ifr_name, InterfaceName.c_str(), IFNAMSIZ - 1);

    // Get IP
    if (ioctl(us.GetSocket(), SIOCGIFADDR, &ifr) == 0)
    {
        struct sockaddr_in* ipaddr = (struct sockaddr_in*)&ifr.ifr_addr;
        Info.IP = inet_ntoa(ipaddr->sin_addr);
    }

    // Get Netmask
    if (ioctl(us.GetSocket(), SIOCGIFNETMASK, &ifr) == 0)
    {
        struct sockaddr_in* Netmask = (struct sockaddr_in*)&ifr.ifr_netmask;
        Info.Netmask = inet_ntoa(Netmask->sin_addr);
    }

    // Get Gateway
    std::ifstream routeFile("/proc/net/route");
    std::string line;
    while (std::getline(routeFile, line))
    {
        std::istringstream iss(line);
        std::string iface, flags, refcnt, use, metric, mtu, window, irtt;
        uint32_t destination, gatewayStr;

        // Skip the header line and read the columns
        if (!(iss >> iface >> std::hex >> destination >> gatewayStr >> flags >> refcnt >> use >> metric >> mtu >> window >> irtt))
        {
            continue; // Skip malformed lines
        }

        // Check if the destination is the default route (0.0.0.0)
        if (iface == InterfaceName && destination == 0) // 0.0.0.0 indicates default route
        {
            struct in_addr gatewayAddr{gatewayStr};
            Info.Gateway = inet_ntoa(gatewayAddr); // Convert to dotted decimal notation
            break;
        }
    }

    strncpy(ifr.ifr_name, InterfaceName.c_str(), IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';

    if (ioctl(us.GetSocket(), SIOCGIFHWADDR, &ifr) == 0)
    {
		unsigned char* mac = (unsigned char*)ifr.ifr_hwaddr.sa_data;
		char macAddress[18];
		std::snprintf(macAddress, sizeof(macAddress), "%02x:%02x:%02x:%02x:%02x:%02x",
					  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		Info.MAC = macAddress;
    }

    return Info;
}
#include<iostream>
bool Linx::ConfigNetwork(const std::string& InterfaceName, const char* IP, const char* Netmask, const char* Gateway)
{
	UdpSocket us;

    struct ifreq ifr;
    strncpy(ifr.ifr_name, InterfaceName.c_str(), IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';

    // Close interface
    if (ioctl(us.GetSocket(), SIOCGIFFLAGS, &ifr) == -1)
    {
        perror("ioctl(SIOCGIFFLAGS)");
        return false;
    }

    ifr.ifr_flags &= ~IFF_UP;
    if (ioctl(us.GetSocket(), SIOCSIFFLAGS, &ifr) == -1)
    {
        perror("ioctl(SIOCSIFFLAGS)");
        return false;
    }

    ifr.ifr_flags |= IFF_UP; // Set the interface up
    if (ioctl(us.GetSocket(), SIOCSIFFLAGS, &ifr) == -1)
    {
        perror("ioctl SIOCSIFFLAGS");
        return false;
    }

    // Set IP address
    struct sockaddr_in* addr = reinterpret_cast<struct sockaddr_in*>(&ifr.ifr_addr);
    addr->sin_family = AF_INET;
    if (inet_pton(AF_INET, IP, &addr->sin_addr) <= 0)
    {
        perror("inet_pton");
		return false;
    }

    if (ioctl(us.GetSocket(), SIOCSIFADDR, &ifr) == -1)
    {
        perror("ioctl SIOCSIFADDR");
		return false;
    }

    // Set Netmask
    if(Netmask)
    {
		struct sockaddr_in* netmask_addr = reinterpret_cast<struct sockaddr_in*>(&ifr.ifr_netmask);
		netmask_addr->sin_family = AF_INET;
		if (inet_pton(AF_INET, Netmask, &netmask_addr->sin_addr) <= 0)
		{
			perror("inet_pton netmask");
			return false;
		}

		if (ioctl(us.GetSocket(), SIOCSIFNETMASK, &ifr) == -1)
		{
			perror("ioctl SIOCSIFNETMASK");
			return false;
		}
    }

    // Set Gateway
    if(Gateway)
    {
		std::string command = "ip route add default via "s + Gateway + " dev " + InterfaceName;
		if(0 != std::system(command.c_str()))
		{
			return false;
		}
    }

    return true;
}
#endif
