#ifndef FIXED_LIST_HPP
#define FIXED_LIST_HPP

#include "fixed/impl/basic_listed_vector.hpp"

namespace fixed
{
template <typename T, _impl::container_size_type SIZE,
    template <typename, _impl::container_size_type> typename Alloc_pattern
    = _impl::aligned_stack_allocator>
using listed_vector = _impl::basic_listed_vector<T, SIZE, Alloc_pattern>;
}

#endif //! FIXED_LIST_HPP