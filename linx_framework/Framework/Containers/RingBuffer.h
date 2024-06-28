#pragma once

namespace linx
{
	template<class Type> 
	class RingBuffer
	{
<<<<<<< HEAD
	public:
		using value_type = Type;
		using size_type = long long;
		static const int kValueTypeSize = sizeof(value_type);

		RingBuffer() noexcept:
			_Buffer(nullptr),
			_MaxLen(0),
			_Head(0),
			_Rear(0),
			_ReadMode(E_READ_ALL),
			_WriteMode(E_FILL),
			_bIsEmpty(true),
			_bIsFull(false)
		{
		}

		explicit RingBuffer(size_type Len) noexcept:
			RingBuffer()
		{
			_Buffer = new Type[Len];
			_MaxLen = Len;
		}

		RingBuffer(const RingBuffer& RB) noexcept
=======
	public:
		using value_type = Type;
		using size_type = long long;
		static const int kValueTypeSize = sizeof(value_type);

		RingBuffer() noexcept:
			_Buffer(nullptr),
			_MaxLen(0),
			_Head(0),
			_Rear(0),
			_ReadMode(E_READ_ALL),
			_WriteMode(E_FILL),
			_bIsEmpty(true),
			_bIsFull(false)
		{
		}

		explicit RingBuffer(size_type Len) noexcept:
			RingBuffer()
		{
			_Buffer = new Type[Len];
			_MaxLen = Len;
		}

		RingBuffer(const RingBuffer& RB) noexcept
		{
			_Buffer = new Type[RB._MaxLen];
			_MaxLen = RB._MaxLen;
			memcpy(_Buffer, RB._Buffer, kValueTypeSize * _MaxLen);
			PartCopy(RB);
		}

		RingBuffer(RingBuffer&& RB) noexcept
		{
			_Buffer = RB._Buffer;
			RB._Buffer = nullptr;
			_MaxLen = RB._MaxLen;
			PartCopy(RB);
		}

		~RingBuffer() noexcept
		{
			delete[] _Buffer;
			_Buffer = nullptr;
		}

	public:
		/**
		 * do what when data length is less than read length
		 */
		enum E_READ_EMPTY_MODE : char
		{
			/** Do noting */
			E_READ_NOTHING = 0,

			/** Read remaining data */
			E_READ_ALL
		};

		/**
		 * Do what when write length is more than remaining length
		 */
		enum E_WRITE_FULL_MODE : char
		{
			/** Do nothing */
			E_WRITE_NOTHING = 0,

			/** Cover the data from the position of Front, and set Front to Rear */
			E_COVER,

			/** Write data to the remaining space */
			E_FILL
		};

	public:
		/** Read data from RingBuffer to DstPtr memory */
		size_type Read(Type* DstPtr, size_type Len, bool bIsConst = false) noexcept
		{
			size_type Ret = 0;
			do
			{
				if (Len <= 0)
				{
					break;
				}

				if (GetDataLen() >= Len)
				{
					Ret = RingRead(DstPtr, Len, bIsConst);
				}
				else
				{
					switch (_ReadMode)
					{
					case E_READ_NOTHING:
						break;
					case E_READ_ALL:
						Ret = RingRead(DstPtr, GetDataLen(), bIsConst);
						break;
					default:
						break;
					}
				}
			} while (false);

			return Ret;
		}

		/** Read data from this RingBuffer to DstRingBuffer */
		size_type Read(RingBuffer<Type>& DstRingBuffer, size_type Len, bool bIsConst = false) noexcept
		{
			size_type Ret = 0;
			size_type TransLen = 0;
			do
			{
				if (Len <= 0)
				{
					break;
				}

				if (GetDataLen() >= Len)
				{
					TransLen = Len;
				}
				else
				{
					switch (_ReadMode)
					{
					case E_READ_NOTHING:
						TransLen = 0;
						break;
					case E_READ_ALL:
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
					switch (DstRingBuffer._WriteMode)
					{
					case E_WRITE_NOTHING:
						TransLen = 0;
						break;
					case E_COVER:
						bCoverFlag = true;
						break;
					case E_FILL:
						TransLen = DstRingBuffer.GetRemainLen();
						break;
					default:
						TransLen = 0;
						break;
					}
				}

				Ret = RingRead(DstRingBuffer, TransLen, bIsConst);
				if (bCoverFlag)
				{
					DstRingBuffer._Head = DstRingBuffer._Rear;
				}
			} while (false);

			return Ret;
		}

		/** Write data from SrcPtr memory to RingBuffer */
		size_type Write(const Type* SrcPtr, size_type Len) noexcept
		{
			size_type Ret = 0;
			do
			{
				if (Len <= 0)
				{
					break;
				}

				if (GetRemainLen() >= Len)
				{
					Ret = RingWrite(SrcPtr, Len);
					break;
				}
				else
				{
					switch (_WriteMode)
					{
					case E_WRITE_NOTHING:
						break;
					case E_COVER:
						Ret = RingWrite(SrcPtr, Len);
						break;
					case E_FILL:
						Ret = RingWrite(SrcPtr, GetRemainLen());
						break;
					default:
						break;
					}
				}
			} while (false);

			return Ret;
		}

		/** Write data from SrcRingBuffer to this RingBuffer */
		size_type Write(RingBuffer<Type>& SrcRingBuffer, size_type Len) noexcept
		{
			size_type Ret = 0;
			size_type TransLen = 0;
			do
			{
				if (Len <= 0)
				{
					break;
				}

				if (SrcRingBuffer.GetDataLen() >= Len)
				{
					TransLen = Len;
					break;
				}
				else
				{
					switch (SrcRingBuffer._ReadMode)
					{
					case E_READ_NOTHING:
						TransLen = 0;
						break;
					case E_READ_ALL:
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
					switch (_WriteMode)
					{
					case E_WRITE_NOTHING:
						TransLen = 0;
						break;
					case E_COVER:
						bCoverFlag = true;
						break;
					case E_FILL:
						TransLen = GetRemainLen();
						break;
					default:
						TransLen = 0;
						break;
					}
				}

				Ret = RingWrite(SrcRingBuffer, TransLen);
				if (bCoverFlag)
				{
					_Head = _Rear;
				}
			} while (false);

			return Ret;
		}

		/** Set the length of RingBuffer */
		void ReallocBuffer(size_type Len) noexcept
		{
			delete[] _Buffer;
			_Buffer = new Type[Len];

			_MaxLen = Len;
		}

	public:
		RingBuffer& operator=(const RingBuffer& RB) noexcept
		{
			delete[] _Buffer;
			_Buffer = new Type[RB._MaxLen];
			_MaxLen = RB._MaxLen;
			memcpy(_Buffer, RB._Buffer, kValueTypeSize * _MaxLen);
			PartCopy(RB);

			return *this;
		}

		Type& operator[](size_type Pos) noexcept
		{
			size_type RealPos = (Pos + _Head) % _MaxLen;

			return _Buffer[RealPos];
		}

		const Type& operator[](size_type Pos) const noexcept
		{
			size_type RealPos = (Pos + _Head) % _MaxLen;

			return _Buffer[RealPos];
		}

	public:
		size_type GetDataLen() const noexcept
		{
			register size_type Len = _Rear - _Head;

			if (0 == Len && _bIsFull)
			{
				Len = _MaxLen;
			}
			else if (Len < 0)
			{
				Len += _MaxLen;
			}

			return Len;
		}

		size_type GetRemainLen() const noexcept
		{
			register size_type Len = _Head - _Rear;

			if (_bIsFull)
			{
				Len = 0;
			}
			else if (Len <= 0)
			{
				Len += _MaxLen;
			}

			return Len;
		}

		size_type GetMaxLen() const noexcept
		{
			return _MaxLen;
		}

		size_type GetHead() const noexcept
		{
			return _Head;
		}

		size_type GetRear() const noexcept
		{
			return _Rear;
		}

		value_type* GetBuffer() const noexcept
		{
			return _Buffer;
		}

		E_READ_EMPTY_MODE GetReadMode() const noexcept
		{
			return _ReadMode;
		}

		E_WRITE_FULL_MODE GetWriteMode() const noexcept
		{
			return _WriteMode;
		}

		void SetReadMode(E_READ_EMPTY_MODE Mode) noexcept
		{
			_ReadMode = Mode;
		}

		void SetWriteMode(E_WRITE_FULL_MODE Mode) noexcept
		{
			_WriteMode = Mode;
		}

		bool GetFullFlag() const noexcept
		{
			return _bIsFull;
		}

		bool GetEmptyFlag() const noexcept
		{
			return _bIsEmpty;
		}

	protected:
		Type* _Buffer;
		size_type _MaxLen;

		size_type _Head;
		size_type _Rear;

		E_READ_EMPTY_MODE _ReadMode;
		E_WRITE_FULL_MODE _WriteMode;

		bool _bIsFull;
		bool _bIsEmpty;
	protected:
		size_type RingRead(value_type* DstPtr, size_type Len, bool bIsConst) noexcept
>>>>>>> parent of 8d3c8af (pointer iterator)
		{
			_Buffer = new Type[RB._MaxLen];
			_MaxLen = RB._MaxLen;
			memcpy(_Buffer, RB._Buffer, kValueTypeSize * _MaxLen);
			PartCopy(RB);
		}

<<<<<<< HEAD
		RingBuffer(RingBuffer&& RB) noexcept
		{
			_Buffer = RB._Buffer;
			RB._Buffer = nullptr;
			_MaxLen = RB._MaxLen;
			PartCopy(RB);
		}

		~RingBuffer() noexcept
		{
			delete[] _Buffer;
			_Buffer = nullptr;
		}

	public:
		/**
		 * do what when data length is less than read length
		 */
		enum E_READ_EMPTY_MODE : char
		{
			/** Do noting */
			E_READ_NOTHING = 0,

			/** Read remaining data */
			E_READ_ALL
		};
=======
			if (!bIsConst)
			{
				_bIsFull = false;
				_bIsEmpty = (Len >= GetDataLen());
			}

			if (_Head + Len < _MaxLen)
			{
				memcpy(DstPtr, _Buffer + _Head, kValueTypeSize * Len);
				if (!bIsConst)
					_Head += Len;
			}
			else
			{
				memcpy(DstPtr, _Buffer + _Head, kValueTypeSize * (_MaxLen - _Head));
				memcpy(DstPtr + _MaxLen - _Head, _Buffer, kValueTypeSize * (_Head + Len - _MaxLen));
				if (!bIsConst)
					_Head += Len - _MaxLen;
			}

			return Len;
		}

		size_type RingRead(RingBuffer& DstRingBuffer, size_type Len, bool bIsConst) noexcept
		{
			if (Len <= 0)
			{
				return 0;
			}

			if (!bIsConst)
			{
				this->_bIsFull = false;
				this->_bIsEmpty = (Len >= this->GetDataLen());
			}
			DstRingBuffer._bIsEmpty = false;
			DstRingBuffer._bIsFull = (Len >= DstRingBuffer.GetRemainLen());

			size_type SrcSplitLen = Len;
			size_type DstSplitLen = Len;
			if (this->_Head + Len >= this->_MaxLen)
			{
				SrcSplitLen = this->_MaxLen - this->_Head;
			}
			if (DstRingBuffer._Rear + Len >= DstRingBuffer._MaxLen)
			{
				DstSplitLen = DstRingBuffer._MaxLen - DstRingBuffer._Rear;
			}
>>>>>>> parent of 8d3c8af (pointer iterator)

			if (SrcSplitLen < DstSplitLen)
			{
				memcpy(DstRingBuffer._Buffer + DstRingBuffer._Rear, this->_Buffer + this->_Head, kValueTypeSize * SrcSplitLen);
				memcpy(DstRingBuffer._Buffer + DstRingBuffer._Rear + SrcSplitLen, this->_Buffer, kValueTypeSize * (DstSplitLen - SrcSplitLen));
				memcpy(DstRingBuffer._Buffer, this->_Buffer + DstSplitLen - SrcSplitLen, kValueTypeSize * (Len - DstSplitLen));
			
			}
			else
			{
				memcpy(DstRingBuffer._Buffer + DstRingBuffer._Rear, this->_Buffer + this->_Head, kValueTypeSize * DstSplitLen);
				memcpy(DstRingBuffer._Buffer, this->_Buffer + this->_Head + DstSplitLen, kValueTypeSize * (SrcSplitLen - DstSplitLen));
				memcpy(DstRingBuffer._Buffer + SrcSplitLen - DstSplitLen, this->_Buffer, kValueTypeSize * (Len - SrcSplitLen));
			}
			
			if (!bIsConst)
			{
				if (Len == SrcSplitLen)
					this->_Head = (this->_Head + Len) % this->_MaxLen;
				else
					this->_Head = Len - SrcSplitLen;
			}

			if (Len == DstSplitLen)
				DstRingBuffer._Rear = (DstRingBuffer._Rear + Len) % DstRingBuffer._MaxLen;
			else
				DstRingBuffer._Rear = Len - DstSplitLen;


			if (DstRingBuffer._bIsFull)
			{
				DstRingBuffer._Head = DstRingBuffer._Rear;
			}

			return Len;
		}

		size_type RingWrite(const value_type* SrcPtr, size_type Len) noexcept
		{
			if (Len <= 0)
			{
				return 0;
			}

			_bIsEmpty = false;
			_bIsFull = (Len >= GetRemainLen());

			if (_Rear + Len < _MaxLen)
			{
				memcpy(_Buffer + _Rear, SrcPtr, kValueTypeSize * Len);
				_Rear += Len;
			}
			else
			{
				memcpy(_Buffer + _Rear, SrcPtr, kValueTypeSize * (_MaxLen - _Rear));
				memcpy(_Buffer, SrcPtr + _MaxLen - _Rear, kValueTypeSize * (_Rear + Len - _MaxLen));
				_Rear += Len - _MaxLen;
			}

			if (_bIsFull)
			{
				_Head = _Rear;
			}

			return Len;
		}

		size_type RingWrite(RingBuffer& SrcRingBuffer, size_type Len) noexcept
		{
			if (Len <= 0)
			{
				return 0;
			}

			this->_bIsEmpty = false;
			this->_bIsFull = (Len >= this->GetRemainLen());
			SrcRingBuffer._bIsFull = false;
			SrcRingBuffer._bIsEmpty = (Len >= SrcRingBuffer.GetDataLen());

			size_type DstSplitLen = Len;
			size_type SrcSplitLen = Len;
			if (this->_Rear + Len >= this->_MaxLen)
			{
				DstSplitLen = this->_MaxLen - this->_Rear;
			}
			if (SrcRingBuffer._Head + Len >= SrcRingBuffer._MaxLen)
			{
				SrcSplitLen = SrcRingBuffer._MaxLen - SrcRingBuffer._Head;
			}

			if (SrcSplitLen < DstSplitLen)
			{
				memcpy(this->_Buffer + this->_Rear, SrcRingBuffer._Buffer + SrcRingBuffer._Head, kValueTypeSize * SrcSplitLen);
				memcpy(this->_Buffer + this->_Rear + SrcSplitLen, SrcRingBuffer._Buffer, kValueTypeSize * (DstSplitLen - SrcSplitLen));
				memcpy(this->_Buffer, SrcRingBuffer._Buffer + DstSplitLen - SrcSplitLen, kValueTypeSize * (Len - DstSplitLen));
			}
			else
			{
				memcpy(this->_Buffer + this->_Rear, SrcRingBuffer._Buffer + SrcRingBuffer._Head, kValueTypeSize * DstSplitLen);
				memcpy(this->_Buffer, SrcRingBuffer._Buffer + SrcRingBuffer._Head + DstSplitLen, kValueTypeSize * (SrcSplitLen - DstSplitLen));
				memcpy(this->_Buffer + SrcSplitLen - DstSplitLen, SrcRingBuffer._Buffer, kValueTypeSize * (Len - SrcSplitLen));
			}

			if (Len == SrcSplitLen)
				SrcRingBuffer._Head = (SrcRingBuffer._Head + Len) % SrcRingBuffer._MaxLen;
			else
				SrcRingBuffer._Head = Len - SrcSplitLen;

			if (Len == DstSplitLen)
				this->_Rear = (this->_Rear + Len) % this->_MaxLen;
			else
				this->_Rear = Len - DstSplitLen;

			if (this->_bIsFull)
			{
				this->_Head = this->_Rear;
			}

			return Len;
		}

	private:
		void PartCopy(const RingBuffer& RB) noexcept
		{
			_Head = RB._Head;
			_Rear = RB._Rear;
			_ReadMode = RB._ReadMode;
			_WriteMode = RB._WriteMode;
			_bIsEmpty = RB._bIsEmpty;
			_bIsFull = RB._bIsFull;
		}
	};

<<<<<<< HEAD
	public:
		/** Read data from RingBuffer to DstPtr memory */
		size_type Read(Type* DstPtr, size_type Len, bool bIsConst = false) noexcept
		{
			size_type Ret = 0;
			do
			{
				if (Len <= 0)
				{
					break;
				}

				if (GetDataLen() >= Len)
				{
					Ret = RingRead(DstPtr, Len, bIsConst);
				}
				else
				{
					switch (_ReadMode)
					{
					case E_READ_NOTHING:
						break;
					case E_READ_ALL:
						Ret = RingRead(DstPtr, GetDataLen(), bIsConst);
						break;
					default:
						break;
					}
				}
			} while (false);

			return Ret;
		}

		/** Read data from this RingBuffer to DstRingBuffer */
		size_type Read(RingBuffer<Type>& DstRingBuffer, size_type Len, bool bIsConst = false) noexcept
		{
			size_type Ret = 0;
			size_type TransLen = 0;
			do
			{
				if (Len <= 0)
				{
					break;
				}

				if (GetDataLen() >= Len)
				{
					TransLen = Len;
				}
				else
				{
					switch (_ReadMode)
					{
					case E_READ_NOTHING:
						TransLen = 0;
						break;
					case E_READ_ALL:
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
					switch (DstRingBuffer._WriteMode)
					{
					case E_WRITE_NOTHING:
						TransLen = 0;
						break;
					case E_COVER:
						bCoverFlag = true;
						break;
					case E_FILL:
						TransLen = DstRingBuffer.GetRemainLen();
						break;
					default:
						TransLen = 0;
						break;
					}
				}

				Ret = RingRead(DstRingBuffer, TransLen, bIsConst);
				if (bCoverFlag)
				{
					DstRingBuffer._Head = DstRingBuffer._Rear;
				}
			} while (false);

			return Ret;
		}

		/** Write data from SrcPtr memory to RingBuffer */
		size_type Write(const Type* SrcPtr, size_type Len) noexcept
		{
			size_type Ret = 0;
			do
			{
				if (Len <= 0)
				{
					break;
				}

				if (GetRemainLen() >= Len)
				{
					Ret = RingWrite(SrcPtr, Len);
					break;
				}
				else
				{
					switch (_WriteMode)
					{
					case E_WRITE_NOTHING:
						break;
					case E_COVER:
						Ret = RingWrite(SrcPtr, Len);
						break;
					case E_FILL:
						Ret = RingWrite(SrcPtr, GetRemainLen());
						break;
					default:
						break;
					}
				}
			} while (false);

			return Ret;
		}

		/** Write data from SrcRingBuffer to this RingBuffer */
		size_type Write(RingBuffer<Type>& SrcRingBuffer, size_type Len) noexcept
		{
			size_type Ret = 0;
			size_type TransLen = 0;
			do
			{
				if (Len <= 0)
				{
					break;
				}

				if (SrcRingBuffer.GetDataLen() >= Len)
				{
					TransLen = Len;
					break;
				}
				else
				{
					switch (SrcRingBuffer._ReadMode)
					{
					case E_READ_NOTHING:
						TransLen = 0;
						break;
					case E_READ_ALL:
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
					switch (_WriteMode)
					{
					case E_WRITE_NOTHING:
						TransLen = 0;
						break;
					case E_COVER:
						bCoverFlag = true;
						break;
					case E_FILL:
						TransLen = GetRemainLen();
						break;
					default:
						TransLen = 0;
						break;
					}
				}

				Ret = RingWrite(SrcRingBuffer, TransLen);
				if (bCoverFlag)
				{
					_Head = _Rear;
				}
			} while (false);

			return Ret;
		}

		/** Set the length of RingBuffer */
		void ReallocBuffer(size_type Len) noexcept
		{
			delete[] _Buffer;
			_Buffer = new Type[Len];

			_MaxLen = Len;
		}

	public:
		RingBuffer& operator=(const RingBuffer& RB) noexcept
		{
			delete[] _Buffer;
			_Buffer = new Type[RB._MaxLen];
			_MaxLen = RB._MaxLen;
			memcpy(_Buffer, RB._Buffer, kValueTypeSize * _MaxLen);
			PartCopy(RB);

			return *this;
		}

		Type& operator[](size_type Pos) noexcept
		{
			size_type RealPos = (Pos + _Head) % _MaxLen;

			return _Buffer[RealPos];
		}

		const Type& operator[](size_type Pos) const noexcept
		{
			size_type RealPos = (Pos + _Head) % _MaxLen;

			return _Buffer[RealPos];
		}

	public:
		size_type GetDataLen() const noexcept
		{
			register size_type Len = _Rear - _Head;

			if (0 == Len && _bIsFull)
			{
				Len = _MaxLen;
			}
			else if (Len < 0)
			{
				Len += _MaxLen;
			}

			return Len;
		}

		size_type GetRemainLen() const noexcept
		{
			register size_type Len = _Head - _Rear;

			if (_bIsFull)
			{
				Len = 0;
			}
			else if (Len <= 0)
			{
				Len += _MaxLen;
			}

			return Len;
		}

		size_type GetMaxLen() const noexcept
		{
			return _MaxLen;
		}

		size_type GetHead() const noexcept
		{
			return _Head;
		}

		size_type GetRear() const noexcept
		{
			return _Rear;
		}

		value_type* GetBuffer() const noexcept
		{
			return _Buffer;
		}

		E_READ_EMPTY_MODE GetReadMode() const noexcept
		{
			return _ReadMode;
		}

		E_WRITE_FULL_MODE GetWriteMode() const noexcept
		{
			return _WriteMode;
		}

		void SetReadMode(E_READ_EMPTY_MODE Mode) noexcept
		{
			_ReadMode = Mode;
		}

		void SetWriteMode(E_WRITE_FULL_MODE Mode) noexcept
		{
			_WriteMode = Mode;
		}

		bool GetFullFlag() const noexcept
		{
			return _bIsFull;
		}

		bool GetEmptyFlag() const noexcept
		{
			return _bIsEmpty;
		}

	protected:
		Type* _Buffer;
		size_type _MaxLen;

		size_type _Head;
		size_type _Rear;

		E_READ_EMPTY_MODE _ReadMode;
		E_WRITE_FULL_MODE _WriteMode;

		bool _bIsFull;
		bool _bIsEmpty;
	protected:
		size_type RingRead(value_type* DstPtr, size_type Len, bool bIsConst) noexcept
		{
			if (Len <= 0)
			{
				return 0;
			}

			if (!bIsConst)
			{
				_bIsFull = false;
				_bIsEmpty = (Len >= GetDataLen());
			}

			if (_Head + Len < _MaxLen)
			{
				memcpy(DstPtr, _Buffer + _Head, kValueTypeSize * Len);
				if (!bIsConst)
					_Head += Len;
			}
			else
			{
				memcpy(DstPtr, _Buffer + _Head, kValueTypeSize * (_MaxLen - _Head));
				memcpy(DstPtr + _MaxLen - _Head, _Buffer, kValueTypeSize * (_Head + Len - _MaxLen));
				if (!bIsConst)
					_Head += Len - _MaxLen;
			}

			return Len;
		}

		size_type RingRead(RingBuffer& DstRingBuffer, size_type Len, bool bIsConst) noexcept
		{
			if (Len <= 0)
			{
				return 0;
			}

			if (!bIsConst)
			{
				this->_bIsFull = false;
				this->_bIsEmpty = (Len >= this->GetDataLen());
			}
			DstRingBuffer._bIsEmpty = false;
			DstRingBuffer._bIsFull = (Len >= DstRingBuffer.GetRemainLen());

			size_type SrcSplitLen = Len;
			size_type DstSplitLen = Len;
			if (this->_Head + Len >= this->_MaxLen)
			{
				SrcSplitLen = this->_MaxLen - this->_Head;
			}
			if (DstRingBuffer._Rear + Len >= DstRingBuffer._MaxLen)
			{
				DstSplitLen = DstRingBuffer._MaxLen - DstRingBuffer._Rear;
			}

			if (SrcSplitLen < DstSplitLen)
			{
				memcpy(DstRingBuffer._Buffer + DstRingBuffer._Rear, this->_Buffer + this->_Head, kValueTypeSize * SrcSplitLen);
				memcpy(DstRingBuffer._Buffer + DstRingBuffer._Rear + SrcSplitLen, this->_Buffer, kValueTypeSize * (DstSplitLen - SrcSplitLen));
				memcpy(DstRingBuffer._Buffer, this->_Buffer + DstSplitLen - SrcSplitLen, kValueTypeSize * (Len - DstSplitLen));
			
			}
			else
			{
				memcpy(DstRingBuffer._Buffer + DstRingBuffer._Rear, this->_Buffer + this->_Head, kValueTypeSize * DstSplitLen);
				memcpy(DstRingBuffer._Buffer, this->_Buffer + this->_Head + DstSplitLen, kValueTypeSize * (SrcSplitLen - DstSplitLen));
				memcpy(DstRingBuffer._Buffer + SrcSplitLen - DstSplitLen, this->_Buffer, kValueTypeSize * (Len - SrcSplitLen));
			}
			
			if (!bIsConst)
			{
				if (Len == SrcSplitLen)
					this->_Head = (this->_Head + Len) % this->_MaxLen;
				else
					this->_Head = Len - SrcSplitLen;
			}

			if (Len == DstSplitLen)
				DstRingBuffer._Rear = (DstRingBuffer._Rear + Len) % DstRingBuffer._MaxLen;
			else
				DstRingBuffer._Rear = Len - DstSplitLen;


			if (DstRingBuffer._bIsFull)
			{
				DstRingBuffer._Head = DstRingBuffer._Rear;
			}

			return Len;
		}

		size_type RingWrite(const value_type* SrcPtr, size_type Len) noexcept
		{
			if (Len <= 0)
			{
				return 0;
			}

			_bIsEmpty = false;
			_bIsFull = (Len >= GetRemainLen());

			if (_Rear + Len < _MaxLen)
			{
				memcpy(_Buffer + _Rear, SrcPtr, kValueTypeSize * Len);
				_Rear += Len;
			}
			else
			{
				memcpy(_Buffer + _Rear, SrcPtr, kValueTypeSize * (_MaxLen - _Rear));
				memcpy(_Buffer, SrcPtr + _MaxLen - _Rear, kValueTypeSize * (_Rear + Len - _MaxLen));
				_Rear += Len - _MaxLen;
			}

			if (_bIsFull)
			{
				_Head = _Rear;
			}

			return Len;
		}

		size_type RingWrite(RingBuffer& SrcRingBuffer, size_type Len) noexcept
		{
			if (Len <= 0)
			{
				return 0;
			}

			this->_bIsEmpty = false;
			this->_bIsFull = (Len >= this->GetRemainLen());
			SrcRingBuffer._bIsFull = false;
			SrcRingBuffer._bIsEmpty = (Len >= SrcRingBuffer.GetDataLen());

			size_type DstSplitLen = Len;
			size_type SrcSplitLen = Len;
			if (this->_Rear + Len >= this->_MaxLen)
			{
				DstSplitLen = this->_MaxLen - this->_Rear;
			}
			if (SrcRingBuffer._Head + Len >= SrcRingBuffer._MaxLen)
			{
				SrcSplitLen = SrcRingBuffer._MaxLen - SrcRingBuffer._Head;
			}

			if (SrcSplitLen < DstSplitLen)
			{
				memcpy(this->_Buffer + this->_Rear, SrcRingBuffer._Buffer + SrcRingBuffer._Head, kValueTypeSize * SrcSplitLen);
				memcpy(this->_Buffer + this->_Rear + SrcSplitLen, SrcRingBuffer._Buffer, kValueTypeSize * (DstSplitLen - SrcSplitLen));
				memcpy(this->_Buffer, SrcRingBuffer._Buffer + DstSplitLen - SrcSplitLen, kValueTypeSize * (Len - DstSplitLen));
			}
			else
			{
				memcpy(this->_Buffer + this->_Rear, SrcRingBuffer._Buffer + SrcRingBuffer._Head, kValueTypeSize * DstSplitLen);
				memcpy(this->_Buffer, SrcRingBuffer._Buffer + SrcRingBuffer._Head + DstSplitLen, kValueTypeSize * (SrcSplitLen - DstSplitLen));
				memcpy(this->_Buffer + SrcSplitLen - DstSplitLen, SrcRingBuffer._Buffer, kValueTypeSize * (Len - SrcSplitLen));
			}

			if (Len == SrcSplitLen)
				SrcRingBuffer._Head = (SrcRingBuffer._Head + Len) % SrcRingBuffer._MaxLen;
			else
				SrcRingBuffer._Head = Len - SrcSplitLen;

			if (Len == DstSplitLen)
				this->_Rear = (this->_Rear + Len) % this->_MaxLen;
			else
				this->_Rear = Len - DstSplitLen;

			if (this->_bIsFull)
			{
				this->_Head = this->_Rear;
			}

			return Len;
		}

	private:
		void PartCopy(const RingBuffer& RB) noexcept
		{
			_Head = RB._Head;
			_Rear = RB._Rear;
			_ReadMode = RB._ReadMode;
			_WriteMode = RB._WriteMode;
			_bIsEmpty = RB._bIsEmpty;
			_bIsFull = RB._bIsFull;
		}
	};

=======
>>>>>>> parent of 8d3c8af (pointer iterator)
}