#pragma once

#include <random>
#include <type_traits>

namespace Linx
{
	/** Gets a random number in [RangeFirst, RangeLast]. */
	template<typename Type>
	inline Type Random(Type RangeFirst, Type RangeLast)
	{
		std::mt19937 gen(std::random_device{}()); // 以 rd() 作为种子的 Mersenne Twister 引擎  
		using DisType = std::conditional_t<
			is_floating_point_v<Type>,
			std::uniform_real_distribution<>,
			std::uniform_int_distribution<>>;
	  
		return DisType(RangeFirst, RangeLast)(gen);  
	}

	/** Gets a random number in [0, RangeLast]. */
	template<typename Type>
	inline Type Random(Type Range)
	{
		return Random((Type)0, Range);  
	}

	/** Gets a random number sequence in [RangeFirst, RangeLast]. */
	template<typename Type, typename ItType>
	void RandomSequence(Type RangeFirst, Type RangeLast, ItType OutFirst, ItType OutLast)
	{
		std::mt19937 gen(std::random_device{}()); // 以 rd() 作为种子的 Mersenne Twister 引擎  
		using DisType = std::conditional_t<
			is_floating_point_v<Type>,
			std::uniform_real_distribution<>,
			std::uniform_int_distribution<>>;
		DisType Dis(RangeFirst, RangeLast);
		
		for (; OutFirst != OutLast; OutFirst++)
		{
			*OutFirst = Dis(gen);
		}
	}
	
	/** Gets a random number sequence in [0, RangeLast]. */
	template<typename Type, typename ItType>
	inline void RandomSequence(Type RangeLast, ItType OutFirst, ItType OutLast)
	{
		RandomSequence((Type)0, RangeLast, OutFirst, OutFirst, OutLast);
	}
}
