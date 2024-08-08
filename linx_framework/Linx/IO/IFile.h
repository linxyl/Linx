#pragma once

#include "File.h"

namespace Linx
{
	class IFile
	{
	public:
		inline IFile(File& InFile) noexcept : FileRef(InFile) {};

	public:
		/** Open the file based on the stored file information. */
		inline bool Open() noexcept { return FileRef.Open(); }

		/** 
		 * Open the file.
		 * @param InFilename	Filename format.
		 *						%Y=year, %m=month, %d=day, %H=hour, %M=minute, %S=second, %s=millisecond.
		 * @param InFlag		File properties, should be the value after EFileFlag is performed '|' operated.
		 * @return whether the file is opened.
		 */
		inline bool Open(const char* InFilename, uint32_t InFlag = 
			EFileFlag::ERead | EFileFlag::EWrite | EFileFlag::ECreate | EFileFlag::EOpen) noexcept { return FileRef.Open(InFilename, InFlag); }
		inline bool Open(const std::string& InFilename, uint32_t InFlag = 
			EFileFlag::ERead | EFileFlag::EWrite | EFileFlag::ECreate | EFileFlag::EOpen) noexcept { return FileRef.Open(InFilename, InFlag); }

		/** Returns whether the file is opened. */
		inline bool IsOpen() const noexcept { return FileRef.IsOpen(); }

		/** Close the file. */
		inline void Close() noexcept { FileRef.Close(); }

		/** Read data from the file. */
		inline long Read(void* Buf, size_t BufSize) noexcept { return FileRef.Read(Buf, BufSize); }

		/** Write data to the file. */
		inline long Write(const void* Buf, size_t BufSize) noexcept { return FileRef.Write(Buf, BufSize); }

		/** Sets the offset of the pointer from the start of the file. */
		inline size_t SeekBegin(long Offset) const noexcept { return FileRef.SeekBegin(Offset); }

		/** Sets the offset of the pointer from the current location of the file. */
		inline size_t SeekCurrent(long Offset) const noexcept { return FileRef.SeekCurrent(Offset); }

		/** Sets the offset of the pointer from the end of the file. */
		inline size_t SeekEnd(long Offset) const noexcept { return FileRef.SeekEnd(Offset); }

	public:

		/************************************************************************/
		/*               Set the splitting mode of files.                   */
		/************************************************************************/

		/** Set the number of days for splitting each file. */
		inline void SetSplitByDay(size_t Days) noexcept { FileRef.SetSplitByDay(Days); }

		/** Set the number of hours each file splits. */
		inline void SetSplitByHour(size_t Hours) noexcept { FileRef.SetSplitByHour(Hours); }

		/** Set the number of minutes each file splits. */
		inline void SetSplitByMinute(size_t Minutes) noexcept { FileRef.SetSplitByMinute(Minutes); }

		/** Set the number of seconds each file splits. */
		inline void SetSplitBySeconds(size_t Seconds) noexcept { FileRef.SetSplitBySeconds(Seconds); }

		/** Set the number of bytes each file splits. */
		inline void SetSplitBySize(size_t Size) noexcept { FileRef.SetSplitBySize(Size); }

		/** Set the number of KBs each file splits. */
		inline void SetSplitBySizeK(size_t Size) noexcept { FileRef.SetSplitBySizeK(Size); }

		/** Set the number of MBs each file splits. */
		inline void SetSplitBySizeM(size_t Size) noexcept { FileRef.SetSplitBySizeM(Size); }

	private:
		File& FileRef;
	};
}
