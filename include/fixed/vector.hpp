#ifndef HEAPLESS_VECTOR_HPP
#define HEAPLESS_VECTOR_HPP

#include "impl/basic_vector.hpp"
#include "impl/fixed_def.hpp"

namespace fixed
{
	template <typename T, _impl::size_type SIZE, typename Allocator = _impl::basic_stack_allocator<T, SIZE>>
	using vector = _impl::basic_vector<T, SIZE, Allocator>;
}

#endif //!HEAPLESS_VECTOR_HPP