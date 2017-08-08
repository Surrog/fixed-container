#ifndef FIXED_LIST_HPP
#define FIXED_LIST_HPP

#include "fixed/impl/basic_list.hpp"

namespace fixed
{
	template <typename T, _impl::size_type SIZE, typename Allocator = _impl::basic_stack_allocator<T, SIZE>>
	using list = _impl::basic_list<T, SIZE, Allocator>;
}

#endif //!FIXED_LIST_HPP