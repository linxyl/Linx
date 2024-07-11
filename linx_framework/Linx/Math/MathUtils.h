#pragma once

#include <type_traits>

namespace Linx
{
	/**
	 * Takes the smallest power of two greater or equal than the input number.
	 * For example, input 3 returns 4, input 10 returns 16, and input 32 returns 32.
	 */
	template<typename Type, typename Enable = std::enable_if_t<std::is_integral_v<Type>>>
	Type NextHigherPowerOfTwo(Type n)
	{
		if ((n & (n - 1)) == 0)
			return n;

		n -= 1;
		n |= n >> 1;
		n |= n >> 2;
		n |= n >> 4;
		if constexpr (sizeof(n) >= 2) { n |= n >> 8; }
		if constexpr (sizeof(n) >= 4) { n |= n >> 16; }
		if constexpr (sizeof(n) >= 8) { n |= n >> 32; }

		return n + 1;
	}
}
