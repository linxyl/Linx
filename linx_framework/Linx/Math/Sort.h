#pragma once

#include "type_traits"

namespace Linx
{
	template<typename ItType>
	void InsertionSort(ItType InBegin, ItType InEnd)
	{
		auto Size = InEnd - InBegin;
		for (size_t i = 1; i < Size; i++)
		{
			auto key = InBegin[i];
			// Move elements of arr[0..i-1], that are greater than key,
			// to one position ahead of their current position
			size_t j = i - 1;
			for (; j >= 0 && InBegin[j] > key; j--)
			{
				InBegin[j + 1] = InBegin[j];
			}
			InBegin[j + 1] = key;
		}
	}

	template<typename ItType>
	void InsertionSort(const ItType InBegin, const ItType InEnd, ItType OutBegin)
	{

	}
}
