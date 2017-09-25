#ifndef HEAPLESS_STRING_HPP
#define HEAPLESS_STRING_HPP

#include "impl/basic_string.hpp"
#include "impl/fixed_def.hpp"

namespace fixed
{
template <_impl::container_size_type SIZE>
using string = _impl::basic_string<char, SIZE>;

template <_impl::container_size_type SIZE>
using wstring = _impl::basic_string<wchar_t, SIZE>;

template <_impl::container_size_type SIZE>
using u16string = _impl::basic_string<char16_t, SIZE>;

template <_impl::container_size_type SIZE>
using u32string = _impl::basic_string<char32_t, SIZE>;
}

#endif //! HEAPLESS_STRING_HPP