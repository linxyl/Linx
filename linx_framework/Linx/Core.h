#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

//#include "Config.h"
#include "Utils/Logger.h"

namespace Linx
{
	void LinxInit();

#ifdef _WIN32
	inline void Sleep(int Milliseconds) { ::Sleep(Milliseconds); }
#else
	inline void Sleep(int Milliseconds) { usleep(Milliseconds * 1000); }
#endif
}
