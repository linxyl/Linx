#pragma once

#include <fstream>
#include <sstream>

#include "Utils/Singleton.h"

namespace Linx
{
#define Debug	*(Singleton<Logger>::Instance()) << Debugf << __FILE__ << ": " << __LINE__ << ": "
#define Info	*(Singleton<Logger>::Instance()) << Infof << __FILE__ << ": " << __LINE__ << ": "
#define Warning	*(Singleton<Logger>::Instance()) << Warningf << __FILE__ << ": " << __LINE__ << ": "
#define Error	*(Singleton<Logger>::Instance()) << Errorf << __FILE__ << ": " << __LINE__ << ": "
#define Fatal	*(Singleton<Logger>::Instance()) << Fatalf << __FILE__ << ": " << __LINE__ << ": "

	/** Level of the information recorded in the log */
	namespace ELogLevel {
		enum Type : uint8_t
		{
			LevelDebug = 0,
			LevelInfo,
			LevelWarning,
			LevelError,
			LevelFatal,
		};
	};

	class LoggerBuf : public std::stringbuf
	{
	public:
		using Super = std::stringbuf;

		/** Open the log file. */
		LoggerBuf* Open();

		/** Close the log file. */
		void Close() noexcept;

		/** Returns whether the log file is open. */
		inline bool IsOpen() const noexcept { return File.is_open(); }

		std::string Filename;

		bool bPrintable = true;

		size_t SplitSize = 0;

		long long SplitMilliSeconds = 0;

		ELogLevel::Type CurrentLevel = ELogLevel::LevelDebug;

		ELogLevel::Type LogLevel = ELogLevel::LevelDebug;

	protected:
		// Begin std::stringbuf Interface.
		virtual int sync() override;
		// End std::stringbuf Interface.

	private:
		/** When the log file reaches certain conditions, another log file is created. */
		void Rotate() noexcept;

		size_t WrittenLen;

		long long LastMilliSeconds;

		std::ofstream File;
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

		/** Set the log output level. */
		inline void SetCurrentLevel(ELogLevel::Type InLevel) noexcept { Buf.CurrentLevel = InLevel; }

		/** Set the log output level. */
		inline void SetLogLevel(ELogLevel::Type InLevel) noexcept { Buf.LogLevel = InLevel; }

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

	template <class Elem, class Traits>
	std::basic_ostream<Elem, Traits>& Debugf(std::basic_ostream<Elem, Traits>& Ostr)
	{
		static_cast<Logger*>(&Ostr)->SetCurrentLevel(ELogLevel::LevelDebug);
		Ostr << "Debug: ";
		return Ostr;
	}

	template <class Elem, class Traits>
	std::basic_ostream<Elem, Traits>& Infof(std::basic_ostream<Elem, Traits>& Ostr)
	{
		static_cast<Logger*>(&Ostr)->SetCurrentLevel(ELogLevel::LevelInfo);
		Ostr << "Info: ";
		return Ostr;
	}

	template <class Elem, class Traits>
	std::basic_ostream<Elem, Traits>& Warningf(std::basic_ostream<Elem, Traits>& Ostr)
	{
		static_cast<Logger*>(&Ostr)->SetCurrentLevel(ELogLevel::LevelWarning);
		Ostr << "Warning: ";
		return Ostr;
	}

	template <class Elem, class Traits>
	std::basic_ostream<Elem, Traits>& Errorf(std::basic_ostream<Elem, Traits>& Ostr)
	{
		static_cast<Logger*>(&Ostr)->SetCurrentLevel(ELogLevel::LevelError);
		Ostr << "Error: ";
		return Ostr;
	}

	template <class Elem, class Traits>
	std::basic_ostream<Elem, Traits>& Fatalf(std::basic_ostream<Elem, Traits>& Ostr)
	{
		static_cast<Logger*>(&Ostr)->SetCurrentLevel(ELogLevel::LevelFatal);
		Ostr << "Fatal: ";
		return Ostr;
	}
}
