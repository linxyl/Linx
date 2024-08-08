#pragma once

#include "Linx/IOStream/StreambufBase.h"
#include "Linx/IO/IFile.h"

namespace Linx
{
	class FileBuf : public StreambufBase<File>
	{
		friend class FileStream;
	public:
		FileBuf() : StreambufBase<File>() {};
	};

	/**
	 * A file stream that can rotate depending on time or size.
	 */
	class FileStream : public std::iostream, public IFile
	{
	public:
		FileStream() : std::iostream(&Buf), IFile(Buf.IO)
		{};

		/** 
		 * Open the file.
		 * @param InFilename	Filename format.
		 *						%Y=year, %m=month, %d=day, %H=hour, %M=minute, %S=second, %s=millisecond.
		 * @param InFlag		File properties, should be the value after EFileFlag is performed '|' operated.
		 */
		inline FileStream(const char* InFilename, uint32_t InFlag = 
			EFileFlag::ERead | EFileFlag::EWrite | EFileFlag::ECreate | EFileFlag::EOpen) noexcept :
			FileStream()
		{
			Buf.IO.Open(InFilename, InFlag);
		};
		inline FileStream(const std::string& InFilename, uint32_t InFlag = 
			EFileFlag::ERead | EFileFlag::EWrite | EFileFlag::ECreate | EFileFlag::EOpen) noexcept :
			FileStream()
		{
			Buf.IO.Open(InFilename, InFlag);
		};

	protected:
		FileBuf Buf;
	};
}

