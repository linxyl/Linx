#pragma once

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdint.h>
#include <string>

#include "IOBase.h"

namespace Linx
{
	/**
	 * Set the properties of the open file.
	 */
	struct FileOpenFlag
	{
		/** File writable. */
		uint8_t bWrite : 1;

		/** File readable. */
		uint8_t bRead : 1;

		/** Create when the file does not exist. */
		uint8_t bCreate : 1;

		/** Can be opened when the file already exists. */
		uint8_t bOpen : 1;

		/** mpty the contents of the file when it is opened. */
		uint8_t bTruncate : 1;

		/** Append content to the end of a file when it is opened. */
		uint8_t bAppend : 1;

	public:
		inline FileOpenFlag() noexcept :
			bWrite(true),
			bRead(true),
			bCreate(true),
			bOpen(true),
			bTruncate(false),
			bAppend(false)
		{}

		inline FileOpenFlag(bool flag) noexcept :
			bWrite(false),
			bRead(false),
			bCreate(false),
			bOpen(false),
			bTruncate(false),
			bAppend(false)
		{}
	};

	/**
	 * A file class that can rotate depending on time or size.
	 */
	class File : public IOBase
	{
	public:
		using Super = IOBase;

	public:
		File() noexcept = default;
		/**
		 * Open the file.
		 * @param InFilename	Filename format.
		 *						%Y=year, %m=month, %d=day, %H=hour, %M=minute, %S=second, %s=millisecond.
		 * @param InFlag		File properties, should be the value after EFileFlag is performed '|' operated.
		 */
		inline File(const char* InFilename, FileOpenFlag InFlag = FileOpenFlag()) noexcept
		{
			Open(InFilename, InFlag);
		}
		inline File(const std::string& InFilename, FileOpenFlag InFlag = FileOpenFlag()) noexcept
		{
			Open(InFilename, InFlag);
		}

		virtual ~File() noexcept { Close(); }

	private:
		std::string Filename;

		FileOpenFlag FileFlag;

	public:
		/** Open the file based on the stored file information. */
		bool Open() noexcept;

		/**
		 * Open the file.
		 * @param InFilename	Filename format.
		 *						%Y=year, %m=month, %d=day, %H=hour, %M=minute, %S=second, %s=millisecond.
		 * @param InFlag		File properties, should be the value after EFileFlag is performed '|' operated.
		 * @return whether the file is opened.
		 */
		bool Open(const char* InFilename, FileOpenFlag InFlag = FileOpenFlag()) noexcept;
		bool Open(const std::string& InFilename, FileOpenFlag InFlag = FileOpenFlag()) noexcept;

		/** Returns whether the file is opened. */
		bool IsOpen() const noexcept;

		/** Close the file. */
		void Close() noexcept;

		/** Read data from the file. */
		long Read(void* Buf, size_t BufSize) noexcept;

		/** Write data to the file. */
		virtual long Write(const void* Buf, size_t BufSize) noexcept override;

		/** Sets the offset of the pointer from the start of the file. */
		long SeekBegin(long Offset) const noexcept;

		/** Sets the offset of the pointer from the current location of the file. */
		long SeekCurrent(long Offset) const noexcept;

		/** Sets the offset of the pointer from the end of the file. */
		long SeekEnd(long Offset) const noexcept;

		/**
		 * Map a file to memory.
		 * @param Size			The size to map.
		 * @param Offset		Offset of the file map. Must be an integer multiple of the page of the MMU.
		 * @param AccessMode	Read and write permissions. Should use EFileFlag.
		 */
		char* MemMap(size_t Size, size_t Offset = 0, FileOpenFlag InFlag = FileOpenFlag());

		/** Unmap memory. */
		void UnMemMap();

	public:

		/************************************************************************/
		/*					 Set the splitting mode of files.                   */
		/************************************************************************/

		/** Set the number of seconds each file splits. */
		inline void SetSplitBySeconds(size_t Seconds) noexcept { SplitMilliSeconds = Seconds * 1000; SplitSize = 0; }

		/** Set the number of minutes each file splits. */
		inline void SetSplitByMinutes(size_t Minutes) noexcept { SplitMilliSeconds = Minutes * 60 * 1000; SplitSize = 0; }

		/** Set the number of hours each file splits. */
		inline void SetSplitByHours(size_t Hours) noexcept { SplitMilliSeconds = Hours * 60 * 60 * 1000; SplitSize = 0; }

		/** Set the number of days for splitting each file. */
		inline void SetSplitByDays(size_t Days) noexcept { SplitMilliSeconds = Days * 24 * 60 * 60 * 1000; SplitSize = 0; }

		/** Set the number of bytes each file splits. */
		inline void SetSplitBySize(size_t Size) noexcept { SplitSize = Size; SplitMilliSeconds = 0; }

		/** Set the number of KBs each file splits. */
		inline void SetSplitBySizeK(size_t Size) noexcept { SplitSize = Size * 1024; SplitMilliSeconds = 0; }

		/** Set the number of MBs each file splits. */
		inline void SetSplitBySizeM(size_t Size) noexcept { SplitSize = Size * 1024 * 1024; SplitMilliSeconds = 0; }

	private:

		/************************************************************************/
		/*                          FileStream rotate                           */
		/************************************************************************/

		/** When the file reaches certain conditions, another file is created. */
		void Rotate() noexcept;

		/** Check whether should rotate the file. */
		bool ShouldRotate() const noexcept;

		/** Number of bytes that have been written to the current file */
		size_t WrittenLen;

		/** Milliseconds since the file was last created */
		long long LastMilliSeconds;

		/** The number of bytes split by size. If it is 0, not divided by size */
		size_t SplitSize = 0;

		/** The split time. If it is 0, not divided by time */
		long long SplitMilliSeconds = 0;

#ifdef _WIN32
		/** Used map the memory */
		HANDLE hFileMap = NULL;
#else
		size_t MapSize;
#endif

		char* pMemMap = nullptr;
	};
}
