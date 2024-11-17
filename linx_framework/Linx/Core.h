#pragma once

#include <string>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace Linx
{
	/** Initialize global variables */
	void LinxInit();

#ifdef _WIN32
	/** Sleep in seconds. */
	inline void SleepS(int Seconds) { ::Sleep(Seconds * 1000); }
	/** Sleep in millisecond. */
	inline void SleepM(int Milliseconds) { ::Sleep(Milliseconds); }
#else
	/** Sleep in seconds. */
	inline void SleepS(int Seconds) { ::sleep(Seconds); }
	/** Sleep in millisecond. */
	inline void SleepM(int Milliseconds) { usleep(Milliseconds * 1000); }
#endif

	/** Execute a system command and return the printed string. */
	std::string ExecCmd(const std::string& Cmd);
}
