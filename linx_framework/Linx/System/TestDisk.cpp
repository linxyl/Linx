#include "Linx/TestConfig.h"
#ifdef TEST_DISK
#include "Disk.h"
#include "Linx/Utils/Debug.h"

using namespace std;
using namespace Linx;

int main()
{
    DiskInfo Info = GetDiskInfo("/");
    DEBUG_VAR(Info.TotalSpace)
    DEBUG_VAR(Info.UsedSpace)
    DEBUG_VAR(Info.AvailSpace)

    getchar();
  
    return 0; 
}

#endif
