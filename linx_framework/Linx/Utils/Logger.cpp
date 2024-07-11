#include "Logger.h"

#include <cassert>

#include <iostream>

#include "System/Time.h"
#include "Utils/Template.h"

using namespace Linx;
using namespace std;

LoggerBuf* LoggerBuf::Open()
{
	WrittenLen = 0;
	LastMilliSeconds = GetTotalMilliSeconds();

	File.open(Filename + GetTimeString(".%Y-%m-%d_%H-%M-%S.log"), ios_base::app | ios_base::out, ios_base::_Default_open_prot);
	if (File.is_open())
		return this;
	else
		return nullptr;
}

void LoggerBuf::Close() noexcept
{
	File.close();
}

int LoggerBuf::sync()
{
	if (CurrentLevel < LogLevel)
	{
		swap(LoggerBuf()); // Clear stream
		return 0;
	}
	assert(IsOpen());

	std::string Time = GetTimeString("[%Y-%m-%d %H:%M:%S.%s] ");
	if (bPrintable)
	{
		cout << Time << str();
		cout.flush();
	}
	File << Time << str();
	File.flush();

	// Log rotate

	auto& s = str();
	WrittenLen += (Time.size() + str().size());
	if (SplitSize > 0 && WrittenLen >= SplitSize)
	{
		Rotate();
	}
	// Time alignment
	else if (SplitMilliSeconds > 0 && GetTotalMilliSeconds() / SplitMilliSeconds > LastMilliSeconds / SplitMilliSeconds)
	{
		Rotate();
	}

	swap(LoggerBuf()); // Clear stream

	return 0;
}

void LoggerBuf::Rotate() noexcept
{
	Close();
	Open();
}


Logger::Logger(const string& InFilename, ELogLevel::Type InLevel) :
	Super(addressof(Buf))
{
	Open(InFilename);
	Buf.LogLevel = InLevel;
}

void Logger::Open(const std::string& InFilename)
{
	Buf.Filename = InFilename;
	if (!Buf.Open())
	{
		throw std::logic_error("Open log file failed: " + string(InFilename));
	}
}

void Logger::Close() noexcept
{
	Buf.Close();
}