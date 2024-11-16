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
