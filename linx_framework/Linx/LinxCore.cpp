#include "LinxCore.h"
#include "Linx/Utils/Logger.h"
#include "Linx/Utils/Singleton.h"

void Linx::Init()
{
	auto LogInst = Singleton<Logger>::Instance();
	LogInst->Open("test");
	LogInst->SetLogLevel(ELogLevel::LevelWarn);
	LogInst->SetSplitBySeconds(3);
	//LogInst->SetSplitBySizeK(300);
}
