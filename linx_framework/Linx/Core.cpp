#include <stdio.h>
#include <stdexcept>

#include "Core.h"
#include "Linx/Utils/Logger.h"
#include "Linx/Utils/Singleton.h"

void Linx::LinxInit()
{
	// Initialize Logger
#if LOG_OPEN
	auto LogInst = Singleton<Logger>::Instance();
	LogInst->Open(LOG_FILE);
	LogInst->SetLogLevel(LOG_LEVEL);

#if 0 != LOG_SPLIT_SECONDS
	LogInst->SetSplitBySeconds(LOG_SPLIT_SECONDS);
#elif 0 != LOG_SPLIT_MINUTES
	LogInst->SetSplitByMinutes(LOG_SPLIT_MINUTES);
#elif 0 != LOG_SPLIT_HOURS
	LogInst->SetSplitByHours(LOG_SPLIT_HOURS);
#elif 0 != LOG_SPLIT_DAYS
	LogInst->SetSplitByDays(LOG_SPLIT_DAYS);
#endif

#if 0 != LOG_SPLIT_SIZE
	LogInst->SetSplitBySize(LOG_SPLIT_SIZE);
#elif 0 != LOG_SPLIT_SIZE_K
	LogInst->SetSplitBySizeK(LOG_SPLIT_SIZE_K);
#elif 0 != LOG_SPLIT_SIZE_M
	LogInst->SetSplitBySizeM(LOG_SPLIT_SIZE_M);
#endif
#endif
}

std::string Linx::ExecCmd(const std::string& Cmd)
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
