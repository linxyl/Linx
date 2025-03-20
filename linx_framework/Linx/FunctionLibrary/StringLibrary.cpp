#include <algorithm>
#include "StringLibrary.h"

#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

std::string& Linx::Replace(std::string& Target, const std::string& Old, const std::string& New)
{
    size_t Pos = 0;
    while ((Pos = Target.find(Old, Pos)) != std::string::npos)
    {
        Target.replace(Pos, Old.length(), New);
        Pos += New.length();
    }
    return Target;
}

std::string& Linx::Trim(std::string& Str)
{
	if (Str.empty())
	{
		return Str;
	}
	Str.erase(0, Str.find_first_not_of(" \t\r\n"));
	Str.erase(Str.find_last_not_of(" \t\r\n") + 1);
	return Str;
}

std::string& Linx::TrimAll(std::string& Str)
{
	auto end_it = std::remove_if(Str.begin(), Str.end(), ::isspace);
	Str = string(Str.begin(), end_it);
	return Str;
}