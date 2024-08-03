#pragma once

#include <thread>

namespace Linx
{
	class Thread
	{
	public:
		Thread() = default;
		virtual ~Thread() = default;

	public:
		virtual void Run() {};

		void Start();
		virtual void Stop() {};

		inline bool joinable() const noexcept { return ThreadHandle.joinable(); }
		inline void Join() { ThreadHandle.join(); }
		inline void Detach() { ThreadHandle.detach(); }

	private:
		std::thread ThreadHandle;
	};
}