#pragma once

#include <string.h>
#include <iostream>

namespace Linx
{
	template<class IOType>
	class StreambufBase : public std::streambuf
	{
	public:
		using Super = std::streambuf;
		static constexpr size_t BufSize = 1024;
		static constexpr size_t ReserveSize = 4;

	public:
		StreambufBase(IOType* InIO);
		virtual ~StreambufBase() { sync(); }

	protected:
		// Begin std::basic_streambuf Interface.
		virtual std::streambuf::int_type overflow(int_type c) override;
		virtual std::streambuf::int_type underflow() override;
		virtual std::streamsize xsgetn(char* Ptr, std::streamsize Count) override;
		virtual std::streamsize xsputn(const char* Ptr, std::streamsize Count) override;
		virtual int sync() override;
		// End std::basic_streambuf Interface.

	protected:
		virtual int FlushWriteBuffer();

		virtual void ClearWriteBuffer();

		char ReadBuf[BufSize];
		char WriteBuf[BufSize];

		IOType* IO;
	};


	template<class IOType>
	StreambufBase<IOType>::StreambufBase(IOType* InIO) :
		IO(InIO)
	{
		// One character less to let the BufSize-th character cause a call of overflow()
		setg(ReadBuf + ReserveSize, ReadBuf + ReserveSize, ReadBuf + ReserveSize);
		setp(WriteBuf, WriteBuf + (BufSize - 1));
	}

	template<class IOType>
	std::streambuf::int_type StreambufBase<IOType>::overflow(int_type c)
	{
		if (c != traits_type::eof())
		{
			*pptr() = c;
			pbump(1);
		}

		if (FlushWriteBuffer() == traits_type::eof())
		{
			return traits_type::eof();
		}
		return c;
	}

	template<class IOType>
	std::streambuf::int_type StreambufBase<IOType>::underflow()
	{
		// Is read position before end of buffer?
		if (gptr() < egptr())
		{
			return traits_type::to_int_type(*gptr());
		}

		int NumPutback = gptr() - eback();
		if (NumPutback > (int)ReserveSize)
		{
			NumPutback = ReserveSize;
		}

		memmove(ReadBuf + (ReserveSize - NumPutback), gptr() - NumPutback, NumPutback);

		// Read new characters
		auto Num = sgetn(ReadBuf + ReserveSize, BufSize - 4);
		if (Num <= 0)
		{
			return traits_type::eof();
		}

		// Reset ReadBuf pointers
		setg(ReadBuf + (ReserveSize - NumPutback), ReadBuf + ReserveSize, ReadBuf + ReserveSize + Num);

		// Return next character
		return traits_type::to_int_type(*gptr());
	}

	template<class IOType>
	std::streamsize Linx::StreambufBase<IOType>::xsgetn(char* Ptr, std::streamsize Count)
	{
		return IO->Read(Ptr, Count);
	}

	template<class IOType>
	std::streamsize Linx::StreambufBase<IOType>::xsputn(const char* Ptr, std::streamsize Count)
	{
		FlushWriteBuffer();
		return IO->Write(Ptr, Count);
	}

	template<class IOType>
	int StreambufBase<IOType>::sync()
	{
		if (traits_type::eof() == FlushWriteBuffer())
		{
			return 0;
		}
		return Super::sync();
	}

	template<class IOType>
	int Linx::StreambufBase<IOType>::FlushWriteBuffer()
	{
		int Count = pptr() - pbase();

		if (IO->Write(WriteBuf, Count) <= 0)
		{
			return traits_type::eof();
		}

		ClearWriteBuffer();
		return 0;
	}

	template<class IOType>
	void Linx::StreambufBase<IOType>::ClearWriteBuffer()
	{
		pbump(pbase() - pptr());
	}
}
