#pragma once

#include <string>
#include <memory>
#ifdef _WIN32
#include <windows.h>
#endif

namespace Linx
{
	struct HandleDeleter {
		void operator()(HANDLE h) const noexcept {
			if (h != INVALID_HANDLE_VALUE && h != nullptr) {
				CloseHandle(h);
			}
		}
	};

	template<typename Type>
	class FileAllocator
	{
	public:
		using value_type = Type;
		using pointer = Type*;
		using const_pointer = const Type*;
		using reference = Type&;
		using const_reference = const Type&;
		using size_type = size_t;
		using difference_type = std::ptrdiff_t;

	public:
		constexpr FileAllocator() noexcept = default;
		constexpr FileAllocator(const std::string& InFilename) noexcept { Open(InFilename); }
		template <class Other>
		constexpr FileAllocator(const FileAllocator<Other>& InAllocator) noexcept
		{
			hFile = InAllocator.GetFile();
			hMapFile = InAllocator.GetMapFile();
		}

		inline Type* allocate(const size_t Count)
		{
			++AllocCount;

			SetFilePointer(reinterpret_cast<HANDLE>(hFile.get()), Count, NULL, FILE_BEGIN);
			SetEndOfFile(reinterpret_cast<HANDLE>(hFile.get())); 

			CloseHandle(hMapFile);
			hMapFile = CreateFileMapping(
				reinterpret_cast<HANDLE>(hFile.get()),
				NULL,
				PAGE_READWRITE,
				0,
				Count,
				NULL);

			if (NULL == hMapFile)
			{
				Close();
				return nullptr;
			}

			UnmapViewOfFile(Ptr);
			Ptr = MapViewOfFile(hMapFile, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);
			if (!Ptr)
			{
				Close();
				return nullptr;
			}

			return (Type*)Ptr;
		}

		inline void deallocate(Type* const Ptr, const size_t Count)
		{
			--AllocCount;

			if (0 == AllocCount)
			{
				UnmapViewOfFile(Ptr);
				Close();
			}
		}

		inline std::shared_ptr<void> GetFile() const noexcept { return hFile; }
		inline HANDLE GetMapFile() const noexcept { return hMapFile; }

	private:
		inline bool Open(const char* InFilename) noexcept
		{
			Close();

			HANDLE TempFile = CreateFile(
				InFilename,
				GENERIC_WRITE | GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL
			);

			if (TempFile == INVALID_HANDLE_VALUE)
			{
				return false;
			}

			hFile = std::shared_ptr<void>(TempFile, HandleDeleter());

			return true;
		}
		inline bool Open(const std::string& InFilename) noexcept { return Open(InFilename.c_str()); }

		void Close()
		{
		}

	private:
		std::shared_ptr<void> hFile;
		HANDLE hMapFile = NULL;
		void* Ptr = nullptr;

		size_t AllocCount = 0;
	};
}