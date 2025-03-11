#include "Logger.h"

using namespace Linx;
using namespace std;

std::streamsize LoggerBuf::xsputn(const char* Ptr, std::streamsize Count)
{
	if (CurrentLevel < LogLevel)
	{
		return std::streambuf::xsputn(Ptr, Count);
	}
	return Super::xsputn(Ptr, Count);
}

int LoggerBuf::sync()
{
	auto Ret = Super::sync();
	Unlock();
	return Ret;
}

int LoggerBuf::FlushWriteBuffer()
{
	if (CurrentLevel < LogLevel)
	{
		ClearWriteBuffer();
		return 0;
	}
	return Super::FlushWriteBuffer();
}

void LoggerBuf::ClearWriteBuffer()
{
	Super::ClearWriteBuffer();
}