#ifndef HEAPLESS_VECTOR_HPP
#define HEAPLESS_VECTOR_HPP

#include "impl/basic_vector.hpp"

namespace heapless
{
	template <typename T, std::size_t SIZE>
	using vector = _impl::basic_vector<T, SIZE>;
}

#endif //!HEAPLESS_VECTOR_HPP