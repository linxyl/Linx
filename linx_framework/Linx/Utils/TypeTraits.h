#pragma once

#include <type_traits>
#include <complex>

namespace Linx
{
	template <typename T, typename... Ts>
	inline constexpr bool is_any_of_v = std::disjunction_v<std::is_same<T, Ts>...>;

	/** Determine whether is a character type. */
	template<typename T>
	constexpr bool is_char_type_v =
	is_any_of_v<std::remove_cv_t<T>,
		char,
		signed char,
		unsigned char,
		wchar_t,
		char16_t,
		char32_t>;

	template<typename Iterator, typename Enable = void>
	struct is_complex_iterator : std::false_type {};

	template<typename T>
	struct is_complex_iterator<T, typename std::enable_if<std::is_same<typename std::iterator_traits<T>::value_type, std::complex<typename std::iterator_traits<T>::value_type::value_type>>::value>::type>
		: std::true_type {};

	template<typename T>
	constexpr bool is_complex_iterator_v = is_complex_iterator<T>::value;

	template <typename T>
	constexpr bool dependent_false_v  = std::false_type::value;

#define GENERATE_HAS_MEMBER_FUNCTION(F)\
	template<typename, typename = void>\
	struct has_member_function_##F : std::false_type\
	{ };\
	\
	\
	template<typename T>\
	struct has_member_function_##F<T, std::void_t<decltype(std::declval<T>().##F())>> : std::true_type\
	{ };

#define HAS_MEMBER(T, x)\
	has_member_function_##x<T>::value;
}
