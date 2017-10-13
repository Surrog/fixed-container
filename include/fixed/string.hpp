#ifndef HEAPLESS_STRING_HPP
#define HEAPLESS_STRING_HPP

#include "impl/basic_string.hpp"
#include "impl/fixed_def.hpp"
#include "fixed/impl/constexpr_allocation_pattern.hpp"

namespace fixed
{
template <fixed::_impl::container_size_type SIZE, 
	template <typename, fixed::_impl::container_size_type> typename Alloc_pattern = fixed::_impl::constexpr_stack_allocator>
using string = _impl::basic_string<char, SIZE, std::char_traits<char>, Alloc_pattern>;

template <fixed::_impl::container_size_type SIZE,
	template <typename, fixed::_impl::container_size_type> typename Alloc_pattern = fixed::_impl::constexpr_stack_allocator>
using wstring = _impl::basic_string<wchar_t, SIZE, std::char_traits<wchar_t>, Alloc_pattern>;

template <fixed::_impl::container_size_type SIZE,
	template <typename, fixed::_impl::container_size_type> typename Alloc_pattern = fixed::_impl::constexpr_stack_allocator>
using u16string = _impl::basic_string<char16_t, SIZE, std::char_traits<char16_t>, Alloc_pattern>;

template <fixed::_impl::container_size_type SIZE,
	template <typename, fixed::_impl::container_size_type> typename Alloc_pattern = fixed::_impl::constexpr_stack_allocator>
using u32string = _impl::basic_string<char32_t, SIZE, std::char_traits<char32_t>, Alloc_pattern>;
}

#endif //! HEAPLESS_STRING_HPP