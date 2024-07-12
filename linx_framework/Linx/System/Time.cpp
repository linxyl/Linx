#include "Time.h"

#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

#include "Linx/FunctionLibrary/StringLibrary.h"

using namespace Linx;
using namespace std;

Linx::TimeInfo Linx::GetTime()
{
	auto now = std::chrono::system_clock::now();  
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);  
  
	std::tm *ltm = std::localtime(&now_c);  
  
	TimeInfo Time;
	Time.Year = 1900 + ltm->tm_year;
	Time.Month = 1 + ltm->tm_mon;
	Time.Day = ltm->tm_mday;
	Time.Hour = ltm->tm_hour;
	Time.Minute = ltm->tm_min;
	Time.Second = ltm->tm_sec;
  
	auto duration = now.time_since_epoch();  
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();  

	Time.MilliSecond = milliseconds % 1000;

	return Time;
}

long long Linx::GetTotalMilliSeconds()
{
	auto now = std::chrono::system_clock::now();  
	auto duration = now.time_since_epoch();  
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();  

	return milliseconds;
}

std::string Linx::GetTimeString(std::string Format)
{
	auto Time = GetTime();

    size_t Pos = 0;
	char tmp[8];
    if ((Pos = Format.find("%Y", 0)) != std::string::npos)
    {
		sprintf(tmp, "%04u", Time.Year);
        Format.replace(Pos, sizeof("%Y") - 1, tmp);
    }
    if ((Pos = Format.find("%m", 0)) != std::string::npos)
    {
		sprintf(tmp, "%02u", Time.Month);
        Format.replace(Pos, sizeof("%m") - 1, tmp);
    }
    if ((Pos = Format.find("%d", 0)) != std::string::npos)
    {
		sprintf(tmp, "%02u", Time.Day);
        Format.replace(Pos, sizeof("%d") - 1, tmp);
    }
	if ((Pos = Format.find("%H", 0)) != std::string::npos)
    {
		sprintf(tmp, "%02u", Time.Hour);
        Format.replace(Pos, sizeof("%H") - 1, tmp);
    }
	if ((Pos = Format.find("%M", 0)) != std::string::npos)
    {
		sprintf(tmp, "%02u", Time.Minute);
        Format.replace(Pos, sizeof("%M") - 1, tmp);
    }
	if ((Pos = Format.find("%S", 0)) != std::string::npos)
    {
		sprintf(tmp, "%02u", Time.Second);
        Format.replace(Pos, sizeof("%H") - 1, tmp);
    }
	if ((Pos = Format.find("%s", 0)) != std::string::npos)
    {
		sprintf(tmp, "%03lld", Time.MilliSecond);
        Format.replace(Pos, sizeof("%s") - 1, tmp);
    }

	return Format;
}
