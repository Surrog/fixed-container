#ifndef HEAPLESS_VECTOR_HPP
#define HEAPLESS_VECTOR_HPP

#include "impl/basic_vector.hpp"
#include "impl/fixed_def.hpp"

namespace fixed
{
template <typename T, _impl::container_size_type SIZE,
    template <typename, _impl::container_size_type> typename Alloc_pattern
    = _impl::basic_stack_allocator>
using vector = _impl::basic_vector<T, SIZE, Alloc_pattern>;
}

template <typename T, fixed::_impl::container_size_type LSIZE,
    fixed::_impl::container_size_type RSIZE,
    template <typename, fixed::_impl::container_size_type> typename LALLOC,
    template <typename, fixed::_impl::container_size_type> typename RALLOC>
bool operator==(const fixed::vector<T, LSIZE, LALLOC>& lval,
    const fixed::vector<T, RSIZE, RALLOC>& rval)
{
    return lval.size() == rval.size()
        && std::equal(lval.begin(), lval.end(), rval.begin(), rval.end());
}

template <typename T, fixed::_impl::container_size_type LSIZE,
    fixed::_impl::container_size_type RSIZE,
    template <typename, fixed::_impl::container_size_type> typename LALLOC,
    template <typename, fixed::_impl::container_size_type> typename RALLOC>
bool operator!=(const fixed::vector<T, LSIZE, LALLOC>& lval,
    const fixed::vector<T, RSIZE, RALLOC>& rval)
{
    return !operator==(lval, rval);
}

#endif //! HEAPLESS_VECTOR_HPP