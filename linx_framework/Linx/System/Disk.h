#pragma once

#include <stdint.h>
#include <string>

namespace Linx
{
	/** Information of the disk */
	struct DiskInfo
	{
		uint64_t TotalSpace;
		uint64_t UsedSpace;
		uint64_t AvailSpace;
	};

	/** Returns the information of the disk within the path. */
	DiskInfo GetDiskInfo(const std::string& Path);
};
