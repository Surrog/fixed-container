#ifndef HEAPLESS_STRING_HPP
#define HEAPLESS_STRING_HPP

#include "impl/basic_string.hpp"

namespace fixed
{
	template <std::size_t SIZE>
	using string = _impl::basic_string<char, SIZE>;

	template <std::size_t SIZE>
	using wstring = _impl::basic_string<wchar_t, SIZE>;

	template <std::size_t SIZE>
	using u16string = _impl::basic_string<char16_t, SIZE>;

	template <std::size_t SIZE>
	using u32string = _impl::basic_string<char32_t, SIZE>;
}

#endif //!HEAPLESS_STRING_HPP