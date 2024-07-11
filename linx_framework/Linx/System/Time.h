#pragma once

#include <string>

namespace Linx
{
	/** Stores time information. */
	struct TimeInfo
	{
		int Year;
		int Month;
		int Day;
		int Hour;
		int Minute;
		int Second;
		long long MilliSecond;
	};

	/** Get current time. */
	TimeInfo GetTime();

	/** Gets total milliseconds. */
	long long GetTotalMilliSeconds();

	/**
	 * Get the string of current time.
	 * @param Format	Output format.	%Y=year, %m=month, %d=day, %H=hour, %M=minute, %S=second, %s=millisecond.
	 */
	std::string GetTimeString(std::string Format = "%Y-%m-%d %H:%M:%S");
}
