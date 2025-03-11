#pragma once

#include <stdint.h>

namespace Linx
{
	template<class Type = uint8_t>
	struct Array
	{
	public:
		constexpr Array(const Type* InPtr, size_t InSize)
		{
			Ptr = InPtr;
			Size = InSize;
		}

		template <size_t ArraySize>
		constexpr Array(Type(&InArray)[ArraySize])
		{
			Ptr = InArray;
			Size = ArraySize;
		}

	public:
		/** Pointer of array */
		Type* Ptr;

		/** Size of array */
		size_t Size;
	};
}