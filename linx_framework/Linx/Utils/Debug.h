#pragma once

#include <type_traits>
#include <iostream>

/**
 * Output the value of each element in the container.
 * @param Cols		How many elements are output per line.
 */
template<class ContainerType>
void DebugContainer(ContainerType& Container, size_t Cols = 8)
{
	size_t Count = 0;
	for (const auto& it: Container)
	{
		if (Count % Cols == 0)
		{
			std::cout << Count / Cols * Cols << " :\t";
		}

		std::cout << it << '\t';

		if (++Count % Cols == 0)
		{
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}

/** Same as DebugContainer, buf print container name. */
#define DebugContainerEx(Container, ...)\
{\
	std::cout << #Container":" << endl;\
	DebugContainer(Container, ##__VA_ARGS__);\
}
