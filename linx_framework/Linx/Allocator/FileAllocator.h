#pragma once

#include <string>
#include <memory>
#ifndef _WIN32
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#endif
#include "Linx/Utils/HandleWrapper.h"

namespace Linx
{
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
#ifndef _WIN32
		using HANDLE = int;
#endif

	public:
		constexpr FileAllocator() noexcept = default;
		constexpr FileAllocator(const char* InFilename) noexcept { Open(InFilename); }
		constexpr FileAllocator(const std::string& InFilename) noexcept { Open(InFilename); }
		template <class Other>
		constexpr FileAllocator(const FileAllocator<Other>& InAllocator) noexcept :
			bValid(false),
			hFile(InAllocator.GetFile()),
			hMapFile(InAllocator.GetMapFile())
		{}

		inline Type* allocate(const size_t Count)
		{
			++AllocCount;
			if (!bValid)
			{
				return new Type[Count];
			}

#ifdef _WIN32
			SetFilePointer(hFile.get()->Get(), Count * sizeof(value_type), NULL, FILE_BEGIN);
			SetEndOfFile(hFile.get()->Get());

			hLastMapFile = hMapFile;
			hMapFile = CreateFileMapping(
				hFile.get()->Get(),
				NULL,
				PAGE_READWRITE,
				0,
				Count * sizeof(value_type),
				NULL);

			if (NULL == hMapFile)
			{
				return nullptr;
			}

			LastPtr = Ptr;
			Ptr = MapViewOfFile(hMapFile, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);
			if (!Ptr)
			{
				return nullptr;
			}

			return (Type*)Ptr;
#else
			if (ftruncate(hFile.get()->Get(), Count * TypeSize) == -1)
			{
				return nullptr;
			}

			struct stat sb;
			if (fstat((intptr_t)(hFile.get()->Get()), &sb) == -1)
			{
				return nullptr;
			}

			void* Ptr = mmap(0, Count * TypeSize, PROT_READ | PROT_WRITE, MAP_SHARED, hFile.get()->Get(), 0);
			if (Ptr == MAP_FAILED)
			{
				return nullptr;
			}

			return (Type*)Ptr;
#endif
		}

		inline void deallocate(Type* const InPtr, const size_t Count)
		{
			--AllocCount;
			if (!bValid)
			{
				delete[] InPtr;
				return;
			}

#ifdef _WIN32
			UnmapViewOfFile(LastPtr);
			CloseHandle(hLastMapFile);
#else
			munmap(LastPtr, Count * TypeSize);
			close(hLastMapFile);
#endif

			if (0 == AllocCount)
			{
#ifdef _WIN32
				UnmapViewOfFile(Ptr);
				CloseHandle(hMapFile);
#else
				munmap(Ptr, Count * TypeSize);
				close(hMapFile);
#endif
			}
		}

		inline std::shared_ptr<HandleWrapper> GetFile() const noexcept { return hFile; }
		inline HANDLE GetMapFile() const noexcept { return hMapFile; }

	private:
		bool Open(const char* InFilename) noexcept
		{
#ifdef _WIN32
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
#else
			HANDLE TempFile = open(InFilename, O_RDWR | O_CREAT, 0666);
			if (TempFile == -1)
			{
				return false;
			}
#endif

			hFile = std::make_shared<HandleWrapper>(TempFile);
			return true;
		}
		inline bool Open(const std::string& InFilename) noexcept { return Open(InFilename.c_str()); }

	private:
		std::shared_ptr<HandleWrapper> hFile;
		HANDLE hMapFile = NULL;
		HANDLE hLastMapFile = NULL;
		void* Ptr = nullptr;
		void* LastPtr = nullptr;

		size_t AllocCount = 0;

		bool bValid = true;
	};
}
