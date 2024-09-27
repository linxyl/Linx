#pragma once

#include <stdint.h>

namespace Linx
{
	/** Information of the memory */
	struct MemInfo
	{
		/** Memory percentage usage rate */
		float MemoryUsageRate;

		/** The amount of actual physical memory, in bytes */
		uint64_t TotalPhys;

		/** The amount of physical memory currently available, in bytes */
		uint64_t AvailPhys;

		/** The current committed memory limit for the system or the current process */
		uint64_t TotalPageFile;

		/** The maximum amount of memory the current process can commit, in bytes */
		uint64_t AvailPageFile;

		/** The size of the user-mode portion of the virtual address space of the calling process, in bytes */
		uint64_t TotalVirtual;

		/** The amount of available memory currently of the virtual address space of the calling process, in bytes */
		uint64_t AvailVirtual;
	};

	/** Returns the information of the current memory. */
	MemInfo GetMemInfo();
}
