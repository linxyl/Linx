#pragma once

#include "Linx/IOStream/StreambufBase.h"
#include "Linx/IO/File.h"

namespace Linx
{
	/**
	 * A file stream that can rotate depending on time or size.
	 */
	class FileStream : public std::iostream, public File
	{
	public:
		FileStream() : std::iostream(&Buf), Buf(this)
		{};
		inline FileStream(const char* InFilename, FileOpenFlag InFlag = FileOpenFlag())
			:std::iostream(&Buf), Buf(this), File(InFilename, InFlag)
		{}
		inline FileStream(std::string InFilename, FileOpenFlag InFlag = FileOpenFlag())
			:std::iostream(&Buf), Buf(this), File(InFilename, InFlag)
		{}

	protected:
		StreambufBase<File> Buf;
	};
}

