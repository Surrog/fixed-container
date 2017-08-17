#ifndef HEAPLESS_VECTOR_HPP
#define HEAPLESS_VECTOR_HPP

#include "impl/basic_vector.hpp"
#include "impl/fixed_def.hpp"

namespace fixed
{
	template <typename T, _impl::size_type SIZE,
		template <typename, _impl::size_type> typename Alloc_pattern = _impl::basic_stack_allocator>
	using vector = _impl::basic_vector<T, SIZE, Alloc_pattern>;
}

#endif //!HEAPLESS_VECTOR_HPP