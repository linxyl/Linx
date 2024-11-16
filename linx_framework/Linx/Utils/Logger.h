#pragma once

#include <cassert>

#include "Linx/IOStream/FileStream.h"
#include "Linx/Utils/Singleton.h"
#include "Linx/Thread/Lock/SpinLock.h"
#include "Linx/Config.h"

namespace Linx
{

#define LOG			Singleton<Logger>::Instance()->Lock().SetCurrentLevel(ELogLevel::LevelFatal)
#define LOG_DEBUG	Singleton<Logger>::Instance()->Lock().SetCurrentLevel(ELogLevel::LevelDebug) << "Debug: " <<  __FILE__ << ": " << __LINE__ << ": "
#define LOG_INFO	Singleton<Logger>::Instance()->Lock().SetCurrentLevel(ELogLevel::LevelInfo) << "Info: " << __FILE__ << ": " << __LINE__ << ": "
#define LOG_WARN	Singleton<Logger>::Instance()->Lock().SetCurrentLevel(ELogLevel::LevelWarn) << "Warn: " << __FILE__ << ": " << __LINE__ << ": "
#define LOG_ERROR	Singleton<Logger>::Instance()->Lock().SetCurrentLevel(ELogLevel::LevelError) << "Error: " << __FILE__ << ": " << __LINE__ << ": "
#define LOG_FATAL	Singleton<Logger>::Instance()->Lock().SetCurrentLevel(ELogLevel::LevelFatal) << "Fatal: " << __FILE__ << ": " << __LINE__ << ": "

	/** Level of the information recorded in the log */
	namespace ELogLevel {
		enum Type : uint8_t
		{
			LevelDebug = 0,
			LevelInfo,
			LevelWarn,
			LevelError,
			LevelFatal,
		};
	};

	class LoggerBuf : public StreambufBase<File>
	{
	public:
		using Super = StreambufBase<File>;

		LoggerBuf(File* InIO) : Super(InIO) {};

	public:
		/** Set the level of the log itself. */
		inline void SetLogLevel(ELogLevel::Type InLevel) noexcept { LogLevel = InLevel; }

		/** Get the level of the log itself. */
		inline ELogLevel::Type GetLogLevel() const noexcept { return LogLevel; }

		/** Set the log output level. */
		inline void SetCurrentLevel(ELogLevel::Type InLevel) noexcept { CurrentLevel = InLevel; }

		/** Get the log output level. */
		inline ELogLevel::Type GetCurrentLevel() const noexcept { return CurrentLevel; }

		/** Lock prevents multi thread contention. */
		inline void Lock() { LockInst.lock(); }
		inline void Unlock() { LockInst.unlock(); }

	protected:
		// Begin std::basic_streambuf Interface.
		virtual std::streamsize xsputn(const char* Ptr, std::streamsize Count) override;
		virtual int sync() override;
		// End std::basic_streambuf Interface.

		// Begin StreambufBase Interface.
		virtual int FlushWriteBuffer() override;
		virtual void ClearWriteBuffer() override;
		// End StreambufBase Interface.

	private:
		ELogLevel::Type LogLevel = ELogLevel::LevelDebug;
		ELogLevel::Type CurrentLevel = ELogLevel::LevelDebug;

		Spinlock LockInst;
	};

	/**
	 * The log file stream that can write The current time and the user's own messages.
	 * You can set whether to print log information on the console at the same time.
	 */
	class Logger : public std::iostream, public File
	{
	public:
		Logger() : std::iostream(&Buf), Buf(this)
		{};
		inline Logger(const char* InFilename, uint32_t InFlag = 
			EFileFlag::ERead | EFileFlag::EWrite | EFileFlag::ECreate | EFileFlag::EOpen)
			:std::iostream(&Buf), Buf(this), File(InFilename, InFlag)
		{}
		inline Logger(std::string InFilename, uint32_t InFlag = 
			EFileFlag::ERead | EFileFlag::EWrite | EFileFlag::ECreate | EFileFlag::EOpen)
			:std::iostream(&Buf), Buf(this), File(InFilename, InFlag)
		{}

	public:
		/** Lock prevents multi thread contention. Should not be used! */
		inline Logger& Lock() { Buf.Lock(); return *this; }
		inline Logger& Unlock() { Buf.Unlock(); return *this; }

		/** Set the log output level. */
		inline void SetLogLevel(ELogLevel::Type InLevel) noexcept { Buf.SetLogLevel(InLevel); }

		/** Get the level of the log itself. */
		inline ELogLevel::Type GetLogLevel() const noexcept { return Buf.GetLogLevel(); }

		/** Set the log output level. */
		inline Logger& SetCurrentLevel(ELogLevel::Type InLevel) noexcept { Buf.SetCurrentLevel(InLevel); return *this; }

		/** Get the log output level. */
		inline ELogLevel::Type GetCurrentLevel() const noexcept { return Buf.GetCurrentLevel(); }

	private:
		LoggerBuf Buf;
	};
}
