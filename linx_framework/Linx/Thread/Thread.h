#pragma once

#include <stdint.h>
#include <thread>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#else
#endif

namespace Linx
{
	/**
	 * A thread function wrapper class.
	 * You need to override the Run function
	 * as your custom thread function.
	 * Call Start function to start the thread.
	 */
	class Thread
	{
	public:
#ifdef _WIN32
		using MaskType = DWORD_PTR;
#else
		using MaskType = cpu_set_t;
#endif

	public:
		inline Thread() noexcept = default;
		inline Thread(uint32_t CPU) noexcept { Mask = MakeMask(CPU); }
		inline Thread(const std::vector<uint32_t>& CPU) noexcept { Mask = MakeMask(CPU); }
		virtual ~Thread() = default;

	protected:
		/** Implementation of the thread. */
		virtual void Run() = 0;

	public:
		/** Start run the thread */
		void Start();
		virtual void Stop() {};

		inline bool IsJoinable() const noexcept { return ThreadHandle.joinable(); }
		inline void Join() { ThreadHandle.join(); }
		inline void Detach() { ThreadHandle.detach(); }

		static inline bool SetAffinity(uint32_t CPU)
		{
			return SetAffinityByMask(MakeMask(CPU));
		}
		static inline bool SetAffinity(const std::vector<uint32_t>& CPU)
		{
			return SetAffinityByMask(MakeMask(CPU));
		}

		inline auto GetHandle() const noexcept { return ThreadHandle.get_id(); }

	private:
		void RunImpl();

		static bool SetAffinityByMask(MaskType Mask);

		static MaskType MakeMask(uint32_t CPU);
		static MaskType MakeMask(const std::vector<uint32_t>& CPU);

		std::thread ThreadHandle;

		MaskType Mask{};
	};
}