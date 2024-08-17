#pragma once

#include <cstddef>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#endif

namespace Linx
{
	template<typename Type>
	class VirtualAllocator
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
		constexpr VirtualAllocator() noexcept {}
		constexpr VirtualAllocator(const VirtualAllocator&) noexcept = default;
		template <class Other>
		constexpr VirtualAllocator(const VirtualAllocator<Other>& InAllocator) noexcept :
			bValid(false)
		{}
		inline ~VirtualAllocator() = default;
		inline VirtualAllocator& operator=(const VirtualAllocator&) = default;

		inline Type* allocate(const size_t Count)
		{
			if (!bValid)
			{
				return new Type[Count];
			}

#ifdef _WIN32
			return (Type*)VirtualAlloc(
				NULL,
				Count * sizeof(value_type),
				MEM_COMMIT | MEM_RESERVE,
				PAGE_READWRITE
			);
#else
			return (Type*)mmap(
				NULL,
				Count * sizeof(value_type),
				PROT_READ | PROT_WRITE,
				MAP_PRIVATE | MAP_ANON,
				-1,
				0
			);
#endif
		}

		inline void deallocate(Type* const Ptr, const size_t Count)
		{
			if (!bValid)
			{
				delete[] Ptr;
				return;
			}

			if (Ptr)
			{
#ifdef _WIN32
				VirtualFree(Ptr, 0, MEM_RELEASE);
#else
				munmap(Ptr, Count * sizeof(value_type));
#endif
			}
		}

	private:
		bool bValid = true;
	};
}
