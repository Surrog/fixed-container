#ifndef FIXED_LIST_HPP
#define FIXED_LIST_HPP

#include "fixed/impl/basic_list.hpp"

namespace fixed
{
	template <typename T, _impl::container_size_type SIZE, template <typename, _impl::container_size_type>  typename Alloc_pattern = _impl::basic_stack_allocator>
	using list = _impl::basic_list<T, SIZE, Alloc_pattern>;
}

#endif //!FIXED_LIST_HPP