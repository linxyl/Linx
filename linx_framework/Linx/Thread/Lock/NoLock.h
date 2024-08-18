#pragma once

namespace Linx
{
	class NoLock
	{
	public:
		NoLock() = default;
		NoLock(const NoLock&) = delete;
		NoLock& operator=(const NoLock&) = delete;

		constexpr void lock() noexcept {};
		constexpr void unlock() noexcept {};
	};
}