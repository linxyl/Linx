#include "Linx/IO/File.h"

#include <iostream>

#include "Linx/System/Time.h"
#include "Linx/Utils/Template.h"

using namespace Linx;
using namespace std;

FileBuf* FileBuf::Open()
{
	WrittenLen = 0;
	LastMilliSeconds = GetTotalMilliSeconds();

	return static_cast<FileBuf*>(Super::open(GetTimeString(Filename), Mode));
}

int FileBuf::sync()
{
	FlushBuffer();
	return Super::sync();
}

int FileBuf::FlushBuffer()
{
	assert(IsOpen());

	if (ShouldRotate())
	{
		Rotate();
	}

	int Count = pptr() - pbase();
	WrittenLen += Count;

	return 0;
}

void FileBuf::Rotate() noexcept
{
	Close();
	Open();
}

bool FileBuf::ShouldRotate() const noexcept
{
	if (SplitSize > 0 && WrittenLen >= SplitSize)
	{
		return true;
	}
	// Time alignment
	else if (SplitMilliSeconds > 0 && GetTotalMilliSeconds() / SplitMilliSeconds > LastMilliSeconds / SplitMilliSeconds)
	{
		return true;
	}
	return false;
}

std::streamsize FileBuf::xsputn(const char* Ptr, std::streamsize Count)
{
	if (ShouldRotate())
	{
		Rotate();
	}

	auto Size = Super::xsputn(Ptr, Count);
	WrittenLen += Size;

	return Size;
}

File::File(const string& InFilename, std::ios_base::openmode Mode) :
	Super(addressof(Buf))
{
	Open(InFilename);
}

bool File::Open(const std::string& InFilename, std::ios_base::openmode Mode)
{
	Buf.Filename = InFilename;
	Buf.Mode = Mode;
	if (!Buf.Open())
	{
		setstate(ios_base::failbit);
		return false;
	}
	setstate(ios_base::goodbit);
	return true;
}

std::streamsize File::Read(char* Ptr, std::streamsize Count)
{
	if (Count < 0)
	{
		setstate(ios_base::badbit);
		return -1;
	}

	ios_base::iostate State = ios_base::goodbit;
	std::streamsize Res = -1;
	try
	{
		std::streamsize Res = rdbuf()->sgetn(Ptr, Count);
		if (Res != Count)
		{
			State |= ios_base::badbit;
		}
	}
	catch (...)
	{
		setstate(ios_base::badbit);
	}
	setstate(State);

	return Res;

}

std::streamsize File::Write(char* Ptr, std::streamsize Count)
{
	if (Count < 0)
	{
		setstate(ios_base::badbit);
		return -1;
	}

	ios_base::iostate State = ios_base::goodbit;
	std::streamsize Res = -1;
	try
	{
		std::streamsize Res = rdbuf()->sputn(Ptr, Count);
		if (Res != Count)
		{
			State |= ios_base::badbit;
		}
	}
	catch (...)
	{
		setstate(ios_base::badbit);
	}
	setstate(State);

	return Res;
}
