#pragma once

#include <type_traits>

namespace Linx
{
	/** Determine whether is a character type. */
	template<typename T>
	constexpr bool is_char_type_v =
		_Is_any_of_v<remove_cv_t<T>,
		char,
		signed char,
		unsigned char,
		wchar_t,
		char16_t,
		char32_t>;
}