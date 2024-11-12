#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace Linx
{
	void LinxInit();

#ifdef _WIN32
	inline void SleepS(int Seconds) { ::Sleep(Seconds * 1000); }
	inline void SleepM(int Milliseconds) { ::Sleep(Milliseconds); }
#else
	inline void SleepS(int Seconds) { ::sleep(Seconds); }
	inline void SleepM(int Milliseconds) { usleep(Milliseconds * 1000); }
#endif
}
