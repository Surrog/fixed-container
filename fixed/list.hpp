#ifndef FIXED_LIST_HPP
#define FIXED_LIST_HPP

#include "fixed/impl/fixed_def.hpp"
#include "fixed/impl/basic_list.hpp"
#include "fixed/impl/basic_allocator.hpp"

namespace fixed
{
	template <typename T, fixed::_impl::size_type SIZE, 
		template <typename, fixed::_impl::size_type> typename Alloc = fixed::_impl::basic_stack_allocator>
	using list = fixed::_impl::basic_list<T, SIZE, Alloc>
}

#endif //!FIXED_LIST_HPP