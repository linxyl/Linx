#pragma once

#include <type_traits>

#define PI	3.14159265358979323846

namespace Linx
{
	/**
	 * Takes the smallest power of two greater or equal than the input number.
	 * For example, input 3 returns 4, input 10 returns 16, and input 32 returns 32.
	 */
	template<typename Type, typename Enable = std::enable_if_t<std::is_integral_v<Type>>>
	constexpr Type NextHigherPowerOfTwo(Type n)
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

	/**
	 * Calculates the next inverted ordinal of a number. 
	 * For example, if the width is 3, the next inverted ordinal of 5(101) is 3(011).
	 * 
	 * @param Num	Target number.
	 * @param Width	Width of number.
	 */
	constexpr size_t Rader(size_t Num, size_t Width)
	{
		size_t k = Width >> 1;
		while (Num >= k)
		{
			Num -= k;
			k >>= 1;
		}
		if (Num < k)
			Num += k;
		return Num;
	}
}
