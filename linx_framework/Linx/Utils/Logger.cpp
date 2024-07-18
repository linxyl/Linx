#include "Logger.h"

#include <iostream>

#include "Linx/System/Time.h"
#include "Linx/Utils/Template.h"

using namespace Linx;
using namespace std;

LoggerBuf::LoggerBuf()
{
	// One character less to let the BufSize-th character cause a call of overflow()
	setp(Buffer, Buffer + (BufSize - 1));
}

LoggerBuf* LoggerBuf::Open()
{
	WrittenLen = 0;
	LastMilliSeconds = GetTotalMilliSeconds();

	File = fopen((MainFilename + GetTimeString(".%Y-%m-%d_%H-%M-%S.log")).c_str(), "a");
	if(File)
	{
		return this;
	}
	else
	{
		return nullptr;
	}
}

LoggerBuf::int_type LoggerBuf::overflow(int_type c)
{
	if (c != char_traits<char>::eof())
	{
		*pptr() = c;
		pbump(1);
	}

	if (FlushBuffer() == char_traits<char>::eof())
	{
		return char_traits<char>::eof();
	}
	return c;
}

int LoggerBuf::sync()
{
	if (char_traits<char>::eof() == FlushBuffer())
	{
		return -1;
	}
	return 0;
}

int LoggerBuf::FlushBuffer()
{
	if (CurrentLevel < LogLevel)
	{
		ClearBuffer();
		return 0;
	}

	assert(IsOpen());

	int Count = pptr() - pbase();
	std::string Time = GetTimeString("[%Y-%m-%d %H:%M:%S.%s] ");
	
	// Output in console
	if (bPrintable)
	{
		(cout << Time).write(Buffer, Count);
		cout.flush();
	}

	// Write to file
	if (Time.size() != fwrite(Time.c_str(), sizeof(decltype(Time)::size_type), Time.size(), File))
	{
		return char_traits<char>::eof();
	}
	if ((size_t)Count != fwrite(Buffer, sizeof(Buffer[0]), Count, File))
	{
		return char_traits<char>::eof();
	}
	fflush(File);

	WrittenLen += (Time.size() + Count);

	// Log rotate
	if (ShouldRotate())
	{
		Rotate();
	}

	ClearBuffer();
	return 0;
}

void LoggerBuf::ClearBuffer()
{
	pbump(pbase() - pptr());
	Unlock();
}

void LoggerBuf::Rotate() noexcept
{
	Close();
	Open();
}

bool LoggerBuf::ShouldRotate() const noexcept
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

Logger::Logger(const string& InFilename, ELogLevel::Type InLevel) :
	Super(addressof(Buf))
{
	Open(InFilename);
	Buf.SetLogLevel(InLevel);
}

void Logger::Open(const std::string& InFilename)
{
	Buf.MainFilename = InFilename;
	if (!Buf.Open())
	{
		setstate(ios_base::failbit);
		throw std::logic_error("Open log file failed: " + string(InFilename));
	}
	setstate(ios_base::goodbit);
}

void Logger::Close() noexcept
{
	Buf.Close();
}
