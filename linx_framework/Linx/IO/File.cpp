#ifndef _WIN32
#include <fcntl.h>
#include <unistd.h>
#endif
#include "File.h"
#include "Linx/System/Time.h"

#ifdef _WIN32
bool Linx::File::Open() noexcept
{
	Close();

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

	WrittenLen = 0;
	LastMilliSeconds = GetTotalMilliSeconds();

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
	if (!ReadFile(Handle, Buf, BufSize, &BytesRead, nullptr))
		return -1;
	return BytesRead;
}

long Linx::File::Write(const void* Buf, size_t BufSize) noexcept
{
	if (ShouldRotate())
	{
		Rotate();
	}

	DWORD BytesWrite;
	if (!WriteFile(Handle, Buf, BufSize, &BytesWrite, nullptr))
		return -1;
	return BytesWrite;
}

long Linx::File::SeekBegin(long Offset) const noexcept
{
	return SetFilePointer(Handle, Offset, nullptr, FILE_BEGIN);
}

long Linx::File::SeekCurrent(long Offset) const noexcept
{
	return SetFilePointer(Handle, Offset, nullptr, FILE_CURRENT);
}

long Linx::File::SeekEnd(long Offset) const noexcept
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

bool Linx::File::Open() noexcept
{
	Close();

	int Mode = 0;
	if ((FileFlag & EFileFlag::EWrite) && !(FileFlag & EFileFlag::ERead))
		Mode |= O_WRONLY;
	else if (!(FileFlag & EFileFlag::EWrite) && (FileFlag & EFileFlag::ERead))
		Mode |= O_RDONLY;
	if ((FileFlag & EFileFlag::EWrite) && (FileFlag & EFileFlag::ERead))
		Mode |= O_RDWR;

	if(FileFlag & EFileFlag::ECreate)
		Mode |= O_CREAT;
	if(!(FileFlag & EFileFlag::EOpen))
		Mode |= O_EXCL;
	if(FileFlag & EFileFlag::EAppend)
		Mode |= O_APPEND;

    Handle = open(GetTimeString(Filename).c_str(), O_RDWR | O_CREAT, 0666);

	WrittenLen = 0;
	LastMilliSeconds = GetTotalMilliSeconds();

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

	return read(Handle, Buf, BufSize);
}

long Linx::File::Write(const void* Buf, size_t BufSize) noexcept
{
	if (ShouldRotate())
	{
		Rotate();
	}

	return write(Handle, Buf, BufSize);
}

long Linx::File::SeekBegin(long Offset) const noexcept
{
	return lseek(Handle, Offset, SEEK_SET);
}

long Linx::File::SeekCurrent(long Offset) const noexcept
{
	return lseek(Handle, Offset, SEEK_CUR);
}

long Linx::File::SeekEnd(long Offset) const noexcept
{
	return lseek(Handle, Offset, SEEK_END);
}

void Linx::File::Close() noexcept
{
	if (INVALID_HANDLE_VALUE != Handle)
	{
		close(Handle);
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

#endif
