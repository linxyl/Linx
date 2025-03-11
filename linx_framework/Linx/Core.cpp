#include <stdio.h>
#include <stdexcept>

#include "Core.h"

std::string Linx::GetCmdStr(const std::string& Cmd)
{
    char buffer[1024];
    std::string result;
#ifdef _WIN32
    FILE* pipe = _popen(Cmd.c_str(), "r");
#else
    FILE* pipe = popen(Cmd.c_str(), "r");
#endif
	if (!pipe)
	{
		throw std::runtime_error("popen failed!");
	}

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
	{
        result += buffer;
    }
#ifdef _WIN32
    _pclose(pipe);
#else
	pclose(pipe);
#endif

    return result;
}
