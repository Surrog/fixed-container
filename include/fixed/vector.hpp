#ifndef HEAPLESS_VECTOR_HPP
#define HEAPLESS_VECTOR_HPP

#include "impl/basic_vector.hpp"

namespace fixed
{
	template <typename T, std::size_t SIZE, typename Allocator = _impl::basic_stack_allocator<T, SIZE>>
	using vector = _impl::basic_vector<T, SIZE, Allocator>;
}

#endif //!HEAPLESS_VECTOR_HPP