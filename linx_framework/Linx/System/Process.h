#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace Linx
{
	inline int GetPID()
	{
#ifdef _WIN32
		return GetCurrentProcessId();
#else
		return getpid();
#endif
	}
}
