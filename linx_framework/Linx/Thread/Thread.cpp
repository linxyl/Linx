#include "Thread.h"

using namespace std;

void Linx::Thread::Start()
{
	ThreadHandle = thread(&Thread::RunImpl, this);
}

bool Linx::Thread::SetAffinityByMask(MaskType Mask)
{
#ifdef _WIN32
    DWORD_PTR affinityMask = SetThreadAffinityMask(GetCurrentThread(), Mask);
    if (!affinityMask)
    {
        return false;
    }
#else
    return (0 == pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &Mask));
#endif
}

void Linx::Thread::RunImpl()
{
    SetAffinityByMask(Mask);
    Run();
}

Linx::Thread::MaskType Linx::Thread::MakeMask(uint32_t CPU)
{
#ifdef _WIN32
    return 1 << CPU;
#else
    MaskType cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(CPU, &cpuset);
    return cpuset;
#endif
}

Linx::Thread::MaskType Linx::Thread::MakeMask(const std::vector<uint32_t>& CPU)
{
#ifdef _WIN32
    MaskType Mask = 0;
    for (auto it : CPU)
    {
        Mask |= 1 << it;
    }
    return Mask;
#else
    MaskType cpuset;
    CPU_ZERO(&cpuset);
    for (auto it : CPU)
    {
        CPU_SET(it, &cpuset);
    }
    return cpuset;
#endif
}
