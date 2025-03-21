#pragma once

#include <type_traits>
#include <string>
#include <iostream>

#define BEGIN_MAKE_ARG_PARSER(ClassName)						BEGIN_MAKE_ARG_PARSER_IMPL(ClassName)
#define END_MAKE_ARG_PARSER										};
#define BEGIN_ARG_PARSER_ADD_OPT								BEGIN_ARG_PARSER_ADD_OPT_IMPL
#define END_ARG_PARSER_ADD_OPT									END_ARG_PARSER_ADD_OPT_IMPL
#define BIND_ARG_PARSER_FUNCTION(Opt0, Func, ...)				ParseOneOpt(Opt0, Func, ##__VA_ARGS__);
#define BIND_ARG_PARSER_ONE_OPT(Opt0, ...)						ParseOneOpt(Opt0, __VA_ARGS__);
#define BIND_ARG_PARSER_TWO_OPT(Opt0, Opt1, ...)				ParseTwoOpt(Opt0, Opt1, __VA_ARGS__);
#define BIND_ARG_PARSER_THREE_OPT(Opt0, Opt1, Opt2, ...)		ParseThreeOpt(Opt0, Opt1, Opt2, __VA_ARGS__);
#define BIND_ARG_PARSER_FOUR_OPT(Opt0, Opt1, Opt2, Opt3, ...)	ParseFourOpt(Opt0, Opt1, Opt2, Opt3, __VA_ARGS__);
#define SET_ARG_PARSER_HELP_STRING(s)							SET_ARG_PARSER_HELP_STRING_IMPY(s)


#define BEGIN_MAKE_ARG_PARSER_IMPL(ClassName)\
	class ClassName : public ArgParserBase\
	{\
		using SelfClassName = ClassName;

#define BEGIN_ARG_PARSER_ADD_OPT_IMPL\
	public:\
		SelfClassName(int argc, char** argv) : ArgParserBase(argc, argv)\
		{\
			try\
			{\
				//BIND_ARG_PARSER_FUNCTION("-h", "--help", PrintHelp);

#define END_ARG_PARSER_ADD_OPT_IMPL\
			}\
			catch (...)\
			{\
				PrintHelp();\
			}\
		}

#define SET_ARG_PARSER_HELP_STRING_IMPY(s)\
	protected:\
		static void PrintHelp() noexcept\
		{ cout << s << endl; exit(0); }


namespace Linx
{
	class ArgParserBase
	{
	public:
		constexpr ArgParserBase(int argc, char** argv) noexcept : argc(argc), argv(argv) {};

	public:
		constexpr void ParseRecursion(int index) const noexcept {};

		template<typename Type, typename... Args>
		void ParseRecursion(int index, Type& Var, Args&&... args)
		{
			if (index >= argc)
			{
				throw 0;
			}
			
			if constexpr (std::is_integral_v<std::remove_reference_t<Type>>)
			{
				Var = stoll(argv[index], nullptr, 0);
			}
			else if constexpr (std::is_same_v<std::remove_reference_t<Type>, std::string>)
			{
				Var = argv[index];
			}
			else if constexpr (is_same_v<std::decay_t<std::remove_reference_t<Type>>, char*>)
			{
				strcpy(Var, argv[index]);
			}
			else
			{
				static_assert(false, "Invalid type");
			}

			ParseRecursion(index + 1, std::forward<Args>(args)...);
		}
		
		template <typename... Args, typename Type0>
		bool ParseOneOpt(const char* Opt, Type0& Arg0, Args&&... args)
		{
			for (size_t i = 1; i < argc; i++)
			{
				if (0 == strcmp(argv[i], Opt))
				{
					if constexpr (std::is_function_v<std::remove_pointer_t<Type0>>)
					{
						Arg0(std::forward<Args>(args)...);
					}
					else
					{
						ParseRecursion(i + 1, Arg0, std::forward<Args>(args)...);
					}
					return true;
				}
			}
			return false;
		}

		template <typename... Args, typename Type0>
		bool ParseTwoOpt(const char* Opt0, const char* Opt1, Type0& Arg0, Args&&... args)
		{
			if (ParseOneOpt(Opt0, Arg0, std::forward<Args>(args)...))
			{
				return true;
			}
			if (ParseOneOpt(Opt1, Arg0, std::forward<Args>(args)...))
			{
				return true;
			}
			return false;
		}

		template <typename... Args, typename Type0>
		bool ParseThreeOpt(const char* Opt0, const char* Opt1, const char* Opt2, Type0& Arg0, Args&&... args)
		{
			if (ParseTwoOpt(Opt0, Opt1, Arg0, std::forward<Args>(args)...))
			{
				return true;
			}
			if (ParseOneOpt(Opt2, Arg0, std::forward<Args>(args)...))
			{
				return true;
			}
			return false;
		}

		template <typename... Args, typename Type0>
		bool ParseFourOpt(const char* Opt0, const char* Opt1, const char* Opt2, const char* Opt3, Type0& Arg0, Args&&... args)
		{
			if (ParseTwoOpt(Opt0, Opt1, Arg0, std::forward<Args>(args)...))
			{
				return true;
			}
			if (ParseTwoOpt(Opt2, Opt3, Arg0, std::forward<Args>(args)...))
			{
				return true;
			}
			return false;
		}

	private:
		int argc;

		char** argv;
	};
}
