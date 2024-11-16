#ifdef _WIN32
#include <windows.h>
#include <stdexcept>
#else
#include <fstream>
#include <string>
#endif
#include <exception>

#include "MemInfo.h"

using namespace Linx;
using namespace std;

/** Returns the information of the current memory. */
MemInfo Linx::GetMemInfo()
{
#ifdef _WIN32
	MEMORYSTATUSEX statex;  
	statex.dwLength = sizeof(statex);  

	if (!GlobalMemoryStatusEx(&statex))
	{
		throw std::logic_error("GetMemInfo failed");
	}

	MemInfo Info;
	Info.TotalPhys = statex.ullTotalPhys;
	Info.AvailPhys = statex.ullAvailPhys;
	Info.TotalPageFile = statex.ullTotalPageFile;
	Info.AvailPageFile = statex.ullAvailPageFile;
	Info.TotalVirtual = statex.ullTotalVirtual;
	Info.AvailVirtual = statex.ullAvailVirtual;
	Info.MemoryUsageRate = statex.dwMemoryLoad;

	return Info;
#else
	std::ifstream file("/proc/meminfo");
	std::string line;

	auto f = [&](const std::string& key){
		std::string key_str = key + ":";
		const auto key_len = key_str.length();
		size_t PosBegin, PosEnd;

		while (std::getline(file, line))
		{
			if (line.substr(0, key_len) == key_str)
			{
				PosBegin = line.find_first_not_of(' ', key_len);
				PosEnd = line.find_first_of(' ', PosBegin);
				if (PosEnd != std::string::npos)
				{
					return line.substr(PosBegin, PosEnd - PosBegin);
				}
			}
		}
		throw std::logic_error("GetMemInfo failed");
	};

	MemInfo Info;
	Info.TotalPhys = 1024 * std::stoul(f("MemTotal"));
	Info.AvailPhys = 1024 * std::stoul(f("MemAvailable"));
	Info.TotalPageFile = 1024 * (Info.TotalPhys + std::stoul(f("SwapTotal")));
	Info.AvailPageFile = 1024 * (Info.AvailPhys + std::stoul(f("SwapFree")));
	Info.TotalVirtual = 1024 * std::stoul(f("VmallocTotal"));
	Info.AvailVirtual = 1024 * (Info.TotalVirtual - std::stoul(f("VmallocUsed")));
	Info.MemoryUsageRate = 100.0 - (float)Info.AvailPhys / Info.TotalPhys * 100.0;

	return Info;
#endif
}
