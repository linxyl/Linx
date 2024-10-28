#pragma once

#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <errno.h>
#endif

namespace Linx
{
	class IOBase
	{
	public:
		virtual ~IOBase() {};

#ifndef _WIN32
		using HANDLE = int;
		static constexpr int INVALID_HANDLE_VALUE = -1;
#endif

		virtual size_t Write(const void* Buf, size_t Size)
		{
			if (bShouldPrint)
			{
				std::cout.write((const char*)Buf, Size);
			}
			return Size;
		}

		/** Returns the handle. */
		inline HANDLE GetHandle() const noexcept { return Handle; }

		/** Returns the code of the last error. */
		inline int GetLastError() const noexcept
		{
#ifdef _WIN32
			return ::GetLastError();
#else
			return errno;
#endif
		}

	protected:
		HANDLE Handle = INVALID_HANDLE_VALUE;

	private:
		bool bShouldPrint = false;
	};
}