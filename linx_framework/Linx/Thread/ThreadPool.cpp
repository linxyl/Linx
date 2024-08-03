#include "ThreadPool.h"

using namespace std;
using namespace Linx;

ThreadPool::ThreadPool(int ThreadNum /*= std::thread::hardware_concurrency()*/)
	: Queue(ThreadNum),
	bIsRunning(false)
{
	assert(ThreadNum >= 0);

	if (ThreadNum > 0)
	{
		Start(ThreadNum);
	}
}

void ThreadPool::Start(int ThreadNum)
{
	Stop();

	std::lock_guard<decltype(Mutex)> locker(Mutex);
	bIsRunning = true;
	Queue.Continue();
	ThreadGroup.reserve(ThreadNum);
	for (int i = 0; i < ThreadNum; i++)
	{
		ThreadGroup.push_back(thread(&ThreadPool::RunInThread, this));
	}
}

void ThreadPool::Stop()
{
	std::lock_guard<decltype(Mutex)> locker(Mutex);
	bIsRunning = false;
	Queue.Stop();

	for (auto& t : ThreadGroup)
	{
		t.join();
	}

	ThreadGroup.clear();
}

void Linx::ThreadPool::RunInThread()
{
	while (bIsRunning)
	{
		auto pTask = Queue.Take();
		if (pTask)
		{
			pTask->Run();
		}
	}
}