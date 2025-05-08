#pragma once

#include <stdint.h>
#include <string.h>

#include <memory>
#include <functional>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "Linx/Math/MathLibrary.h"

namespace Linx
{
	namespace ERingBufferReadMode
	{
		/** Do what when data length is less than read length */
		enum Mode : uint8_t
		{
			/** Do noting */
			ReadNothing = 0,

			/** Block until the data length is sufficient */
			ReadBlock,

			/** Read remaining data */
			ReadAll
		};
	}

	namespace ERingBufferWriteMode
	{
		/** Do what when write length is more than remaining length */
		enum Mode : uint8_t
		{
			/** Do nothing */
			WriteNothing = 0,

			/** Block until the remaining length is sufficient */
			WriteBlock,

			/** Cover the data from the position of Front, and set Front to Rear */
			WriteCover,

			/** Write data to the remaining space */
			WriteFill
		};
	}

	template<class Type, class Alloc>
	class RingBuffer;

	/**
	 * Points to elements in RingBuffer.
	 * RingBuffer must be specified in the constructor.
	 */
	template<class Type, class Alloc>
	class RingBufferConstIterator
	{
		template<class RingBufferType, class RingBufferAlloc>
		friend class RingBuffer;

	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = Type;
		using difference_type = std::ptrdiff_t;
		using pointer = const value_type*;
		using reference = const value_type&;

		/** RingBuffer's size_type */
		using size_type = size_t;

	public:
		inline pointer GetPtr() const noexcept { return RB->pBuffer + GetOffset(); }

		inline size_type GetOffset() const noexcept { return EntireOffset & RB->OffsetMask; }

	public:
		inline RingBufferConstIterator(RingBuffer<Type, Alloc>* InPtr) noexcept :
			RB(InPtr),
			EntireOffset()
		{}

		inline RingBufferConstIterator(const RingBufferConstIterator& InIterator) noexcept :
			RB(InIterator.RB)
		{
			EntireOffset.store(InIterator.EntireOffset);
		}

		inline reference operator*() const noexcept { return RB->pBuffer[GetOffset()]; }

		inline pointer operator->() const noexcept { return RB->pBuffer; }

		inline RingBufferConstIterator& operator++() noexcept
		{
			EntireOffset += 1;
			return *this;
		}

		inline RingBufferConstIterator operator++(int) noexcept
		{
			RingBufferConstIterator Tmp = *this;
			++*this;
			return Tmp;
		}

		inline RingBufferConstIterator& operator--() noexcept
		{
			EntireOffset -= 1;
			return *this;
		}

		inline RingBufferConstIterator operator--(int) noexcept
		{
			RingBufferConstIterator Tmp = *this;
			--*this;
			return Tmp;
		}

		inline RingBufferConstIterator& operator+=(const difference_type InOffset) noexcept
		{
			EntireOffset += InOffset;
			return *this;
		}

		inline RingBufferConstIterator operator+(const difference_type InOffset) const noexcept
		{
			RingBufferConstIterator Tmp = *this;
			Tmp += InOffset;
			return Tmp;
		}

		inline RingBufferConstIterator& operator-=(const difference_type InOffset) noexcept
		{
			return *this += -InOffset;
		}

		inline RingBufferConstIterator& operator=(const RingBufferConstIterator& Right) noexcept
		{
			RB = Right.RB;
			EntireOffset = Right.EntireOffset;
			return *this;
		}

		inline RingBufferConstIterator operator-(const difference_type InOffset) const noexcept
		{
			RingBufferConstIterator Tmp = *this;
			Tmp -= InOffset;
			return Tmp;
		}

		inline difference_type operator-(const RingBufferConstIterator& Right) const noexcept
		{
			return EntireOffset - Right.EntireOffset;
		}

		inline reference operator[](const difference_type InOffset) const noexcept
		{
			return *(*this + InOffset);
		}

		inline bool operator==(const RingBufferConstIterator& Right) const noexcept
		{
			return EntireOffset == Right.EntireOffset;
		}

		inline bool operator!=(const RingBufferConstIterator& Right) const noexcept
		{
			return !(*this == Right);
		}

		inline bool operator<(const RingBufferConstIterator& Right) const noexcept
		{
			return EntireOffset < Right.EntireOffset;
		}

		inline bool operator>(const RingBufferConstIterator& Right) const noexcept
		{
			return EntireOffset > Right.EntireOffset;
		}

		inline bool operator<=(const RingBufferConstIterator& Right) const noexcept
		{
			return !(Right < *this);
		}

		inline bool operator>=(const RingBufferConstIterator& Right) const noexcept
		{
			return !(*this < Right);
		}

	protected:
		/** Indicates which RingBuffer is pointed to */
		RingBuffer<Type, Alloc>* RB;

		/** The entire offset of the element in the RingBuffer */
		std::atomic<uint64_t> EntireOffset = 0;
	};

	/**
	 * Points to elements in RingBuffer.
	 * RingBuffer must be specified in the constructor.
	 * Iterators of different RingBuffers cannot be compared.
	 */
	template<class Type, class Alloc>
	class RingBufferIterator : public RingBufferConstIterator<Type, Alloc>
	{
		using Super = RingBufferConstIterator<Type, Alloc>;

	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = Type;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type*;
		using reference = value_type&;

		/** RingBuffer's size_type */
		using size_type = size_t;

	public:
		inline pointer GetPtr() const noexcept { return Super::RB->pBuffer + Super::GetOffset(); }

	public:
		inline RingBufferIterator(RingBuffer<Type, Alloc>* InPtr) noexcept :
			Super(InPtr)
		{}
		
		inline RingBufferIterator(const RingBufferIterator& InIterator) noexcept :
			Super(InIterator)
		{}

		inline reference operator*() const noexcept { return Super::RB->pBuffer[Super::GetOffset()]; }

		inline pointer operator->() const noexcept { return Super::RB->pBuffer; }

		inline RingBufferIterator& operator++() noexcept
		{
			Super::EntireOffset += 1;
			return *this;
		}

		inline RingBufferIterator operator++(int) noexcept
		{
			RingBufferIterator Tmp = *this;
			++*this;
			return Tmp;
		}

		inline RingBufferIterator& operator--() noexcept
		{
			Super::EntireOffset -= 1;
			return *this;
		}

		inline RingBufferIterator operator--(int) noexcept
		{
			RingBufferIterator Tmp = *this;
			--*this;
			return Tmp;
		}

		inline RingBufferIterator& operator+=(const difference_type InOffset) noexcept
		{
			Super::EntireOffset += InOffset;
			return *this;
		}

		inline RingBufferIterator operator+(const difference_type InOffset) const noexcept
		{
			RingBufferIterator Tmp = *this;
			Tmp += InOffset;
			return Tmp;
		}

		inline RingBufferIterator& operator-=(const difference_type InOffset) noexcept
		{
			return *this += -InOffset;
		}

		inline RingBufferIterator& operator=(const RingBufferIterator& Right) noexcept
		{
			Super::RB = Right.RB;
			Super::EntireOffset.store(Right.EntireOffset);
			return *this;
		}

		inline RingBufferIterator operator-(const difference_type InOffset) const noexcept
		{
			RingBufferIterator Tmp = *this;
			Tmp -= InOffset;
			return Tmp;
		}

		inline difference_type operator-(const RingBufferIterator& Right) const noexcept
		{
			return Super::EntireOffset - Right.Super::EntireOffset;
		}

		inline reference operator[](const difference_type InOffset) const noexcept
		{
			return *(*this + InOffset);
		}

		inline bool operator==(const RingBufferIterator& Right) const noexcept
		{
			return Super::EntireOffset == Right.Super::EntireOffset;
		}

		bool operator!=(const RingBufferIterator& Right) const noexcept
		{
			return !(*this == Right);
		}

		bool operator<(const RingBufferIterator& Right) const noexcept
		{
			return Super::EntireOffset < Right.Super::EntireOffset;
		}

		bool operator>(const RingBufferIterator& Right) const noexcept
		{
			return Super::EntireOffset > Right.Super::EntireOffset;
		}

		bool operator<=(const RingBufferIterator& Right) const noexcept
		{
			return !(Right < *this);
		}

		bool operator>=(const RingBufferIterator& Right) const noexcept
		{
			return !(*this < Right);
		}
	};


	/**
	 * A thread-safe RingBuffer that has a head and a rear.
	 * The element is inserted at the rear and popped at the head.
	 * The allocated memory size will be aligned to a power of 2.
	 */
	template<class Type, class Alloc = std::allocator<Type>>
	class RingBuffer
	{
	public:
		using value_type = Type;
		using allocator_type = Alloc;
		using pointer = Type*;
		using const_pointer = const Type*;
		using reference = Type&;
		using const_reference = const Type&;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using iterator = RingBufferIterator<Type, Alloc>;
		using const_iterator = RingBufferConstIterator<Type, Alloc>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		static constexpr size_t value_size = sizeof(value_type);

		friend iterator;
		friend const_iterator;

	public:
		RingBuffer() noexcept;
		explicit RingBuffer(size_type Len) noexcept;
		RingBuffer(const RingBuffer& RB) noexcept;
		RingBuffer(RingBuffer&& RB) noexcept;
		~RingBuffer();

	public:
		RingBuffer& operator=(const RingBuffer& RB) noexcept;

		Type& operator[](difference_type Pos) noexcept;

		const Type& operator[](difference_type Pos) const noexcept;

	public:
		/** Specify a custom buffer. */
		inline void SetBuffer(value_type* Ptr, size_type Size, std::function<void(value_type*)>&& InDeleter) noexcept
		{
			DeallocBuffer();

			Deleter = InDeleter;
			bUseCustomDeleter = true;
			pBuffer = Ptr;
			SetMaxLen(Size);
		}

		/** Read data from RingBuffer to DstPtr memory */
		size_type Read(Type* DstPtr, size_type Len) noexcept;
		/** Read data from this RingBuffer to DstRingBuffer. */
		size_type Read(RingBuffer<Type, Alloc>& DstRingBuffer, size_type Len) noexcept;
		/**
		 * Use custom functions to read RingBuffer.
		 * @param Func	A function with arguments of Type (Type*, size_t).
		 */
		template<typename FuncType>
		auto Read(FuncType&& Func, size_type Len)
			-> std::conditional_t<
			std::is_void_v<std::invoke_result_t<FuncType, pointer, size_type>>,
			size_type,
			std::invoke_result_t<FuncType, pointer, size_type>>;

		/** Write data from SrcPtr memory to RingBuffer. */
		size_type Write(const Type* SrcPtr, size_type Len) noexcept;
		/** Write data from SrcRingBuffer to this RingBuffer. */
		size_type Write(RingBuffer<Type, Alloc>& SrcRingBuffer, size_type Len) noexcept;
		/**
		 * Use custom functions to write RingBuffer.
		 * @param Func	A function with arguments of Type (Type*, size_t).
		 */
		template<typename FuncType>
		auto Write(FuncType&& Func, size_type Len)
			-> std::conditional_t<
			std::is_void_v<std::invoke_result_t<FuncType, pointer, size_type>>,
			size_type,
			std::invoke_result_t<FuncType, pointer, size_type>>;

		/**
		 * Reallocate the size of RingBuffer.
		 *
		 * @note The size will be aligned to a power of 2.
		 * @return The size of the actual allocation.
		 */
		size_type ReallocBuffer(size_type Size) noexcept;

		/** Clear the buffer. */
		void Clear() noexcept;

	public:
		/** Returns data length. */
		inline size_type GetDataLen() const noexcept { return Rear - Head; }

		/** Returns the length of the remaining space. */
		inline size_type GetRemainLen() const noexcept { return MaxLen - GetDataLen(); }

		/** Returns the max length of the buffer. */
		inline size_type GetMaxLen() const noexcept { return MaxLen; }

		/** Returns the head position int the buffer. */
		inline size_type GetHead() const noexcept { return Head; }

		/** Returns the rear position int the buffer. */
		inline size_type GetRear() const noexcept { return Rear; }

		/** Returns the pointer to the buffer. */
		inline value_type* GetBuffer() const noexcept { return pBuffer; }

		/** Returns whether the buffer is full. */
		inline bool IsFull() const noexcept { return (Rear - Head) == MaxLen; }

		/** Returns whether the buffer is empty. */
		inline bool IsEmpty() const noexcept { return Head == Rear; }

		ERingBufferReadMode::Mode ReadMode;
		ERingBufferWriteMode::Mode WriteMode;

	public:
		inline iterator begin() noexcept { return Head; }
		inline const_iterator begin() const noexcept { return Head; }
		inline iterator end() noexcept { return Rear; }
		inline const_iterator end() const noexcept { return Rear; }
		inline reverse_iterator rbegin() noexcept { return reverse_iterator(end); }
		inline const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end); }
		inline reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
		inline const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
		inline const_iterator cbegin() const noexcept { return begin(); }
		inline const_iterator cend() const noexcept { return end(); }
		inline const_reverse_iterator crbegin() const noexcept { return rbegin(); }
		inline const_reverse_iterator crend() const noexcept { return rend(); }

	private:
		/** Pointer to the buffer */
		Type* pBuffer;

		/** Length of the buffer */
		size_type MaxLen;

		/** Used to mask the offset */
		size_type OffsetMask;

		/** Iterator pointing to the read position */
		iterator Head;

		/** Iterator pointing to the write position */
		iterator Rear;

		/** Used to allocate the buffer */
		allocator_type Allocator;

		/** A custom function to free the buffer */
		std::function<void(value_type*)> Deleter;

		/** Whether to use a custom function to free the buffer */
		bool bUseCustomDeleter = false;

		/** Used by read condition variable */
		std::mutex ReadConditionMutex;

		/** Used when read mode is ReadBlock */
		std::condition_variable ReadConditionVar;

		/** Used by write condition variable */
		std::mutex WriteConditionMutex;

		/** Used when write mode is ReadBlock */
		std::condition_variable WriteConditionVar;

		/** Used to avoid the conflict between reading and writing */
		std::mutex WaitLock;

	private:
		void DeallocBuffer();

		size_type ReadImpl(value_type* DstPtr, size_type Len) noexcept;
		size_type ReadImpl(RingBuffer& DstRingBuffer, size_type Len) noexcept;
		template<typename FuncType>
		auto ReadImpl(FuncType&& Func, size_type Len) noexcept
			-> std::conditional_t<
			std::is_void_v<std::invoke_result_t<FuncType, pointer, size_type>>,
			size_type,
			std::invoke_result_t<FuncType, pointer, size_type>>;

		size_type WriteImpl(const value_type* SrcPtr, size_type Len) noexcept;
		size_type WriteImpl(RingBuffer& SrcRingBuffer, size_type Len) noexcept;
		template<typename FuncType>
		auto WriteImpl(FuncType&& Func, size_type Len) noexcept
			-> std::conditional_t<
			std::is_void_v<std::invoke_result_t<FuncType, pointer, size_type>>,
			size_type,
			std::invoke_result_t<FuncType, pointer, size_type>>;

		void CopyPart(const RingBuffer& RB) noexcept;

		inline void SetMaxLen(size_type Size) noexcept
		{
			MaxLen = Size;
			OffsetMask = MaxLen - 1;
		}

		void ReadWait(size_type Len) noexcept
		{
			while (GetDataLen() < Len)
			{
				std::unique_lock<std::mutex> lock(WaitLock, std::defer_lock);
				if (lock.try_lock())
				{
					std::unique_lock<std::mutex> uLock(ReadConditionMutex);
					ReadConditionVar.wait(uLock);
				}
				WriteConditionVar.notify_one();
			}
		}

		void WriteWait(size_type Len) noexcept
		{
			while (GetRemainLen() < Len)
			{
				std::unique_lock<std::mutex> lock(WaitLock, std::defer_lock);
				if (lock.try_lock())
				{
					std::unique_lock<std::mutex> uLock(WriteConditionMutex);
					WriteConditionVar.wait(uLock);
				}
				ReadConditionVar.notify_one();
			}
		}
	};

	/************************************************************************/
	/*                       Function definition                            */
	/************************************************************************/

	template<class Type, class Alloc>
	RingBuffer<Type, Alloc>::RingBuffer() noexcept :
		pBuffer(nullptr),
		MaxLen(0),
		Head(this),
		Rear(this),
		ReadMode(ERingBufferReadMode::ReadBlock),
		WriteMode(ERingBufferWriteMode::WriteBlock)
	{
	}

	template<class Type, class Alloc>
	RingBuffer<Type, Alloc>::RingBuffer(size_type Len) noexcept :
		RingBuffer()
	{
		ReallocBuffer(Len);
	}

	template<class Type, class Alloc>
	RingBuffer<Type, Alloc>::RingBuffer(const RingBuffer& RB) noexcept :
		pBuffer(nullptr),
		Head(this),
		Rear(this)
	{

		ReallocBuffer(RB.MaxLen);
		memcpy(pBuffer, RB.pBuffer, value_size * MaxLen);
		CopyPart(RB);
	}

	template<class Type, class Alloc>
	RingBuffer<Type, Alloc>::RingBuffer(RingBuffer&& RB) noexcept :
		Head(this),
		Rear(this)
	{

		CopyPart(RB);
		pBuffer = RB.pBuffer;
		RB.pBuffer = nullptr;
	}

	template<class Type, class Alloc>
	RingBuffer<Type, Alloc>::~RingBuffer()
	{

		DeallocBuffer();
	}

	template<class Type, class Alloc>
	typename RingBuffer<Type, Alloc>::size_type RingBuffer<Type, Alloc>::Read(Type* DstPtr, size_type Len) noexcept
	{
		size_type Ret = 0;

		if (Len <= 0)
		{
			return Ret;
		}

		auto DataLen = GetDataLen();
		if (DataLen >= Len)
		{
			Ret = ReadImpl(DstPtr, Len);
		}
		else
		{
			switch (ReadMode)
			{
			case ERingBufferReadMode::ReadNothing:
				break;
			case ERingBufferReadMode::ReadBlock:
				ReadWait(Len);
				Ret = ReadImpl(DstPtr, Len);
				break;
			case ERingBufferReadMode::ReadAll:
				Ret = ReadImpl(DstPtr, DataLen);
				break;
			default:
				break;
			}
		}

		WriteConditionVar.notify_one();

		return Ret;
	}

	template<class Type, class Alloc>
	typename RingBuffer<Type, Alloc>::size_type RingBuffer<Type, Alloc>::Read(RingBuffer<Type, Alloc>& DstRingBuffer, size_type Len) noexcept
	{
		size_type Ret = 0;
		size_type TransLen = 0;

		if (Len <= 0)
		{
			return Ret;
		}

		auto DataLen = GetDataLen();
		if (DataLen >= Len)
		{
			TransLen = Len;
		}
		else
		{
			switch (ReadMode)
			{
			case ERingBufferReadMode::ReadNothing:
				TransLen = 0;
				break;
			case ERingBufferReadMode::ReadBlock:
				ReadWait(Len);
				break;
			case ERingBufferReadMode::ReadAll:
				TransLen = DataLen;
				break;
			default:
				TransLen = 0;
				break;
			}
		}

		bool bCoverFlag = false;
		auto DstRemainLen = DstRingBuffer.GetRemainLen();
		if (DstRemainLen < TransLen)
		{
			switch (DstRingBuffer.WriteMode)
			{
			case ERingBufferWriteMode::WriteNothing:
				TransLen = 0;
				break;
			case ERingBufferWriteMode::WriteBlock:
				DstRingBuffer.WriteWait(TransLen);
				break;
			case ERingBufferWriteMode::WriteCover:
				bCoverFlag = true;
				break;
			case ERingBufferWriteMode::WriteFill:
				TransLen = DstRemainLen;
				break;
			default:
				TransLen = 0;
				break;
			}
		}

		Ret = ReadImpl(DstRingBuffer, TransLen);
		if (bCoverFlag)
		{
			DstRingBuffer.Head = DstRingBuffer.Rear;
		}

		WriteConditionVar.notify_one();
		DstRingBuffer.ReadConditionVar.notify_one();

		return Ret;
	}

	template<class Type, class Alloc>
	template<typename FuncType>
	auto Linx::RingBuffer<Type, Alloc>::Read(FuncType&& Func, size_type Len)
		->std::conditional_t<
		std::is_void_v<std::invoke_result_t<FuncType, pointer, size_type>>,
		size_type,
		std::invoke_result_t<FuncType, pointer, size_type>>
	{
		size_type Ret = 0;

		if (Len <= 0)
		{
			return Ret;
		}

		auto DataLen = GetDataLen();

		if (DataLen >= Len)
		{
			Ret = ReadImpl(Func, Len);
		}
		else
		{
			switch (ReadMode)
			{
			case ERingBufferReadMode::ReadNothing:
				break;
			case ERingBufferReadMode::ReadBlock:
				ReadWait(Len);
				Ret = ReadImpl(Func, Len);
				break;
			case ERingBufferReadMode::ReadAll:
				Ret = ReadImpl(Func, DataLen);
				break;
			default:
				break;
			}
		}

		WriteConditionVar.notify_one();

		return Ret;
	}

	template<class Type, class Alloc>
	typename RingBuffer<Type, Alloc>::size_type RingBuffer<Type, Alloc>::Write(const Type* SrcPtr, size_type Len) noexcept
	{
		size_type Ret = 0;

		if (Len <= 0)
		{
			return Ret;
		}

		auto RemainLen = GetRemainLen();
		if (RemainLen >= Len)
		{
			Ret = WriteImpl(SrcPtr, Len);
		}
		else
		{
			switch (WriteMode)
			{
			case ERingBufferWriteMode::WriteNothing:
				break;
			case ERingBufferWriteMode::WriteBlock:
				WriteWait(Len);
				Ret = WriteImpl(SrcPtr, Len);
				break;
			case ERingBufferWriteMode::WriteCover:
				Ret = WriteImpl(SrcPtr, Len);
				break;
			case ERingBufferWriteMode::WriteFill:
				Ret = WriteImpl(SrcPtr, RemainLen);
				break;
			default:
				break;
			}
		}

		ReadConditionVar.notify_one();

		return Ret;
	}

	template<class Type, class Alloc>
	typename RingBuffer<Type, Alloc>::size_type RingBuffer<Type, Alloc>::Write(RingBuffer<Type, Alloc>& SrcRingBuffer, size_type Len) noexcept
	{
		size_type Ret = 0;
		size_type TransLen = 0;

		if (Len <= 0)
		{
			return Ret;
		}

		auto SrcDataLen = SrcRingBuffer.GetDataLen();
		if (SrcDataLen >= Len)
		{
			TransLen = Len;
		}
		else
		{
			switch (SrcRingBuffer.ReadMode)
			{
			case ERingBufferReadMode::ReadNothing:
				TransLen = 0;
				break;
			case ERingBufferReadMode::ReadBlock:
				SrcRingBuffer.ReadWait(Len);
				break;
			case ERingBufferReadMode::ReadAll:
				TransLen = SrcDataLen;
				break;
			default:
				TransLen = 0;
				break;
			}
		}

		bool bCoverFlag = false;
		auto RemainLen = GetRemainLen();
		if (RemainLen < TransLen)
		{
			switch (WriteMode)
			{
			case ERingBufferWriteMode::WriteNothing:
				TransLen = 0;
				break;
			case ERingBufferWriteMode::WriteBlock:
				WriteWait(TransLen);
				break;
			case ERingBufferWriteMode::WriteCover:
				bCoverFlag = true;
				break;
			case ERingBufferWriteMode::WriteFill:
				TransLen = RemainLen;
				break;
			default:
				TransLen = 0;
				break;
			}
		}

		Ret = WriteImpl(SrcRingBuffer, TransLen);
		if (bCoverFlag)
		{
			Head = Rear;
		}

		SrcRingBuffer.WriteConditionVar.notify_one();
		ReadConditionVar.notify_one();

		return Ret;
	}

	template<class Type, class Alloc>
	template<typename FuncType>
	auto Linx::RingBuffer<Type, Alloc>::Write(FuncType&& Func, size_type Len)
		->std::conditional_t<
		std::is_void_v<std::invoke_result_t<FuncType, pointer, size_type>>,
		size_type,
		std::invoke_result_t<FuncType, pointer, size_type>>
	{
		size_type Ret = 0;

		if (Len <= 0)
		{
			return Ret;
		}

		auto RemainLen = GetRemainLen();
		if (RemainLen >= Len)
		{
			Ret = WriteImpl(Func, Len);
		}
		else
		{
			switch (WriteMode)
			{
			case ERingBufferWriteMode::WriteNothing:
				break;
			case ERingBufferWriteMode::WriteBlock:
				WriteWait(Len);
				Ret = WriteImpl(Func, Len);
				break;
			case ERingBufferWriteMode::WriteCover:
				Ret = WriteImpl(Func, Len);
				break;
			case ERingBufferWriteMode::WriteFill:
				Ret = WriteImpl(Func, RemainLen);
				break;
			default:
				break;
			}
		}

		ReadConditionVar.notify_one();

		return Ret;
	}

	template<class Type, class Alloc>
	typename RingBuffer<Type, Alloc>::size_type RingBuffer<Type, Alloc>::ReallocBuffer(size_type Size) noexcept
	{
		Size = CeilToPowerOfTwo(Size);

		DeallocBuffer();
		pBuffer = Allocator.allocate(Size);
		bUseCustomDeleter = false;
		SetMaxLen(Size);

		return Size;
	}

	template<class Type, class Alloc>
	void RingBuffer<Type, Alloc>::Clear() noexcept
	{
		Head.EntireOffset = 0;
		Rear.EntireOffset = 0;
	}

	template<class Type, class Alloc>
	RingBuffer<Type, Alloc>& RingBuffer<Type, Alloc>::operator=(const RingBuffer& RB) noexcept
	{
		if (this == &RB)
		{
			return *this;
		}

		ReallocBuffer(RB.MaxLen);
		memcpy(pBuffer, RB.pBuffer, value_size * MaxLen);
		CopyPart(RB);

		return *this;
	}

	template<class Type, class Alloc>
	Type& RingBuffer<Type, Alloc>::operator[](difference_type Pos) noexcept
	{
		return Head[Pos];
	}

	template<class Type, class Alloc>
	const Type& RingBuffer<Type, Alloc>::operator[](difference_type Pos) const noexcept
	{
		return Head[Pos];
	}

	template<class Type, class Alloc>
	void RingBuffer<Type, Alloc>::DeallocBuffer()
	{
		if (pBuffer)
		{
			if (bUseCustomDeleter)
			{
				Deleter(pBuffer);
			}
			else
			{
				Allocator.deallocate(pBuffer, MaxLen);
			}
			pBuffer = nullptr;
		}
	}

	template<class Type, class Alloc>
	typename RingBuffer<Type, Alloc>::size_type RingBuffer<Type, Alloc>::ReadImpl(value_type* DstPtr, size_type Len) noexcept
	{
		if (Len <= 0)
		{
			return 0;
		}

		std::atomic_thread_fence(std::memory_order_acquire);

		if (Head.GetOffset() + Len < MaxLen)
		{
			memcpy(DstPtr, Head.GetPtr(), value_size * Len);
		}
		else
		{
			memcpy(DstPtr, Head.GetPtr(), value_size * (MaxLen - Head.GetOffset()));
			memcpy(DstPtr + MaxLen - Head.GetOffset(), pBuffer, value_size * (Head.GetOffset() + Len - MaxLen));
		}

		std::atomic_thread_fence(std::memory_order_release);

		Head += Len;

		return Len;
	}

	template<class Type, class Alloc>
	typename RingBuffer<Type, Alloc>::size_type RingBuffer<Type, Alloc>::ReadImpl(RingBuffer& DstRingBuffer, size_type Len) noexcept
	{
		if (Len <= 0)
		{
			return 0;
		}

		size_type SrcSplitLen = Len;
		size_type DstSplitLen = Len;

		std::atomic_thread_fence(std::memory_order_acquire);

		if (Head.GetOffset() + Len >= MaxLen)
		{
			SrcSplitLen = MaxLen - Head.GetOffset();
		}
		if (DstRingBuffer.Rear.GetOffset() + Len >= DstRingBuffer.MaxLen)
		{
			DstSplitLen = DstRingBuffer.MaxLen - DstRingBuffer.Rear.GetOffset();
		}

		if (SrcSplitLen < DstSplitLen)
		{
			memcpy(DstRingBuffer.Rear.GetPtr(), Head.GetPtr(), value_size * SrcSplitLen);
			memcpy(DstRingBuffer.Rear.GetPtr() + SrcSplitLen, pBuffer, value_size * (DstSplitLen - SrcSplitLen));
			memcpy(DstRingBuffer.pBuffer, pBuffer + DstSplitLen - SrcSplitLen, value_size * (Len - DstSplitLen));

		}
		else
		{
			memcpy(DstRingBuffer.Rear.GetPtr(), Head.GetPtr(), value_size * DstSplitLen);
			memcpy(DstRingBuffer.pBuffer, Head.GetPtr() + DstSplitLen, value_size * (SrcSplitLen - DstSplitLen));
			memcpy(DstRingBuffer.pBuffer + SrcSplitLen - DstSplitLen, pBuffer, value_size * (Len - SrcSplitLen));
		}

		std::atomic_thread_fence(std::memory_order_release);

		Head += Len;
		DstRingBuffer.Rear += Len;

		return Len;
	}

	template<class Type, class Alloc>
	template<typename FuncType>
	auto Linx::RingBuffer<Type, Alloc>::ReadImpl(FuncType&& Func, size_type Len) noexcept
		-> std::conditional_t<
		std::is_void_v<std::invoke_result_t<FuncType, pointer, size_type>>,
		size_type,
		std::invoke_result_t<FuncType, pointer, size_type>>
	{
		using RetType = std::conditional_t<
			std::is_void_v<std::invoke_result_t<FuncType, pointer, size_type>>,
			size_type,
			std::invoke_result_t<FuncType, pointer, size_type>>;

		if (Len <= 0)
		{
			return 0;
		}

		std::atomic_thread_fence(std::memory_order_acquire);

		if (Head.GetOffset() + Len < MaxLen)
		{
			if constexpr (std::is_void_v<decltype(Func(nullptr, 0))>)
			{
				Func(Head.GetPtr(), Len);
			}
			else
			{
				RetType Ret;
				if ((Ret = Func(Head.GetPtr(), Len)) < 0)
					return Ret;
			}
		}
		else
		{
			auto ReadLen0 = MaxLen - Head.GetOffset();
			auto ReadLen1 = Head.GetOffset() + Len - MaxLen;
			if constexpr (std::is_void_v<decltype(Func(nullptr, 0))>)
			{
				if (ReadLen0 > 0)
				{
					Func(Head.GetPtr(), ReadLen0);
				}
				if (ReadLen1 > 0)
				{
					Func(pBuffer, ReadLen1);
				}
			}
			else
			{
				RetType Ret;
				if (ReadLen0 > 0 && (Ret = Func(Head.GetPtr(), ReadLen0)) < 0)
					return Ret;
				if (ReadLen1 > 0 && (Ret = Func(pBuffer, ReadLen1)) < 0)
					return Ret;
			}
		}

		std::atomic_thread_fence(std::memory_order_release);

		Head += Len;

		return Len;
	}

	template<class Type, class Alloc>
	typename RingBuffer<Type, Alloc>::size_type RingBuffer<Type, Alloc>::WriteImpl(const value_type* SrcPtr, size_type Len) noexcept
	{
		if (Len <= 0)
		{
			return 0;
		}

		std::atomic_thread_fence(std::memory_order_acquire);

		if (Rear.GetOffset() + Len < MaxLen)
		{
			memcpy(Rear.GetPtr(), SrcPtr, value_size * Len);
		}
		else
		{
			memcpy(Rear.GetPtr(), SrcPtr, value_size * (MaxLen - Rear.GetOffset()));
			memcpy(pBuffer, SrcPtr + MaxLen - (Rear.GetPtr() - pBuffer), value_size * (Rear.GetOffset() + Len - MaxLen));
		}

		std::atomic_thread_fence(std::memory_order_release);

		Rear += Len;

		return Len;
	}

	template<class Type, class Alloc>
	typename RingBuffer<Type, Alloc>::size_type RingBuffer<Type, Alloc>::WriteImpl(RingBuffer& SrcRingBuffer, size_type Len) noexcept
	{
		if (Len <= 0)
		{
			return 0;
		}

		size_type DstSplitLen = Len;
		size_type SrcSplitLen = Len;

		std::atomic_thread_fence(std::memory_order_acquire);

		if (Rear.GetOffset() + Len >= MaxLen)
		{
			DstSplitLen = MaxLen - Rear.GetOffset();
		}
		if (SrcRingBuffer.Head.GetOffset() + Len >= SrcRingBuffer.MaxLen)
		{
			SrcSplitLen = SrcRingBuffer.MaxLen - SrcRingBuffer.Head.GetOffset();
		}

		if (SrcSplitLen < DstSplitLen)
		{
			memcpy(Rear.GetPtr(), SrcRingBuffer.Head.GetPtr(), value_size * SrcSplitLen);
			memcpy(Rear.GetPtr() + SrcSplitLen, SrcRingBuffer.pBuffer, value_size * (DstSplitLen - SrcSplitLen));
			memcpy(pBuffer, SrcRingBuffer.pBuffer + DstSplitLen - SrcSplitLen, value_size * (Len - DstSplitLen));
		}
		else
		{
			memcpy(Rear.GetPtr(), SrcRingBuffer.Head.GetPtr(), value_size * DstSplitLen);
			memcpy(pBuffer, SrcRingBuffer.Head.GetPtr() + DstSplitLen, value_size * (SrcSplitLen - DstSplitLen));
			memcpy(pBuffer + SrcSplitLen - DstSplitLen, SrcRingBuffer.pBuffer, value_size * (Len - SrcSplitLen));
		}

		std::atomic_thread_fence(std::memory_order_release);

		SrcRingBuffer.Head += Len;
		Rear += Len;

		return Len;
	}

	template<class Type, class Alloc>
	template<typename FuncType>
	auto Linx::RingBuffer<Type, Alloc>::WriteImpl(FuncType&& Func, size_type Len) noexcept
		-> std::conditional_t<
		std::is_void_v<std::invoke_result_t<FuncType, pointer, size_type>>,
		size_type,
		std::invoke_result_t<FuncType, pointer, size_type>>
	{
		using RetType = std::conditional_t<
			std::is_void_v<std::invoke_result_t<FuncType, pointer, size_type>>,
			size_type,
			std::invoke_result_t<FuncType, pointer, size_type>>;

		if (Len <= 0)
		{
			return 0;
		}

		std::atomic_thread_fence(std::memory_order_acquire);

		if (Rear.GetOffset() + Len < MaxLen)
		{
			if constexpr (std::is_void_v<decltype(Func(nullptr, 0))>)
			{
				Func(Rear.GetPtr(), Len);
			}
			else
			{
				RetType Ret;
				if (Ret = Func(Rear.GetPtr(), Len) < 0)
					return Ret;
			}
		}
		else
		{
			auto WriteLen0 = MaxLen - Rear.GetOffset();
			auto WriteLen1 = Rear.GetOffset() + Len - MaxLen;
			if constexpr (std::is_void_v<decltype(Func(nullptr, 0))>)
			{
				if (WriteLen0 > 0)
				{
					Func(Rear.GetPtr(), WriteLen0);
				}
				if (WriteLen1 > 0)
				{
					Func(pBuffer, WriteLen1);
				}
			}
			else
			{
				RetType Ret;
				if (WriteLen0 > 0 && (Ret = Func(Rear.GetPtr(), WriteLen0)) < 0)
					return Ret;
				if (WriteLen1 > 0 && (Ret = Func(pBuffer, WriteLen1)) < 0)
					return Ret;
			}
		}

		std::atomic_thread_fence(std::memory_order_release);

		Rear += Len;

		return Len;
	}

	template<class Type, class Alloc>
	void RingBuffer<Type, Alloc>::CopyPart(const RingBuffer& RB) noexcept
	{
		Head.EntireOffset.store(RB.Head.EntireOffset);
		Rear.EntireOffset.store(RB.Rear.EntireOffset);
		ReadMode = RB.ReadMode;
		WriteMode = RB.WriteMode;
		bUseCustomDeleter = RB.bUseCustomDeleter;
		Deleter = RB.Deleter;
		SetMaxLen(RB.MaxLen);
	}
}
