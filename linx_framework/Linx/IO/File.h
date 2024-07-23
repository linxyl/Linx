#pragma once

#include <cassert>
#include <fstream>

#include "Linx/Utils/Singleton.h"

namespace Linx
{
	class FileBuf : public std::filebuf
	{
	public:
		using Super = std::filebuf;

		FileBuf() { ; }
		~FileBuf() { sync(); }

	public:
		/** 
		 * Open the file.
		 * @param InFilename	Filename format.	%Y=year, %m=month, %d=day, %H=hour, %M=minute, %S=second, %s=millisecond.
		 */
		FileBuf* Open();

		/** Close the file. */
		inline void Close() noexcept { close(); }

		/** Returns whether the file is open. */
		inline bool IsOpen() const noexcept { return is_open(); }

	public:
		std::string Filename;

		std::ios::openmode Mode;

		/** The number of bytes split by size. If it is 0, not divided by size */
		size_t SplitSize = 0;

		/** The split time. If it is 0, not divided by time */
		long long SplitMilliSeconds = 0;

	protected:
		// Begin std::filebuf Interface.
		std::streamsize xsputn(const char* Ptr, std::streamsize Count) override;
		virtual int sync() override;
		// End std::filebuf Interface.

	private:
		/** Flush the buffer to the file. */
		int FlushBuffer();

	private:

		/************************************************************************/
		/*                          File rotate                               */
		/************************************************************************/

		/** When the file reaches certain conditions, another file is created. */
		void Rotate() noexcept;

		/** Check whether should rotate the file. */
		bool ShouldRotate() const noexcept;

		/** Number of bytes that have been written to the current file */
		size_t WrittenLen;

		/** Milliseconds since the file was last created */
		long long LastMilliSeconds;
	};

	/**
	 * A file stream that can rotate depending on time or size.
	 */
	class File : public std::iostream
	{
	public:
		using Super = std::iostream;
		using MyBuf = FileBuf;

	public:
		File() : Super(std::addressof(Buf)) {}

		/** Open the file. */
		explicit File(const std::string& InFilename, std::ios_base::openmode Mode = ios_base::out);

	public:
		/** 
		 * Open the file.
		 * @param InFilename	Filename format.	%Y=year, %m=month, %d=day, %H=hour, %M=minute, %S=second, %s=millisecond.
		 */
		bool Open(const std::string& InFilename, std::ios_base::openmode Mode = ios_base::out);

		/** Close the file. */
		void Close() noexcept { Buf.Close(); }

		/** Returns whether the file is open. */
		inline bool IsOpen() const noexcept { return Buf.IsOpen(); }

		std::streamsize Read(char* Ptr, std::streamsize Count);

		std::streamsize Write(char* Ptr, std::streamsize Count);

	public:

		/************************************************************************/
		/*               Set the splitting mode of files.                   */
		/************************************************************************/

		/** Set the number of days for splitting each file. */
		inline void SetSplitByDay(size_t Days) noexcept { Buf.SplitMilliSeconds = Days * 24 * 60 * 60 * 1000; Buf.SplitSize = 0; }

		/** Set the number of hours each file splits. */
		inline void SetSplitByHour(size_t Hours) noexcept { Buf.SplitMilliSeconds =  Hours * 60 * 60 * 1000; Buf.SplitSize = 0; }

		/** Set the number of minutes each file splits. */
		inline void SetSplitByMinute(size_t Minutes) noexcept { Buf.SplitMilliSeconds = Minutes * 60 * 1000; Buf.SplitSize = 0; }

		/** Set the number of seconds each file splits. */
		inline void SetSplitBySeconds(size_t Seconds) noexcept { Buf.SplitMilliSeconds = Seconds * 1000; Buf.SplitSize = 0; }

		/** Set the number of bytes each file splits. */
		inline void SetSplitBySize(size_t Size) noexcept { Buf.SplitSize = Size; Buf.SplitMilliSeconds = 0; }

		/** Set the number of KBs each file splits. */
		inline void SetSplitBySizeK(size_t Size) noexcept { Buf.SplitSize = Size * 1024; Buf.SplitMilliSeconds = 0; }

		/** Set the number of MBs each file splits. */
		inline void SetSplitBySizeM(size_t Size) noexcept { Buf.SplitSize = Size * 1024 * 1024; Buf.SplitMilliSeconds = 0; }

	protected:
		MyBuf Buf;
	};
}
