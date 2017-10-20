#ifndef FIXED_LIST_HPP
#define FIXED_LIST_HPP

#include "fixed/impl/basic_listed_vector.hpp"

namespace fixed
{
template <typename T, _impl::size_t SIZE,
    template <typename, _impl::size_t> typename Alloc_pattern
    = _impl::aligned_stack_allocator>
using listed_vector = _impl::basic_listed_vector<T, SIZE, Alloc_pattern>;
}

#endif //! FIXED_LIST_HPP