#pragma once

#include <mutex>

namespace Linx
{
	class MutexLock : public std::mutex
	{
		using Super = std::mutex;
	
	public:
		inline void Lock() { Super::lock(); }
	};
}
