#pragma once

#include <unistd.h>

#include "Config.h"

namespace Linx
{
	void LinxInit();

#ifndef _WIN32
	inline void Sleep(int n) { usleep(n * 1000); }
#endif
}
