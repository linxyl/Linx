#pragma once

#include <iostream>
#include <chrono>

/** Print the variable name and its value. */
#define DEBUG_VAR(var) std::cout << #var" : " << var << endl;

/** Print the source filename and the line number. */
#define DEBUG_LINE std::cout << "Filename: " << __FILE__ << "  Line: " << __LINE__ << endl;

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

/** Same as DebugContainer, buf print in radix 16. */
template<class ContainerType>
void DebugContainer16(ContainerType& Container, size_t Cols = 8)
{
	size_t Count = 0;
	std::cout << std::hex;
	for (const auto& it: Container)
	{
		if (Count % Cols == 0)
		{
			std::cout << Count / Cols * Cols << " :\t";
		}

		std::cout << "0x" << it << '\t';

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
	std::cout << "Line " << std::dec << __LINE__ << " : "#Container << endl;\
	DebugContainer(Container, ##__VA_ARGS__);\
}

#define DebugContainer16Ex(Container, ...)\
{\
	std::cout << "Line " << std::dec << __LINE__ << " : "#Container << endl;\
	DebugContainer16(Container, ##__VA_ARGS__);\
}

template<typename T>
inline void DebugIterator(T ItBegin, T ItEnd)
{
	while (ItBegin != ItEnd)
	{
		std::cout << *ItBegin++ << '\t';
	}
	std::cout << std::endl;
}

/** Start the timer and use a name as the ID. */
#define DEBUG_TIME_BEGIN(Name) auto Name = std::chrono::high_resolution_clock::now();

/** End the timer of the specified name, use ResName to store the duration of the timer, in nanoseconds. */
#define DEBUG_TIME_END(Name, ResName)\
	auto Name##End765 = std::chrono::high_resolution_clock::now();\
	auto ResName = (std::chrono::duration_cast<std::chrono::nanoseconds> (Name##End765 - Name)).count();
