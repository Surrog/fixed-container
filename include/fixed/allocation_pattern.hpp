#ifndef FIXED_ALLOCATION_PATTERN_HPP
#define FIXED_ALLOCATION_PATTERN_HPP

#include <type_traits>
#include "fixed/impl/fixed_def.hpp"
#include "fixed/impl/aligned_allocation_pattern.hpp"
#include "fixed/impl/constexpr_allocation_pattern.hpp"

namespace fixed
{
	namespace _impl
	{
		template <bool B,
			template <typename, fixed::_impl::size_t> typename T,
			template <typename, fixed::_impl::size_t> typename F,
			typename VALUE, fixed::_impl::size_t SIZE>
		struct allocator_dispatch
		{
			typedef T<VALUE, SIZE> type;
		};

		template <
			template <typename, fixed::_impl::size_t> typename T,
			template <typename, fixed::_impl::size_t> typename F,
			typename VALUE, fixed::_impl::size_t SIZE>
		struct allocator_dispatch<false, T, F, VALUE, SIZE>
		{
			 typedef F<VALUE, SIZE> type;
		};
	}

	template <typename T, fixed::_impl::size_t SIZE>
	using stack_allocator = typename _impl::allocator_dispatch<
		astd::is_trivial_v<T>,
		fixed::_impl::constexpr_stack_allocator,
		fixed::_impl::aligned_stack_allocator,
		T, SIZE>::type;

	template <typename T, fixed::_impl::size_t SIZE>
	using heap_allocator = fixed::_impl::aligned_heap_allocator<T, SIZE>;
}

#endif //!FIXED_ALLOCATION_PATTERN_HPP