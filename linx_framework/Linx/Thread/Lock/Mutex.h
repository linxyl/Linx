#pragma once

#include <mutex>
#include <condition_variable>

namespace Linx
{
	class MutexLock : public std::mutex
	{
		using Super = std::mutex;
	
	public:
		inline void Lock() { Super::lock(); }
	};
}
