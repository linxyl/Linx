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
		/** Implementation of the thread. */
		virtual void Run() = 0;

	public:
		/** Start run the thread */
		void Start();
		virtual void Stop() {};

		inline bool IsJoinable() const noexcept { return ThreadHandle.joinable(); }
		inline void Join() { ThreadHandle.join(); }
		inline void Detach() { ThreadHandle.detach(); }

	private:
		std::thread ThreadHandle;
	};
}