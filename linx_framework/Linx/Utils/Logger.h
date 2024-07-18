#pragma once

#include <cassert>
#include <mutex>
#include <ostream>

#include "Linx/Utils/Singleton.h"

namespace Linx
{

#define LOG			Singleton<Logger>::Instance()->Lock()
#define LOG_DEBUG	Singleton<Logger>::Instance()->Lock() << DebugM << __FILE__ << ": " << __LINE__ << ": "
#define LOG_INFO	Singleton<Logger>::Instance()->Lock() << InfoM << __FILE__ << ": " << __LINE__ << ": "
#define LOG_WARN	Singleton<Logger>::Instance()->Lock() << WarnM << __FILE__ << ": " << __LINE__ << ": "
#define LOG_ERROR	Singleton<Logger>::Instance()->Lock() << ErrorM << __FILE__ << ": " << __LINE__ << ": "
#define LOG_FATAL	Singleton<Logger>::Instance()->Lock() << FatalM << __FILE__ << ": " << __LINE__ << ": "

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

	class LoggerBuf : public std::streambuf
	{
	public:
		using Super = std::streambuf;
		constexpr static size_t BufSize = 512;

		LoggerBuf();
		~LoggerBuf() { sync(); }

	public:
		/** Open the log file. */
		LoggerBuf* Open();

		/** Close the log file. */
		inline void Close() noexcept { fclose(File); }

		/** Returns whether the log file is open. */
		inline bool IsOpen() const noexcept { return File; }

		/** Set the level of the log itself. */
		inline void SetLogLevel(ELogLevel::Type InLevel) noexcept { LogLevel = InLevel; }

		/** Get the level of the log itself. */
		inline ELogLevel::Type GetLogLevel() const noexcept { return LogLevel; }

		/** Set the log output level. */
		inline void SetCurrentLevel(ELogLevel::Type InLevel) noexcept { CurrentLevel = InLevel; }

		/** Get the log output level. */
		inline ELogLevel::Type GetCurrentLevel() const noexcept { return CurrentLevel; }

		/** Lock prevents multi thread contention. */
		inline void Lock() { Mutex.lock(); }
		inline void Unlock() { Mutex.unlock(); }

	public:
		/** The main part of the log file name, excluding the time */
		std::string MainFilename;

		/** Whether to print on the console */
		bool bPrintable = true;

		/** The number of bytes split by size. If it is 0, not divided by size */
		size_t SplitSize = 0;

		/** The split time. If it is 0, not divided by time */
		long long SplitMilliSeconds = 0;

	protected:
		// Begin std::stringbuf Interface.
		virtual int_type overflow(int_type c);
		virtual int sync() override;
		// End std::stringbuf Interface.

	private:
		/** Flush the buffer to the file. */
		int FlushBuffer();

		void ClearBuffer();

		ELogLevel::Type LogLevel = ELogLevel::LevelDebug;
		ELogLevel::Type CurrentLevel = ELogLevel::LevelDebug;

		FILE* File;

		std::mutex Mutex;

		char Buffer[BufSize];

	private:

		/************************************************************************/
		/*                          Logger rotate                               */
		/************************************************************************/

		/** When the log file reaches certain conditions, another log file is created. */
		void Rotate() noexcept;

		/** Check whether should rotate the log. */
		bool ShouldRotate() const noexcept;

		/** Number of bytes that have been written to the current file */
		size_t WrittenLen;

		/** Milliseconds since the file was last created */
		long long LastMilliSeconds;
	};

	/**
	 * The log file stream that can write The current time and the user's own messages.
	 * You can set whether to print log information on the console at the same time.
	 */
	class Logger : public std::ostream
	{
	public:
		using Super = std::ostream;
		using MyBuf = LoggerBuf;
		using MyIos = std::ios;

	public:
		Logger() : Super(std::addressof(Buf)) {}

		/** Open the log file. */
		explicit Logger(const std::string& InFilename, ELogLevel::Type InLevel = ELogLevel::LevelDebug);

	public:
		/** Open the log file. */
		void Open(const std::string& InFilename);

		/** Close the log file. */
		void Close() noexcept;

		/** Returns whether the log file is open. */
		inline bool IsOpen() const noexcept { return Buf.IsOpen(); }

		/** Lock prevents multi thread contention. Should not be used! */
		inline Logger& Lock() { Buf.Lock(); return *this; }
		inline Logger& Unlock() { Buf.Unlock(); return *this; }

		/** Set the log output level. */
		inline void SetCurrentLevel(ELogLevel::Type InLevel) noexcept { Buf.SetCurrentLevel(InLevel); }

		/** Set the log output level. */
		inline void SetLogLevel(ELogLevel::Type InLevel) noexcept { Buf.SetLogLevel(InLevel); }

		/** Set whether is printable. */
		void SetPrintable(bool Val) noexcept { Buf.bPrintable = Val; }

		/** Returns whether is printable. */
		bool GetPrintable() const noexcept { return Buf.bPrintable; }

	public:

		/************************************************************************/
		/*               Set the splitting mode of log files.                   */
		/************************************************************************/

		/** Set the number of days for splitting each log file. */
		inline void SetSplitByDay(size_t Days) noexcept { Buf.SplitMilliSeconds = Days * 24 * 60 * 60 * 1000; Buf.SplitSize = 0; }

		/** Set the number of hours each log file splits. */
		inline void SetSplitByHour(size_t Hours) noexcept { Buf.SplitMilliSeconds =  Hours * 60 * 60 * 1000; Buf.SplitSize = 0; }

		/** Set the number of minutes each log file splits. */
		inline void SetSplitByMinute(size_t Minutes) noexcept { Buf.SplitMilliSeconds = Minutes * 60 * 1000; Buf.SplitSize = 0; }

		/** Set the number of seconds each log file splits. */
		inline void SetSplitBySeconds(size_t Seconds) noexcept { Buf.SplitMilliSeconds = Seconds * 1000; Buf.SplitSize = 0; }

		/** Set the number of bytes each log file splits. */
		inline void SetSplitBySize(size_t Size) noexcept { Buf.SplitSize = Size; Buf.SplitMilliSeconds = 0; }

		/** Set the number of KBs each log file splits. */
		inline void SetSplitBySizeK(size_t Size) noexcept { Buf.SplitSize = Size * 1024; Buf.SplitMilliSeconds = 0; }

		/** Set the number of MBs each log file splits. */
		inline void SetSplitBySizeM(size_t Size) noexcept { Buf.SplitSize = Size * 1024 * 1024; Buf.SplitMilliSeconds = 0; }

	protected:
		MyBuf Buf;
	};

	/** Set the current log output level to debug. Cannot be used by other classes of streams! */
	template <class Elem, class Traits>
	std::basic_ostream<Elem, Traits>& DebugM(std::basic_ostream<Elem, Traits>& Ostr)
	{
		Logger* pLogger = dynamic_cast<Logger*>(&Ostr);
		assert(pLogger);
		pLogger->SetCurrentLevel(ELogLevel::LevelDebug);
		Ostr << "Debug: ";
		return Ostr;
	}

	/** Set the current log output level to info. Cannot be used by other classes of streams! */
	template <class Elem, class Traits>
	std::basic_ostream<Elem, Traits>& InfoM(std::basic_ostream<Elem, Traits>& Ostr)
	{
		Logger* pLogger = dynamic_cast<Logger*>(&Ostr);
		assert(pLogger);
		pLogger->SetCurrentLevel(ELogLevel::LevelInfo);
		Ostr << "Info: ";
		return Ostr;
	}

	/** Set the current log output level to warn. Cannot be used by other classes of streams! */
	template <class Elem, class Traits>
	std::basic_ostream<Elem, Traits>& WarnM(std::basic_ostream<Elem, Traits>& Ostr)
	{
		Logger* pLogger = dynamic_cast<Logger*>(&Ostr);
		assert(pLogger);
		pLogger->SetCurrentLevel(ELogLevel::LevelWarn);
		Ostr << "Warn: ";
		return Ostr;
	}

	/** Set the current log output level to error. Cannot be used by other classes of streams! */
	template <class Elem, class Traits>
	std::basic_ostream<Elem, Traits>& ErrorM(std::basic_ostream<Elem, Traits>& Ostr)
	{
		Logger* pLogger = dynamic_cast<Logger*>(&Ostr);
		assert(pLogger);
		pLogger->SetCurrentLevel(ELogLevel::LevelError);
		Ostr << "Error: ";
		return Ostr;
	}

	/** Set the current log output level to fatal. Cannot be used by other classes of streams! */
	template <class Elem, class Traits>
	std::basic_ostream<Elem, Traits>& FatalM(std::basic_ostream<Elem, Traits>& Ostr)
	{
		Logger* pLogger = dynamic_cast<Logger*>(&Ostr);
		assert(pLogger);
		pLogger->SetCurrentLevel(ELogLevel::LevelFatal);
		Ostr << "Fatal: ";
		return Ostr;
	}
}
