#pragma once

#include <functional>

namespace Linx
{
	/************************************************************************/
	/*							Comparison Sort                             */
	/************************************************************************/

	/************************** Insertion Sort ******************************/

	template<typename ItType, typename PredType>
	void InsertionSort(ItType InFirst, ItType InLast, PredType Pred)
	{
		auto Size = std::distance(InFirst, InLast);
		for (long i = 1; i < Size; i++)
		{
			auto key = InFirst[i];
			long j = i - 1;
			for (; j >= 0 && Pred(key, InFirst[j]); --j)
			{
				InFirst[j + 1] = InFirst[j];
			}
			InFirst[j + 1] = key;
		}
	}

	template<typename ItType>
	inline void InsertionSort(ItType InFirst, ItType InLast)
	{
		InsertionSort(InFirst, InLast, std::less<>{});
	}

	/***************************** Merge Sort *******************************/

	/** Merge two sorted sequences. */
	template<typename ItType, typename PredType>
	inline void Merge(ItType First, ItType Mid, ItType Last, PredType Pred)
	{
		auto length1 = std::distance(First, Mid);
		auto length2 = std::distance(Mid, Last);

		std::vector<typename std::iterator_traits<ItType>::value_type> Temp(length1 + length2);

		auto it1 = First;
		auto it2 = Mid;
		auto it = Temp.begin();

		while (it1 != Mid && it2 != Last)
		{
			if (Pred(*it1, *it2))
			{
				*it = *it1;
				++it1;
			}
			else
			{
				*it = *it2;
				++it2;
			}
			++it;
		}

		std::copy(it1, Mid, it);
		std::copy(it2, Last, it);

		std::copy(Temp.begin(), Temp.end(), First);
	}

	template<typename ItType, typename PredType>
	void MergeSort(ItType InFirst, ItType InLast, PredType Pred)
	{
		auto Distance = std::distance(InFirst, InLast);
		if (Distance <= 1)
			return;

		auto Mid = InFirst;
		std::advance(Mid, Distance / 2);

		MergeSort(InFirst, Mid, Pred);     // Sort the InFirst half.
		MergeSort(Mid, InLast, Pred);      // Sort the second half.
		Merge(InFirst, Mid, InLast, Pred);   // Merge two parts.
	}

	template<typename ItType>
	inline void MergeSort(ItType InFirst, ItType InLast)
	{
		MergeSort(InFirst, InLast, std::less<>{});
	}

	/****************************** Heap Sort *******************************/

	template<typename ItType, typename PredType>
	void Heapify(ItType InFirst, ItType InLast, int n, int i, PredType Pred)
	{
		int largest = i; // 将当前节点设为最大值
		int left = 2 * i + 1; // 左子节点
		int right = 2 * i + 2; // 右子节点

		// 如果左子节点大于当前最大值
		if (left < n && !Pred(InFirst[left], InFirst[largest]))
			largest = left;

		// 如果右子节点大于当前最大值
		if (right < n && !Pred(InFirst[right], InFirst[largest]))
			largest = right;

		// 如果最大值不是根节点
		if (largest != i)
		{
			std::swap(InFirst[i], InFirst[largest]); // 交换
			Heapify(InFirst, InLast, n, largest, Pred); // 递归堆化受影响的子树
		}
	}

	template<typename ItType, typename PredType>
	void HeapSort(ItType InFirst, ItType InLast, PredType Pred)
	{
		auto n = std::distance(InFirst, InLast);

		// 构建最大堆
		for (int i = n / 2 - 1; i >= 0; i--)
			Heapify(InFirst, InLast, n, i, Pred);

		// 一个接一个地从堆中取出元素
		for (auto i = n - 1; i > 0; i--)
		{
			// 将当前根（最大值）移到数组末尾
			std::swap(*InFirst, InFirst[i]);

			// 调用 Heapify 来将剩余的堆重新调整为最大堆
			Heapify(InFirst, InLast, i, 0, Pred);
		}
	}

	template<typename ItType>
	inline void HeapSort(ItType InFirst, ItType InLast)
	{
		HeapSort(InFirst, InLast, std::less<>{});
	}

	/****************************** Quick Sort *******************************/

	template<typename ItType, typename PredType>
	ItType Partition(ItType InFirst, ItType InLast, PredType Pred)
	{
		auto pivot = *std::prev(InLast); // 选择最后一个元素作为基准
		auto i = InFirst;

		for (auto j = InFirst; j != std::prev(InLast); ++j)
		{
			if (Pred(*j, pivot))
			{
				std::iter_swap(i, j);
				++i;
			}
		}
		std::iter_swap(i, std::prev(InLast)); // 将基准放到正确的位置
		return i;
	}

	template<typename ItType, typename PredType>
	void QuickSort(ItType InFirst, ItType InLast, PredType Pred)
	{
		if (InFirst < InLast)
		{
			auto pivot_it = Partition(InFirst, InLast, Pred);
			QuickSort(InFirst, pivot_it, Pred);
			QuickSort(std::next(pivot_it), InLast, Pred);
		}
	}

	template<typename ItType>
	inline void QuickSort(ItType InFirst, ItType InLast)
	{
		QuickSort(InFirst, InLast, std::less<>{});
	}
}
