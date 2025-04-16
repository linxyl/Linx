#pragma once

#include <string>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "Linx/Utils/Logger.h"
#include "Linx/Utils/Singleton.h"
#include "Linx/Config.h"
#include "Linx/Utils/Array.h"
#include "Linx/Utils/TypeTraits.h"

namespace Linx
{
	/** Initialize global variables */
	inline void LinxInit()
	{
		// Initialize Logger
	#if LOG_OPEN
		auto LogInst = Singleton<Logger>::Instance();
		LogInst->Open(LOG_FILE);
		LogInst->SetLogLevel(LOG_LEVEL);

	#if 0 != LOG_SPLIT_SECONDS
		LogInst->SetSplitBySeconds(LOG_SPLIT_SECONDS);
	#elif 0 != LOG_SPLIT_MINUTES
		LogInst->SetSplitByMinutes(LOG_SPLIT_MINUTES);
	#elif 0 != LOG_SPLIT_HOURS
		LogInst->SetSplitByHours(LOG_SPLIT_HOURS);
	#elif 0 != LOG_SPLIT_DAYS
		LogInst->SetSplitByDays(LOG_SPLIT_DAYS);
	#endif

	#if 0 != LOG_SPLIT_SIZE
		LogInst->SetSplitBySize(LOG_SPLIT_SIZE);
	#elif 0 != LOG_SPLIT_SIZE_K
		LogInst->SetSplitBySizeK(LOG_SPLIT_SIZE_K);
	#elif 0 != LOG_SPLIT_SIZE_M
		LogInst->SetSplitBySizeM(LOG_SPLIT_SIZE_M);
	#endif
	#endif
	}

	template<typename Type>
	constexpr Type ConvertEndian(Type Val) noexcept
	{
		if constexpr (1 == sizeof(Type))
		{
			return Val;
		}
		else if constexpr (2 == sizeof(Type))
		{
			return (Val >> 8) | (Val << 8);
		}
		else if constexpr (4 == sizeof(Type))
		{
			Val = ((Val >> 8) & 0xFF00FF) | ((Val << 8) & 0xFF00FF00);
			return (Val >> 16) | (Val << 16);
		}
		else if constexpr (8 == sizeof(Type))
		{
			Type Temp = 0;
			Temp |= (Val & 0xFF) << 56;
			Temp |= (Val & 0xFF00) << 40;
			Temp |= (Val & 0xFF0000) << 24;
			Temp |= (Val & 0xFF000000) << 8;
			Temp |= (Val & 0xFF00000000) >> 8;
			Temp |= (Val & 0xFF0000000000) >> 24;
			Temp |= (Val & 0xFF000000000000) >> 40;
			Temp |= (Val & 0xFF00000000000000) >> 56;
			return Temp;
		}
		else
		{
			static_assert(Linx::dependent_false_v<Type>, "Unsupported type");
		}

		return Type{};
	}

	template<typename ItSrcType, typename ItDstType>
	void ConvertEndianCopy(ItSrcType SrcBegin, ItSrcType SrcLast, ItDstType DstBegin)
	{
		while (SrcBegin != SrcLast)
		{
			*DstBegin++ = ConvertEndian(*SrcBegin++);
		}
	}

	template<typename ItSrcType, typename ItDstType>
	void ConvertEndianCopy(ItSrcType SrcBegin, size_t Length, ItDstType DstBegin)
	{
		for (size_t i = 0; i < Length; i++)
		{
			DstBegin[i] = ConvertEndian(SrcBegin[i]);
		}
	}

#ifdef _WIN32
	/** Sleep in seconds. */
	inline void SleepS(int Seconds) { ::Sleep(Seconds * 1000); }
	/** Sleep in millisecond. */
	inline void SleepM(int Milliseconds) { ::Sleep(Milliseconds); }
#else
	/** Sleep in seconds. */
	inline void SleepS(int Seconds) { ::sleep(Seconds); }
	/** Sleep in millisecond. */
	inline void SleepM(int Milliseconds) { usleep(Milliseconds * 1000); }
#endif

	/** Execute a system command and return the printed string. */
	std::string GetCmdStr(const std::string& Cmd);
}
