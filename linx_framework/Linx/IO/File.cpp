#include "File.h"
#include "Linx/System/Time.h"

#ifdef _WIN32
bool Linx::File::Open() noexcept
{
	size_t AccessMode = 0;
	if (FileFlag & EFileFlag::EWrite)
		AccessMode |= GENERIC_WRITE;
	if (FileFlag & EFileFlag::ERead)
		AccessMode |= GENERIC_READ;
	if (FileFlag & EFileFlag::EAppend)
		AccessMode |= FILE_APPEND_DATA;

	size_t CreateMode = 0;
	if (FileFlag & EFileFlag::ECreate)
	{
		if (FileFlag & EFileFlag::ETruncate)
			CreateMode = CREATE_ALWAYS;
		else if (FileFlag & EFileFlag::EOpen)
			CreateMode = OPEN_ALWAYS;
		else
			CreateMode = CREATE_NEW;
	}
	else
	{
		if (FileFlag & EFileFlag::ETruncate)
			CreateMode = TRUNCATE_EXISTING;
		else
			CreateMode = OPEN_EXISTING;
	}

	Handle = CreateFile(GetTimeString(Filename).c_str(), AccessMode, 0, nullptr, CreateMode, FILE_ATTRIBUTE_NORMAL, 0);

	return Handle != INVALID_HANDLE_VALUE;
}

bool Linx::File::Open(const std::string& InFilename, uint32_t InFlag) noexcept
{
	Filename = InFilename;
	FileFlag = InFlag;
	return Open();
}

bool Linx::File::Open(const char* InFilename, uint32_t InFlag) noexcept
{
	Filename = InFilename;
	FileFlag = InFlag;
	return Open();
}

bool Linx::File::IsOpen() const noexcept
{
	return Handle != INVALID_HANDLE_VALUE;
}

long Linx::File::Read(void* Buf, size_t BufSize) noexcept
{
	if (ShouldRotate())
	{
		Rotate();
	}

	DWORD BytesRead;
	ReadFile(Handle, Buf, BufSize, &BytesRead, nullptr);
	return BytesRead;
}

long Linx::File::Write(const void* Buf, size_t BufSize) noexcept
{
	if (ShouldRotate())
	{
		Rotate();
	}

	DWORD BytesWrite;
	WriteFile(Handle, Buf, BufSize, &BytesWrite, nullptr);
	return BytesWrite;
}

size_t Linx::File::SeekBegin(long Offset) const noexcept
{
	return SetFilePointer(Handle, Offset, nullptr, FILE_BEGIN);
}

size_t Linx::File::SeekCurrent(long Offset) const noexcept
{
	return SetFilePointer(Handle, Offset, nullptr, FILE_CURRENT);
}

size_t Linx::File::SeekEnd(long Offset) const noexcept
{
	return SetFilePointer(Handle, Offset, nullptr, FILE_END);
}

void Linx::File::Close() noexcept
{
	if (INVALID_HANDLE_VALUE != Handle)
	{
		CloseHandle(Handle);
		Handle = INVALID_HANDLE_VALUE;
	}
}

void Linx::File::Rotate() noexcept
{
	Close();
	Open();
}

bool Linx::File::ShouldRotate() const noexcept
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

#else
#endif