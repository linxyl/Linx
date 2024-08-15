#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace Linx
{
	class HandleWrapper
	{
	public:
#ifndef _WIN32
		using HANDLE = int;
#endif
		inline HandleWrapper(HANDLE InHandle) noexcept : Handle(InHandle) {}
		inline HandleWrapper(HandleWrapper&& other) noexcept : Handle(other.Handle)
		{
			other.Handle = (HANDLE)- 1;
		}
		~HandleWrapper() { Close(); }

		HandleWrapper& operator=(HandleWrapper&& other) noexcept {
			if (this != &other)
			{
				Close();
				Handle = other.Handle;
				other.Handle = (HANDLE)-1;
			}
			return *this;
		}

		HandleWrapper(const HandleWrapper&) = delete;
		HandleWrapper& operator=(const HandleWrapper&) = delete;

		// 提供一个方法来获取文件描述符（如果需要）
		inline HANDLE Get() const noexcept { return Handle; }

		inline void Close() noexcept
		{
#ifdef _WIN32
			CloseHandle(Handle);
#else
			close(Handle);
#endif
		}

	private:
		HANDLE Handle;
	};
}
