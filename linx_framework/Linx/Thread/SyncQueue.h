#pragma once

#include <list>
#include <mutex>
#include <thread>
#include <memory>
#include <condition_variable>

#include "Task.h"

namespace Linx
{
	class SyncQueue
	{
	public:
		SyncQueue(size_t InMaxSize) :MaxSize(InMaxSize), bNeedStop(false)
		{}

	public:
		template<typename TaskType, typename = std::enable_if_t<std::is_base_of_v<Task, TaskType>>>
		void Put(const TaskType& InTask)
		{
			Add(InTask);
		}
		std::shared_ptr<Task> Take();
		void Take(std::list<std::shared_ptr<Task>>& List);
		void Stop();
		void Continue();

	public:
		bool IsEmpty();
		bool IsFull();
		size_t Size();

	private:
		template<typename TaskType>
		void Add(const TaskType& InTask);

	private:
		std::list<std::shared_ptr<Task>> Queue;
		std::mutex Mutex;
		std::condition_variable NotEmpty;
		std::condition_variable NotFull;
		size_t MaxSize;
		bool bNeedStop;
	};


	template<typename TaskType>
	void Linx::SyncQueue::Add(const TaskType& InTask)
	{
		std::unique_lock<decltype(Mutex)>locker(Mutex);
		NotFull.wait(locker, [this] {return bNeedStop || (Queue.size() < MaxSize); });
		if (bNeedStop)
		{
			return;
		}
		Queue.emplace_back(std::make_shared<TaskType>(InTask));
		NotEmpty.notify_one();
	}
}