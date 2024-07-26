#pragma once

#include <type_traits>
#include <string>

namespace Linx
{
	/**
	 * Can be implicitly converted to a variety of common types,
	 * and can also be assigned to these types.
	 */
	class VariableString
	{
	public:
		VariableString() = default;


		template<typename T, typename = std::enable_if_t<
			std::is_integral_v<T> ||
			std::is_same_v<T, char*> ||
			std::is_same_v<T, const char*>>>
		inline VariableString(const T Val) { *this = Val; }
		inline VariableString(const std::string& Val) { *this = Val; }

	public:
		template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
		inline VariableString& operator=(T Val) { Str = std::to_string(Val); return *this; }
		inline VariableString& operator=(const char* Val) noexcept { Str = Val; return *this; }
		inline VariableString& operator=(const std::string& Val) noexcept { Str = Val; return *this; }

		inline operator bool() { return Str == "1"; }
		inline operator signed char() { return ConverToSigned(); }
		inline operator short() { return ConverToSigned(); }
		inline operator int() { return ConverToSigned(); }
		inline operator long long() { return ConverToSigned(); }
		inline operator unsigned char() { return ConvertToUnsigned(); }
		inline operator unsigned short() { return ConvertToUnsigned(); }
		inline operator unsigned int() { return ConvertToUnsigned(); }
		inline operator unsigned long long() { return ConvertToUnsigned(); }
		inline operator float() { return std::stof(Str); }
		inline operator double() { return std::stod(Str); }
		inline operator long double() { return std::stold(Str); }
		inline operator std::string() const noexcept { return Str; }

		inline std::string GetString() const noexcept { return Str; }

	private:
		long long ConverToSigned();
		unsigned long long ConvertToUnsigned();

	private:
		std::string Str;
	};
}