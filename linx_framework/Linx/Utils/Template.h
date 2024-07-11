#pragma once

/** Instantiate char and wchar_t types for the class template. */
#define INS_W_CHAR_TEMPLATE(x)\
template x<char>;\
template x<wchar_t>;

/** Instantiate all character types for the class template. */
#define INS_ALL_CHAR_TEMPLATE(x)\
template x<char>;\
template x<signed char>;\
template x<unsigned char>;\
template x<wchar_t>;\
template x<char16_t>;\
template x<char32_t>;

