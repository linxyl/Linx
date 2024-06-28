#pragma once

#include <stdint.h>
#include <xutility>
#include <xmemory>

namespace linx
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

	template<class Type>
	class RingBufferIterator : public std::_Iterator_base 
	{
		template<class Type, class Alloc>
		friend class RingBuffer;

	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type        = typename Type;
		using difference_type   = typename std::ptrdiff_t;
		using pointer           = typename Type*;
		using reference         = value_type&;

		/** RingBuffer's size_type */
		using size_type			= size_t;

	public:
		_inline pointer GetPtr() const noexcept { return Ptr; }

	public:
		__inline RingBufferIterator() noexcept : Ptr() {}

		__inline RingBufferIterator(pointer InPtr) noexcept : Ptr(InPtr) {}

		__inline reference operator*() const noexcept { return *Ptr; }

		__inline pointer operator->() const noexcept { return Ptr; }

		__inline RingBufferIterator& operator++() noexcept {
			++Ptr;
			return *this;
		}

		__inline RingBufferIterator operator++(int) noexcept {
			RingBufferIterator Tmp = *this;
			++*this;
			return Tmp;
		}

		__inline RingBufferIterator& operator--() noexcept {
			--Ptr;
			return *this;
		}

		__inline RingBufferIterator operator--(int) noexcept {
			RingBufferIterator Tmp = *this;
			--*this;
			return Tmp;
		}

		__inline RingBufferIterator& operator+=(const difference_type Off) noexcept {
			Ptr += Off;
			return *this;
		}

		__inline RingBufferIterator operator+(const difference_type Off) const noexcept {
			RingBufferIterator Tmp = *this;
			Tmp += Off;
			return Tmp;
		}

		__inline RingBufferIterator& operator-=(const difference_type Off) noexcept {
			return *this += -Off;
		}

		__inline RingBufferIterator operator-(const difference_type Off) const noexcept {
			RingBufferIterator Tmp = *this;
			Tmp -= Off;
			return Tmp;
		}

		__inline difference_type operator-(const RingBufferIterator& Right) const noexcept {
			return Ptr - Right.Ptr;
		}

		__inline reference operator[](const difference_type Off) const noexcept {
			return *(*this + Off);
		}

		__inline bool operator==(const RingBufferIterator& Right) const noexcept {
			return Ptr == Right.Ptr;
		}

		bool operator!=(const RingBufferIterator& Right) const noexcept {
			return !(*this == Right);
		}

		bool operator<(const RingBufferIterator& Right) const noexcept {
			return Ptr < Right.Ptr;
		}

		bool operator>(const RingBufferIterator& Right) const noexcept {
			return Right < *this;
		}

		bool operator<=(const RingBufferIterator& Right) const noexcept {
			return !(Right < *this);
		}

		bool operator>=(const RingBufferIterator& Right) const noexcept {
			return !(*this < Right);
		}

	private:
		/** pointer to element in RingBuffer */
		pointer Ptr;

		///** The offset of the element in the RingBuffer */
		//size_type Offset;

		/** Length of the buffer */
		size_type MaxLen;
	};


	template<class Type, class Alloc = std::allocator<Type>> 
	class RingBuffer
	{
	public: 
		using value_type		= Type;
		using allocator_type	= Alloc;
		using pointer			= typename Type*;
		using const_pointer		= typename const Type*;
		using reference			= Type&;
		using const_reference	= const Type&;
		using size_type			= size_t;
		using difference_type	= ptrdiff_t;
		
		using iterator               = RingBufferIterator<Type>;
		using const_iterator         = const RingBufferIterator<Type>;
		using reverse_iterator       = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		static constexpr size_t value_size = sizeof(value_type);

	public:
		RingBuffer() noexcept;
		explicit RingBuffer(size_type Len) noexcept;
		RingBuffer(const RingBuffer& RB) noexcept;
		RingBuffer(RingBuffer&& RB) noexcept;
		~RingBuffer();

	public:
		RingBuffer& operator=(const RingBuffer& RB) noexcept;

		/** Get the value from the index starting from the position of reading. */
		Type& operator[](size_type Pos) noexcept;
		const Type& operator[](size_type Pos) const noexcept;

	public:
		/** Read data from RingBuffer to DstPtr memory */
		size_type Read(Type* DstPtr, size_type Len, bool bIsConst = false) noexcept;
		/** Read data from this RingBuffer to DstRingBuffer. */
		size_type Read(RingBuffer<Type, Alloc>& DstRingBuffer, size_type Len, bool bIsConst = false) noexcept;

		/** Write data from SrcPtr memory to RingBuffer. */
		size_type Write(const Type* SrcPtr, size_type Len) noexcept;
		/** Write data from SrcRingBuffer to this RingBuffer. */
		size_type Write(RingBuffer<Type, Alloc>& SrcRingBuffer, size_type Len) noexcept;

		/** Reallocate the length of RingBuffer. */
		void ReallocBuffer(size_type Len) noexcept;

	private:
		/** Pointer to the buffer */
		Type* pBuffer;

		/** Length of the buffer */
		size_type MaxLen;

		/** Offset of the head position in the buffer */
		RingBufferIterator<value_type> Head;

		/** Offset of the rear position in the buffer */
		RingBufferIterator<value_type> Rear;

		/** Whether the buffer is full */
		bool bIsFull;

		/** Whether the buffer is empty */
		bool bIsEmpty;

		/** Used to allocate the buffer */
		allocator_type Allocator;

	public:
		/** Returns data length. */
		size_type GetDataLen() const noexcept;

		/** Returns the length of the remaining space. */
		size_type GetRemainLen() const noexcept;

		/** Returns the max length of the buffer. */
		size_type GetMaxLen() const noexcept { return MaxLen; }

		/** Returns the head position int the buffer. */
		size_type GetHead() const noexcept { return Head; }

		/** Returns the rear position int the buffer. */
		size_type GetRear() const noexcept { return Rear; }

		/** Returns the pointer to the buffer. */
		value_type* GetBuffer() const noexcept { return pBuffer; }

		/** Returns whether the buffer is full. */
		bool GetFullFlag() const noexcept { return bIsFull; }

		/** Returns whether the buffer is empty. */
		bool GetEmptyFlag() const noexcept { return bIsEmpty; }

		ERingBufferReadMode::Mode ReadMode;
		ERingBufferWriteMode::Mode WriteMode;

	public:
		iterator begin() const noexcept { return Head; }
		iterator end() const noexcept { return Rear; }

	private:
		void SetMaxLen(size_type Len)
		{
			MaxLen = Len;
			Head.MaxLen = Len;
			Rear.MaxLen = Len;
		}

		size_type ReadImpl(value_type* DstPtr, size_type Len, bool bIsConst) noexcept
		{
			if (Len <= 0)
			{
				return 0;
			}

			if (!bIsConst)
			{
				bIsFull = false;
				bIsEmpty = (Len >= GetDataLen());
			}

			if (Head - pBuffer + Len < MaxLen)
			{
				memcpy(DstPtr, Head.GetPtr(), value_size * Len);
				if (!bIsConst)
					Head += Len;
			}
			else
			{
				memcpy(DstPtr, Head.GetPtr(), value_size * (MaxLen - (Head - pBuffer)));
				memcpy(DstPtr + MaxLen - (Head - pBuffer), pBuffer, value_size * (Head - pBuffer + Len - MaxLen));
				if (!bIsConst)
					Head += Len - MaxLen;
			}

			return Len;
		}

		size_type ReadImpl(RingBuffer& DstRingBuffer, size_type Len, bool bIsConst) noexcept
		{
			if (Len <= 0)
			{
				return 0;
			}

			if (!bIsConst)
			{
				bIsFull = false;
				bIsEmpty = (Len >= GetDataLen());
			}
			DstRingBuffer.bIsEmpty = false;
			DstRingBuffer.bIsFull = (Len >= DstRingBuffer.GetRemainLen());

			size_type SrcSplitLen = Len;
			size_type DstSplitLen = Len;
			if (Head - pBuffer + Len >= MaxLen)
			{
				SrcSplitLen = MaxLen - (Head - pBuffer);
			}
			if (DstRingBuffer.Rear - DstRingBuffer.pBuffer + Len >= DstRingBuffer.MaxLen)
			{
				DstSplitLen = DstRingBuffer.MaxLen - (DstRingBuffer.Rear - DstRingBuffer.pBuffer);
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
			
			if (!bIsConst)
			{
				if (Len == SrcSplitLen)
					Head += Len;
				else
					Head = pBuffer + Len - SrcSplitLen;
			}

			if (Len == DstSplitLen)
				DstRingBuffer.Rear += Len;
			else
				DstRingBuffer.Rear = pBuffer + Len - DstSplitLen;


			if (DstRingBuffer.bIsFull)
			{
				DstRingBuffer.Head = DstRingBuffer.Rear;
			}

			return Len;
		}

		size_type WriteImpl(const value_type* SrcPtr, size_type Len) noexcept
		{
			if (Len <= 0)
			{
				return 0;
			}

			bIsEmpty = false;
			bIsFull = (Len >= GetRemainLen());

			if (Rear - pBuffer + Len < MaxLen)
			{
				memcpy(Rear.GetPtr(), SrcPtr, value_size * Len);
				Rear += Len;
			}
			else
			{
				memcpy(Rear.GetPtr(), SrcPtr, value_size * (MaxLen - (Rear - pBuffer)));
				memcpy(pBuffer, SrcPtr + MaxLen - (Rear.GetPtr() - pBuffer), value_size * (Rear - pBuffer + Len - MaxLen));
				Rear += Len - MaxLen;
			}

			if (bIsFull)
			{
				Head = Rear;
			}

			return Len;
		}

		size_type WriteImpl(RingBuffer& SrcRingBuffer, size_type Len) noexcept
		{
			if (Len <= 0)
			{
				return 0;
			}

			bIsEmpty = false;
			bIsFull = (Len >= GetRemainLen());
			SrcRingBuffer.bIsFull = false;
			SrcRingBuffer.bIsEmpty = (Len >= SrcRingBuffer.GetDataLen());

			size_type DstSplitLen = Len;
			size_type SrcSplitLen = Len;
			if (Rear - pBuffer + Len >= MaxLen)
			{
				DstSplitLen = MaxLen - (Rear - pBuffer);
			}
			if (SrcRingBuffer.Head - SrcRingBuffer.pBuffer + Len >= SrcRingBuffer.MaxLen)
			{
				SrcSplitLen = SrcRingBuffer.MaxLen - (SrcRingBuffer.Head - SrcRingBuffer.pBuffer);
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

			if (Len == SrcSplitLen)
				SrcRingBuffer.Head = SrcRingBuffer.Head + Len;
			else
				SrcRingBuffer.Head = pBuffer + Len - SrcSplitLen;

			if (Len == DstSplitLen)
				Rear = Rear + Len;
			else
				Rear = pBuffer + Len - DstSplitLen;

			if (bIsFull)
			{
				Head = Rear;
			}

			return Len;
		}

	private:
		void PartCopy(const RingBuffer& RB) noexcept
		{
			Head = RB.Head - RB.pBuffer + pBuffer;
			Rear = RB.Rear - RB.pBuffer + pBuffer;
			ReadMode = RB.ReadMode;
			WriteMode = RB.WriteMode;
			bIsEmpty = RB.bIsEmpty;
			bIsFull = RB.bIsFull;
		}
	};


	template<class Type, class Alloc> 
	RingBuffer<Type, Alloc>::RingBuffer() noexcept :
		pBuffer(nullptr),
		MaxLen(0),
		Head(nullptr),
		Rear(nullptr),
		ReadMode(ERingBufferReadMode::ReadAll),
		WriteMode(ERingBufferWriteMode::Fill),
		bIsEmpty(true),
		bIsFull(false)
	{
	}

	template<class Type, class Alloc> 
	RingBuffer<Type, Alloc>::RingBuffer(size_type Len) noexcept:
		RingBuffer()
	{
		pBuffer = Allocator.allocate(Len);
		Head = pBuffer;
		Rear = pBuffer;
		SetMaxLen(Len);
	}

	template<class Type, class Alloc> 
	RingBuffer<Type, Alloc>::RingBuffer(const RingBuffer& RB) noexcept
	{
		pBuffer = Allocator.allocate(RB.MaxLen);
		SetMaxLen(RB.MaxLen);
		memcpy(pBuffer, RB.pBuffer, value_size * MaxLen);
		PartCopy(RB);
	}

	template<class Type, class Alloc> 
	RingBuffer<Type, Alloc>::RingBuffer(RingBuffer&& RB) noexcept
	{
		pBuffer = RB.pBuffer;
		PartCopy(RB);
		SetMaxLen(RB.MaxLen);
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
	typename RingBuffer<Type, Alloc>::size_type RingBuffer<Type, Alloc>::Read(Type* DstPtr, size_type Len, bool bIsConst) noexcept
	{
		size_type Ret = 0;

		if (Len <= 0)
		{
			return Ret;
		}

		if (GetDataLen() >= Len)
		{
			Ret = ReadImpl(DstPtr, Len, bIsConst);
		}
		else
		{
			switch (ReadMode)
			{
			case ERingBufferReadMode::ReadNothing:
				break;
			case ERingBufferReadMode::ReadAll:
				Ret = ReadImpl(DstPtr, GetDataLen(), bIsConst);
				break;
			default:
				break;
			}
		}

		return Ret;
	}

	template<class Type, class Alloc>
	typename RingBuffer<Type, Alloc>::size_type RingBuffer<Type, Alloc>::Read(RingBuffer<Type, Alloc>& DstRingBuffer, size_type Len, bool bIsConst) noexcept
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

		Ret = ReadImpl(DstRingBuffer, TransLen, bIsConst);
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
	void RingBuffer<Type, Alloc>::ReallocBuffer(size_type Len) noexcept
	{
		delete[] pBuffer;
		pBuffer = new Type[Len];

		MaxLen = Len;
	}

	template<class Type, class Alloc>
	typename RingBuffer<Type, Alloc>::size_type RingBuffer<Type, Alloc>::GetDataLen() const noexcept
	{
		difference_type Len = Rear - Head;

		if (0 == Len && bIsFull)
		{
			Len = MaxLen;
		}
		else if (Len < 0)
		{
			Len += MaxLen;
		}

		return Len;
	}

	template<class Type, class Alloc>
	typename RingBuffer<Type, Alloc>::size_type RingBuffer<Type, Alloc>::GetRemainLen() const noexcept
	{
		difference_type Len = Head - Rear;

		if (bIsFull)
		{
			Len = 0;
		}
		else if (Len <= 0)
		{
			Len += MaxLen;
		}

		return Len;
	}

	template<class Type, class Alloc>
	RingBuffer<Type, Alloc>& RingBuffer<Type, Alloc>::operator=(const RingBuffer& RB) noexcept
	{
		delete[] pBuffer;
		pBuffer = new Type[RB.MaxLen];
		MaxLen = RB.MaxLen;
		memcpy(pBuffer, RB.pBuffer, value_size * MaxLen);
		PartCopy(RB);

		return *this;
	}

	template<class Type, class Alloc>
	typename Type& RingBuffer<Type, Alloc>::operator[](size_type Pos) noexcept
	{
		auto RealPos = Head + Pos;

		return *RealPos;
	}

	template<class Type, class Alloc>
	const typename Type& RingBuffer<Type, Alloc>::operator[](size_type Pos) const noexcept
	{
		auto RealPos = Head + Pos;

		return *RealPos;
	}
}
