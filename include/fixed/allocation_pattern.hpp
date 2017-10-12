#ifndef FIXED_ALLOCATION_PATTERN_HPP
#define FIXED_ALLOCATION_PATTERN_HPP

#include <type_traits>
#include "fixed/impl/fixed_def.hpp"
#include "fixed/impl/aligned_allocation_pattern.hpp"
#include "fixed/impl/constexpr_allocation_pattern.hpp"

namespace fixed
{
	template <typename T, fixed::_impl::container_size_type SIZE>
	using stack_allocator = typename std::conditional<
		std::is_trivial<T>::value,
		fixed::_impl::constexpr_stack_allocator<T, SIZE>,
		fixed::_impl::aligned_stack_allocator<T, SIZE>
	>::type;

	template <typename T, fixed::_impl::container_size_type SIZE>
	using heap_allocator = fixed::_impl::aligned_heap_allocator<T, SIZE>;
}

#endif //!FIXED_ALLOCATION_PATTERN_HPP