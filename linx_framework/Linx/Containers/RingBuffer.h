#pragma once

#include <stdint.h>
#include <string.h>

#include <memory>

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

			/** Cover the data from the position of Front, and set Front to Rear */
			Cover,

			/** Write data to the remaining space */
			Fill
		};
	}

	template<class Type, class Alloc = std::allocator<Type>>
	class RingBuffer;

	/**
	 * Points to elements in RingBuffer.
	 * RingBuffer must be specified in the constructor.
	 * Iterators of different RingBuffers cannot be compared.
	 */
	template<class Type, class Alloc>
	class RingBufferConstIterator
	{
		template<class RingBufferType, class RingBufferAlloc>
		friend class RingBuffer;

	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type        = Type;
		using difference_type   = std::ptrdiff_t;
		using pointer           = Type*;
		using reference         = const value_type&;

		/** RingBuffer's size_type */
		using size_type			= size_t;

	public:
		inline pointer GetPtr() const noexcept { return RB->pBuffer + Offset; }

		inline size_type GetOffset() const noexcept { return Offset; }

	public:
		inline RingBufferConstIterator(RingBuffer<Type, Alloc>* InPtr) noexcept :
			RB(InPtr),
			Offset(),
			Cycle()
		{}

		inline reference operator*() const noexcept { return RB->pBuffer[Offset]; }

		inline pointer operator->() const noexcept { return RB->pBuffer; }

		inline RingBufferConstIterator& operator++() noexcept
		{
			size_type Temp = (Offset + 1) & RB->OffsetMask;
			if (Temp <= Offset)
			{
				++Cycle;
			}
			Offset = Temp;

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
			size_type Temp = (Offset - 1) & RB->OffsetMask;
			if (Temp >= Offset)
			{
				--Cycle;
			}
			Offset = Temp;

			return *this;
		}

		inline RingBufferConstIterator operator--(int) noexcept
		{
			RingBufferConstIterator Tmp = *this;
			--*this;
			return Tmp;
		}

		inline RingBufferConstIterator& operator+=(const difference_type Off) noexcept
		{
			size_type Temp = (Offset + Off) & RB->OffsetMask;
			if (Off >= 0)
			{
				if (Temp <= Offset)
				{
					++Cycle;
				}
			}
			else
			{
				if (Temp >= Offset)
				{
					--Cycle;
				}
			}
			Offset = Temp;

			return *this;
		}

		inline RingBufferConstIterator operator+(const difference_type Off) const noexcept
		{
			RingBufferConstIterator Tmp = *this;
			Tmp += Off;
			return Tmp;
		}

		inline RingBufferConstIterator& operator-=(const difference_type Off) noexcept
		{
			return *this += -Off;
		}

		inline RingBufferConstIterator& operator=(const RingBufferConstIterator& Right) noexcept
		{
			memcpy(this, &Right, sizeof(*this));

			return *this;
		}

		inline RingBufferConstIterator operator-(const difference_type Off) const noexcept
		{
			RingBufferConstIterator Tmp = *this;
			Tmp -= Off;
			return Tmp;
		}

		inline difference_type operator-(const RingBufferConstIterator& Right) const noexcept
		{
			if (Cycle == Right.Cycle + 1)
			{
				return Offset - Right.Offset + RB->MaxLen;
			}
			if (Cycle + 1 == Right.Cycle)
			{
				return Offset - Right.Offset - RB->MaxLen;
			}
			return Offset - Right.Offset;
		}

		inline reference operator[](const difference_type Off) const noexcept
		{
			return *(*this + Off);
		}

		inline bool operator==(const RingBufferConstIterator& Right) const noexcept
		{
			return (Offset == Right.Offset) && (Cycle == Right.Cycle);
		}

		bool operator!=(const RingBufferConstIterator& Right) const noexcept
		{
			return !(*this == Right);
		}

		bool operator<(const RingBufferConstIterator& Right) const noexcept
		{
			return (Cycle + 1 == Right.Cycle) || (Offset < Right.Offset);
		}

		bool operator>(const RingBufferConstIterator& Right) const noexcept
		{
			return (Cycle == Right.Cycle + 1) || (Offset > Right.Offset);
		}

		bool operator<=(const RingBufferConstIterator& Right) const noexcept
		{
			return !(Right < *this);
		}

		bool operator>=(const RingBufferConstIterator& Right) const noexcept
		{
			return !(*this < Right);
		}

	protected:
		/** Indicates which RingBuffer is pointed to */
		RingBuffer<Type, Alloc>* RB;

		/** The offset of the element in the RingBuffer */
		size_type Offset;

		/** Indicates this in which cycle */
		uint8_t Cycle;
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
		using value_type        = Type;
		using difference_type   = std::ptrdiff_t;
		using pointer           = Type*;
		using reference         = value_type&;

		/** RingBuffer's size_type */
		using size_type			= size_t;

	public:
		inline RingBufferIterator(RingBuffer<Type, Alloc>* InPtr) noexcept:
			Super(InPtr)
		{}

		inline reference operator*() const noexcept { return Super::RB->pBuffer[Super::Offset]; }

		inline pointer operator->() const noexcept { return Super::RB->pBuffer; }

		inline RingBufferIterator& operator++() noexcept
		{
			size_type Temp = (Super::Offset + 1) & Super::RB->OffsetMask;
			if (Temp <= Super::Offset)
			{
				++Super::Cycle;
			}
			Super::Offset = Temp;

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
			size_type Temp = (Super::Offset - 1) & Super::RB->OffsetMask;
			if (Temp >= Super::Offset)
			{
				--Super::Cycle;
			}
			Super::Offset = Temp;

			return *this;
		}

		inline RingBufferIterator operator--(int) noexcept
		{
			RingBufferIterator Tmp = *this;
			--*this;
			return Tmp;
		}

		inline RingBufferIterator& operator+=(const difference_type Off) noexcept
		{
			size_type Temp = (Super::Offset + Off) & Super::RB->OffsetMask;
			if (Off >= 0)
			{
				if (Temp <= Super::Offset)
				{
					++Super::Cycle;
				}
			}
			else
			{
				if (Temp >= Super::Offset)
				{
					--Super::Cycle;
				}
			}
			Super::Offset = Temp;

			return *this;
		}

		inline RingBufferIterator operator+(const difference_type Off) const noexcept
		{
			RingBufferIterator Tmp = *this;
			Tmp += Off;
			return Tmp;
		}

		inline RingBufferIterator& operator-=(const difference_type Off) noexcept
		{
			return *this += -Off;
		}

		inline RingBufferIterator& operator=(const RingBufferIterator& Right) noexcept
		{
			memcpy(this, &Right, sizeof(*this));

			return *this;
		}

		inline RingBufferIterator operator-(const difference_type Off) const noexcept
		{
			RingBufferIterator Tmp = *this;
			Tmp -= Off;
			return Tmp;
		}

		inline difference_type operator-(const RingBufferIterator& Right) const noexcept
		{
			if (Super::Cycle == Right.Super::Cycle + 1)
			{
				return Super::Offset - Right.Super::Offset + Super::RB->MaxLen;
			}
			if (Super::Cycle + 1 == Right.Super::Cycle)
			{
				return Super::Offset - Right.Super::Offset - Super::RB->MaxLen;
			}
			return Super::Offset - Right.Super::Offset;
		}

		inline reference operator[](const difference_type Off) const noexcept
		{
			return *(*this + Off);
		}

		inline bool operator==(const RingBufferIterator& Right) const noexcept
		{
			return (Super::Offset == Right.Super::Offset) && (Super::Cycle == Right.Super::Cycle);
		}

		bool operator!=(const RingBufferIterator& Right) const noexcept
		{
			return !(*this == Right);
		}

		bool operator<(const RingBufferIterator& Right) const noexcept
		{
			return (Super::Cycle + 1 == Right.Super::Cycle) || (Super::Offset < Right.Super::Offset);
		}

		bool operator>(const RingBufferIterator& Right) const noexcept
		{
			return (Super::Cycle == Right.Super::Cycle + 1) || (Super::Offset > Right.Super::Offset);
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
	 * A fast ring buffer that has a head and a rear.
	 * The element is inserted at the rear and popped at the head.
	 * The allocated memory size will be aligned to a power of 2.
	 */
	template<class Type, class Alloc>
	class RingBuffer
	{
	public:
		using value_type		= Type;
		using allocator_type	= Alloc;
		using pointer			= Type*;
		using const_pointer		= const Type*;
		using reference			= Type&;
		using const_reference	= const Type&;
		using size_type			= size_t;
		using difference_type	= ptrdiff_t;

		using iterator               = RingBufferIterator<Type, Alloc>;
		using const_iterator         = RingBufferConstIterator<Type, Alloc>;
		using reverse_iterator       = std::reverse_iterator<iterator>;
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
		/** Read data from RingBuffer to DstPtr memory */
		size_type Read(Type* DstPtr, size_type Len) noexcept;
		/** Get the pointer of the reading location. */
		size_type Read(Type** DstPtr, size_type Len) noexcept;
		/** Read data from this RingBuffer to DstRingBuffer. */
		size_type Read(RingBuffer<Type, Alloc>& DstRingBuffer, size_type Len) noexcept;

		/** Write data from SrcPtr memory to RingBuffer. */
		size_type Write(const Type* SrcPtr, size_type Len) noexcept;
		/** Write data from SrcRingBuffer to this RingBuffer. */
		size_type Write(RingBuffer<Type, Alloc>& SrcRingBuffer, size_type Len) noexcept;
		/**
		 * Use custom functions to write RingBuffer.
		 * @param Func	A function with arguments of Type (Type*, size_t).
		 */
		template<typename FuncType>
		size_type Write(FuncType&& Func, size_type Len);

		/**
		 * Reallocate the size of RingBuffer.
		 *
		 * @note The size will be aligned to a power of 2.
		 * @return The size of the actual allocation.
		 */
		size_type ReallocBuffer(size_type Size) noexcept;

	private:
		/** Pointer to the buffer */
		Type* pBuffer;

		/** Length of the buffer */
		size_type MaxLen;

		/** Used to mask the offset */
		size_type OffsetMask;

		/** Offset of the head position in the buffer */
		iterator Head;

		/** Offset of the rear position in the buffer */
		iterator Rear;

		/** Used to allocate the buffer */
		allocator_type Allocator;

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
		size_type ReadImpl(value_type* DstPtr, size_type Len) noexcept;
		size_type ReadImpl(RingBuffer& DstRingBuffer, size_type Len) noexcept;

		size_type WriteImpl(const value_type* SrcPtr, size_type Len) noexcept;
		size_type WriteImpl(RingBuffer& SrcRingBuffer, size_type Len) noexcept;
		template<typename FuncType>
		size_type WriteImpl(FuncType&& Func, size_type Len) noexcept;

		void CopyPart(const RingBuffer& RB) noexcept;
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
		ReadMode(ERingBufferReadMode::ReadAll),
		WriteMode(ERingBufferWriteMode::Fill)
	{
	}

	template<class Type, class Alloc>
	RingBuffer<Type, Alloc>::RingBuffer(size_type Len) noexcept:
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
		if (this == &RB)
		{
			return;
		}

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
		if (pBuffer)
		{
			Allocator.deallocate(pBuffer, MaxLen);
			pBuffer = nullptr;
		}
	}

	template<class Type, class Alloc>
	typename RingBuffer<Type, Alloc>::size_type RingBuffer<Type, Alloc>::Read(Type* DstPtr, size_type Len) noexcept
	{
		size_type Ret = 0;

		if (Len <= 0)
		{
			return Ret;
		}

		if (GetDataLen() >= Len)
		{
			Ret = ReadImpl(DstPtr, Len);
		}
		else
		{
			switch (ReadMode)
			{
			case ERingBufferReadMode::ReadNothing:
				break;
			case ERingBufferReadMode::ReadAll:
				Ret = ReadImpl(DstPtr, GetDataLen());
				break;
			default:
				break;
			}
		}

		return Ret;
	}

	template<class Type, class Alloc>
	typename RingBuffer<Type, Alloc>::size_type Linx::RingBuffer<Type, Alloc>::Read(Type** DstPtr, size_type Len) noexcept
	{
		size_type RemainingSize = MaxLen - Head.GetOffset();
		size_type Ret = std::min(RemainingSize, GetDataLen());

		*DstPtr = Head.GetPtr();
		Head += Ret;

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

		if (GetDataLen() >= Len)
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
			case ERingBufferReadMode::ReadAll:
				TransLen = GetDataLen();
				break;
			default:
				TransLen = 0;
				break;
			}
		}

		bool bCoverFlag = false;
		if (DstRingBuffer.GetRemainLen() < TransLen)
		{
			switch (DstRingBuffer.WriteMode)
			{
			case ERingBufferWriteMode::WriteNothing:
				TransLen = 0;
				break;
			case ERingBufferWriteMode::Cover:
				bCoverFlag = true;
				break;
			case ERingBufferWriteMode::Fill:
				TransLen = DstRingBuffer.GetRemainLen();
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

		if (GetRemainLen() >= Len)
		{
			Ret = WriteImpl(SrcPtr, Len);
		}
		else
		{
			switch (WriteMode)
			{
			case ERingBufferWriteMode::WriteNothing:
				break;
			case ERingBufferWriteMode::Cover:
				Ret = WriteImpl(SrcPtr, Len);
				break;
			case ERingBufferWriteMode::Fill:
				Ret = WriteImpl(SrcPtr, GetRemainLen());
				break;
			default:
				break;
			}
		}

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

		if (SrcRingBuffer.GetDataLen() >= Len)
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
			case ERingBufferReadMode::ReadAll:
				TransLen = SrcRingBuffer.GetDataLen();
				break;
			default:
				TransLen = 0;
				break;
			}
		}

		bool bCoverFlag = false;
		if (GetRemainLen() < TransLen)
		{
			switch (WriteMode)
			{
			case ERingBufferWriteMode::WriteNothing:
				TransLen = 0;
				break;
			case ERingBufferWriteMode::Cover:
				bCoverFlag = true;
				break;
			case ERingBufferWriteMode::Fill:
				TransLen = GetRemainLen();
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

		return Ret;
	}

	template<class Type, class Alloc>
	template<typename FuncType>
	typename RingBuffer<Type, Alloc>::size_type Linx::RingBuffer<Type, Alloc>::Write(FuncType&& Func, size_type Len)
	{
		size_type Ret = 0;

		if (Len <= 0)
		{
			return Ret;
		}

		if (GetRemainLen() >= Len)
		{
			Ret = WriteImpl(Func, Len);
		}
		else
		{
			switch (WriteMode)
			{
			case ERingBufferWriteMode::WriteNothing:
				break;
			case ERingBufferWriteMode::Cover:
				Ret = WriteImpl(Func, Len);
				break;
			case ERingBufferWriteMode::Fill:
				Ret = WriteImpl(Func, GetRemainLen());
				break;
			default:
				break;
			}
		}

		return Ret;
	}

	template<class Type, class Alloc>
	typename RingBuffer<Type, Alloc>::size_type RingBuffer<Type, Alloc>::ReallocBuffer(size_type Size) noexcept
	{
		Size = NextHigherPowerOfTwo(Size);

		if (pBuffer)
		{
			Allocator.deallocate(pBuffer, MaxLen);
		}
		pBuffer = Allocator.allocate(Size);

		MaxLen = Size;
		OffsetMask = MaxLen - 1;

		return Size;
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
	typename RingBuffer<Type, Alloc>::size_type RingBuffer<Type, Alloc>::ReadImpl(value_type* DstPtr, size_type Len) noexcept
	{
		if (Len <= 0)
		{
			return 0;
		}

		if (Head.GetOffset() + Len < MaxLen)
		{
			memcpy(DstPtr, Head.GetPtr(), value_size * Len);
		}
		else
		{
			memcpy(DstPtr, Head.GetPtr(), value_size * (MaxLen - Head.GetOffset()));
			memcpy(DstPtr + MaxLen - Head.GetOffset(), pBuffer, value_size * (Head.GetOffset() + Len - MaxLen));
		}

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
		if (Head.Offset + Len >= MaxLen)
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

		Head += Len;
		DstRingBuffer.Rear += Len;

		return Len;
	}

	template<class Type, class Alloc>
	typename RingBuffer<Type, Alloc>::size_type RingBuffer<Type, Alloc>::WriteImpl(const value_type* SrcPtr, size_type Len) noexcept
	{
		if (Len <= 0)
		{
			return 0;
		}

		if (Rear.Offset + Len < MaxLen)
		{
			memcpy(Rear.GetPtr(), SrcPtr, value_size * Len);
		}
		else
		{
			memcpy(Rear.GetPtr(), SrcPtr, value_size * (MaxLen - Rear.Offset));
			memcpy(pBuffer, SrcPtr + MaxLen - (Rear.GetPtr() - pBuffer), value_size * (Rear.Offset + Len - MaxLen));
		}

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

		SrcRingBuffer.Head += Len;
		Rear += Len;

		return Len;
	}

	template<class Type, class Alloc>
	template<typename FuncType>
	typename RingBuffer<Type, Alloc>::size_type Linx::RingBuffer<Type, Alloc>::WriteImpl(FuncType&& Func, size_type Len) noexcept
	{
		if (Len <= 0)
		{
			return 0;
		}

		if (Rear.Offset + Len < MaxLen)
		{
			Func(Rear.GetPtr(), Len);
		}
		else
		{
			Func(Rear.GetPtr(), MaxLen - Rear.Offset);
			Func(pBuffer, Rear.Offset + Len - MaxLen);
		}

		Rear += Len;

		return Len;
	}

	template<class Type, class Alloc>
	void RingBuffer<Type, Alloc>::CopyPart(const RingBuffer& RB) noexcept
	{
		Head.Offset = RB.Head.Offset;
		Head.Cycle = RB.Head.Cycle;
		Rear.Offset = RB.Rear.Offset;
		Rear.Cycle = RB.Rear.Cycle;
		ReadMode = RB.ReadMode;
		WriteMode = RB.WriteMode;
		MaxLen = RB.MaxLen;
	}
}
