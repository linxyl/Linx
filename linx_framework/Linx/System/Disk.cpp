#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/vfs.h>
#endif
#include <stdexcept>

#include "Disk.h"

using namespace Linx;

Linx::DiskInfo Linx::GetDiskInfo(const std::string& Path)
{
#ifdef _WIN32
    ULARGE_INTEGER freeBytesAvailableToCaller, totalNumberOfBytes, totalNumberOfFreeBytes;  
    if (!GetDiskFreeSpaceEx(
        Path.c_str(),
        &freeBytesAvailableToCaller,
        &totalNumberOfBytes,
        &totalNumberOfFreeBytes
    ))
    {
        throw std::logic_error("GetDiskInfo failed");
    }

    DiskInfo Info;
    Info.TotalSpace = totalNumberOfBytes.QuadPart;
    Info.AvailSpace = totalNumberOfFreeBytes.QuadPart;
    Info.UsedSpace = Info.TotalSpace - Info.AvailSpace;
    return Info;
#else
    DiskInfo Info;
    struct statfs64 disk_info;
    if (statfs64(Path.c_str(), &disk_info) == 0)
    {
        long long total = disk_info.f_blocks * disk_info.f_bsize;
        long long avail = disk_info.f_bavail * disk_info.f_bsize;
        Info.TotalSpace = total;
        Info.AvailSpace = avail;
        Info.UsedSpace = Info.TotalSpace - Info.AvailSpace;
    }
    else
    {
        throw std::logic_error("GetDiskInfo failed");
    }
    return Info;
#endif
}
