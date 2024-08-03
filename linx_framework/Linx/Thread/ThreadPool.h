#pragma once

#include <cassert>
#include <thread>
#include <vector>

#include "SyncQueue.h"
#include "Linx/Thread/Thread.h"

namespace Linx
{
	/**
	 * A thread-safe thread pool.
	 * It can add tasks to the task queue.
	 * When a thread is idle, it takes a task from the task queue and execute. 
	 */
	class ThreadPool
	{
	public:
		/**
		 * Default constructor.
		 * @param ThreadNum	Number of threads to be started. If greater than 0, start all threads.
		 */
		ThreadPool(int ThreadNum = std::thread::hardware_concurrency());

		~ThreadPool() { Stop(); }

		/** Start the specified number of threads. */
		void Start(int ThreadNum);

		/** Stop all thread. It blocks and waits for the thread to finish executing. */
		void Stop();

		/** Adds a task to the task queue. */
		template<typename TaskType>
		void AddTask(TaskType&& InTask) { Queue.Put(std::forward<TaskType>(InTask)); }

	private:
		/** Functions executed by each thread. */
		void RunInThread();

		/** Store all threads */
		std::vector<std::thread> ThreadGroup;

		/** Synchronization task queue */
		SyncQueue Queue;

		std::mutex Mutex;

		bool bIsRunning;
	};
}