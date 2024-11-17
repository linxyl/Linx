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
    FILE* pipe = _popen(Cmd.c_str(), "r");
	if (!pipe)
	{
		throw std::runtime_error("popen failed!");
	}

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
	{
        result += buffer;
    }
    _pclose(pipe);

    return result;
}
