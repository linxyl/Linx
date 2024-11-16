#pragma once

#include <type_traits>
#include <algorithm>

#define PI	3.14159265358979323846

namespace Linx
{
	template<typename T1, typename T2, typename T3>
	constexpr std::common_type_t<T1, T2, T3> Clamp(T1 Val, T2 Min, T3 Max) noexcept
	{
		return std::max(Min, std::min(Val, Max));
	}

	constexpr float Lerp(float A, float B, float Alpha) noexcept
	{
		return A * (1 - Alpha) + B * Alpha;
	}

	/**
	 * Takes the smallest power of two greater or equal than the input number.
	 * For example, input 3 returns 4, input 10 returns 16, and input 32 returns 32.
	 */
	template<typename Type, typename Enable = std::enable_if_t<std::is_integral_v<Type>>>
	constexpr Type CeilToPowerOfTwo(Type n) noexcept
	{
		if (0 == n)
		{
			return 1;
		}

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
	constexpr size_t Rader(size_t Num, size_t Width) noexcept
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
