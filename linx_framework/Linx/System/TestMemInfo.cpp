#include "Linx/TestConfig.h"
#ifdef TEST_MEM_INFO
#include "MemInfo.h"
#include "Linx/Utils/Debug.h"

using namespace std;
using namespace Linx;

int main()
{
    auto Info = GetMemInfo();

    DEBUG_VAR(Info.MemoryUsageRate)
    DEBUG_VAR(Info.TotalPhys)
    DEBUG_VAR(Info.AvailPhys)
    DEBUG_VAR(Info.TotalPageFile)
    DEBUG_VAR(Info.AvailPageFile)
    DEBUG_VAR(Info.TotalVirtual)
    DEBUG_VAR(Info.AvailVirtual)

    getchar();
  
    return 0; 
}

#endif
