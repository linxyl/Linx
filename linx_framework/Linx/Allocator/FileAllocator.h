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
			if (INVALID_HANDLE_VALUE == reinterpret_cast<HANDLE>(hFile.get()))
			{
				return nullptr;
			}

			HANDLE TempMapFile = CreateFileMapping(
				reinterpret_cast<HANDLE>(hFile.get()),
				NULL,
				PAGE_READWRITE,
				0,
				GetFileSize(reinterpret_cast<HANDLE>(hFile.get()), NULL),
				NULL);

			if (NULL == TempMapFile)
			{
				Close();
				return nullptr;
			}

			void* Ptr = MapViewOfFile(TempMapFile, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);
			if (!Ptr)
			{
				Close();
				return nullptr;
			}

			hMapFile = std::shared_ptr<void>(TempMapFile, HandleDeleter());

			return (Type*)Ptr;
		}

		inline void deallocate(Type* const Ptr, const size_t Count)
		{
			UnmapViewOfFile(Ptr);
			Close();
		}

		inline std::shared_ptr<void> GetFile() const noexcept { return hFile; }
		inline std::shared_ptr<void> GetMapFile() const noexcept { return hMapFile; }

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

			if (TempFile != INVALID_HANDLE_VALUE)
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
		std::shared_ptr<void> hMapFile;
	};
}