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
		inline operator signed char() const noexcept { return strtol(Str.c_str(), nullptr, 0); }
		inline operator short() const noexcept { return strtol(Str.c_str(), nullptr, 0); }
		inline operator int() const noexcept { return strtol(Str.c_str(), nullptr, 0); }
		inline operator long long() const noexcept { return strtoll(Str.c_str(), nullptr, 0); }
		inline operator unsigned char() const noexcept { return strtoul(Str.c_str(), nullptr, 0); }
		inline operator unsigned short() const noexcept { return strtoul(Str.c_str(), nullptr, 0); }
		inline operator unsigned int() const noexcept { return strtoul(Str.c_str(), nullptr, 0); }
		inline operator unsigned long long() const noexcept { return strtoull(Str.c_str(), nullptr, 0); }
		inline operator float() const noexcept { return strtof(Str.c_str(), nullptr); }
		inline operator double() const noexcept { return strtod(Str.c_str(), nullptr); }
		inline operator long double() const noexcept { return strtold(Str.c_str(), nullptr); }
		inline operator std::string() const noexcept { return Str; }

		inline std::string GetString() const noexcept { return Str; }

	private:
		std::string Str;
	};
}