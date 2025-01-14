#include "SyncQueue.h"

using namespace std;

shared_ptr<Task> Linx::SyncQueue::Take()
{
	std::unique_lock<decltype(Mutex)> locker(Mutex);
	NotEmpty.wait(locker, [this] { return bNeedStop || !Queue.empty(); });
	if (bNeedStop)
	{
		return shared_ptr<Task>();
	}
	shared_ptr<Task> OutTask = Queue.front();
	Queue.pop_front();
	NotFull.notify_one();

	return OutTask;
}

void Linx::SyncQueue::Take(list<shared_ptr<Task>>& List)
{
	std::unique_lock<decltype(Mutex)> locker(Mutex);
	NotEmpty.wait(locker, [this] { return bNeedStop || !Queue.empty(); });
	if (bNeedStop)
	{
		return;
	}
	List = std::move(Queue);
	NotFull.notify_one();
}

void Linx::SyncQueue::Stop()
{
	{
		std::lock_guard<decltype(Mutex)> locker(Mutex);
		bNeedStop = true;
		Queue.clear();
	}
	NotFull.notify_all();
	NotEmpty.notify_all();
}

void Linx::SyncQueue::Continue()
{
	std::lock_guard<decltype(Mutex)> locker(Mutex);
	bNeedStop = false;
}

bool Linx::SyncQueue::IsEmpty()
{
	std::lock_guard<decltype(Mutex)> locker(Mutex);
	return Queue.empty();
}

bool Linx::SyncQueue::IsFull()
{
	std::lock_guard<decltype(Mutex)> locker(Mutex);
	return Queue.size() == MaxSize;
}

size_t Linx::SyncQueue::Size()
{
	std::lock_guard<decltype(Mutex)> locker(Mutex);
	return Queue.size();
}