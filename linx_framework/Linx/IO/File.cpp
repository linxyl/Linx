#ifndef _WIN32
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#endif
#include "File.h"
#include "Linx/System/Time.h"

#ifdef _WIN32
bool Linx::File::Open() noexcept
{
	Close();

	size_t AccessMode = 0;
	if (FileFlag.bWrite)
		AccessMode |= GENERIC_WRITE;
	if (FileFlag.bRead)
		AccessMode |= GENERIC_READ;
	if (FileFlag.bAppend)
		AccessMode |= FILE_APPEND_DATA;

	size_t CreateMode = 0;
	if (FileFlag.bCreate)
	{
		if (FileFlag.bTruncate)
			CreateMode = CREATE_ALWAYS;
		else if (FileFlag.bOpen)
			CreateMode = OPEN_ALWAYS;
		else
			CreateMode = CREATE_NEW;
	}
	else
	{
		if (FileFlag.bTruncate)
			CreateMode = TRUNCATE_EXISTING;
		else
			CreateMode = OPEN_EXISTING;
	}

	Handle = CreateFile(GetTimeString(Filename).c_str(), AccessMode, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CreateMode, FILE_ATTRIBUTE_NORMAL, 0);

	WrittenLen = 0;
	LastMilliSeconds = GetTotalMilliSeconds();

	return Handle != INVALID_HANDLE_VALUE;
}

bool Linx::File::Open(const std::string& InFilename, FileOpenFlag InFlag) noexcept
{
	Filename = InFilename;
	FileFlag = InFlag;
	return Open();
}

bool Linx::File::Open(const char* InFilename, FileOpenFlag InFlag) noexcept
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
	Super::Write(Buf, BufSize);

	if (ShouldRotate())
	{
		Rotate();
	}

	DWORD BytesWrite;
	if (!WriteFile(Handle, Buf, BufSize, &BytesWrite, nullptr))
		return -1;
	WrittenLen += BytesWrite;
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

char* Linx::File::MemMap(size_t Size, size_t Offset, FileOpenFlag AccessMode)
{

	DWORD Prot;
	DWORD dwDesiredAccess = 0;
	if (AccessMode.bRead)
	{
		Prot = PAGE_READONLY;
		dwDesiredAccess |= FILE_MAP_READ;
	}
	if (AccessMode.bWrite)
	{
		Prot = PAGE_READWRITE;
		dwDesiredAccess |= FILE_MAP_WRITE;
	}

	uint64_t MaximumSize = Size + Offset;
	hFileMap = CreateFileMapping(Handle,
		NULL,
		Prot,  
		MaximumSize >> 32,
		MaximumSize,
		NULL);

	if (!hFileMap)
	{
		return nullptr;
	}

	pMemMap = (char*)MapViewOfFile(hFileMap,
		dwDesiredAccess,
		Offset >> 32,
		Offset,
		Size);

	if (!pMemMap)
	{
		CloseHandle(hFileMap);
		hFileMap = NULL;
		return nullptr;
	}
	return pMemMap;
}

void Linx::File::UnMemMap()
{
	if (pMemMap)
	{
		UnmapViewOfFile(pMemMap);
		pMemMap = nullptr;
	}

	if (hFileMap)
	{
		CloseHandle(hFileMap);
		hFileMap = NULL;
	}
}

void Linx::File::Close() noexcept
{
	UnMemMap();

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

	int Flag = 0;
	if ((FileFlag.bWrite) && !(FileFlag.bRead))
		Flag |= O_WRONLY;
	else if (!(FileFlag.bWrite) && (FileFlag.bRead))
		Flag |= O_RDONLY;
	else if ((FileFlag.bWrite) && (FileFlag.bRead))
		Flag |= O_RDWR;
	else return false;

	if(FileFlag.bCreate)
		Flag |= O_CREAT;
	if(!(FileFlag.bOpen))
		Flag |= O_EXCL;
	if(FileFlag.bAppend)
		Flag |= O_APPEND;
	if (FileFlag.bTruncate)
		Flag |= O_TRUNC;

    Handle = open(GetTimeString(Filename).c_str(), Flag, 0666);

	WrittenLen = 0;
	LastMilliSeconds = GetTotalMilliSeconds();

	return Handle != INVALID_HANDLE_VALUE;
}

bool Linx::File::Open(const std::string& InFilename, FileOpenFlag InFlag) noexcept
{
	Filename = InFilename;
	FileFlag = InFlag;
	return Open();
}

bool Linx::File::Open(const char* InFilename, FileOpenFlag InFlag) noexcept
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
	Super::Write(Buf, BufSize);

	if (ShouldRotate())
	{
		Rotate();
	}

	auto ret = write(Handle, Buf, BufSize);
	if (ret >= 0)
	{
		WrittenLen += ret;
	}
	return ret;
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

char* Linx::File::MemMap(size_t Size, size_t Offset, FileOpenFlag AccessMode)
{
    struct stat sb;
    if (fstat(Handle, &sb) == -1)
    {
        return nullptr;
    }

	int Prot = 0;
	if (AccessMode.bRead)
	{
		Prot |= PROT_READ;
	}
	if (AccessMode.bWrite)
	{
		Prot |= PROT_WRITE;
	}
	
    pMemMap = (char*)mmap(0, Size, Prot, MAP_SHARED, Handle, Offset);
    if (pMemMap == MAP_FAILED)
    {
        return nullptr;
    }

    MapSize = Size;
    return pMemMap;
}

void Linx::File::UnMemMap()
{
	if (pMemMap)
	{
		munmap(pMemMap, MapSize);
	}
}

void Linx::File::Close() noexcept
{
	UnMemMap();

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