#ifndef FIXED_DEFINITION_HPP
#define FIXED_DEFINITION_HPP

#include <cstddef>
#include <cstdint>
#include <exception>

namespace fixed
{
	namespace _impl
	{
#ifdef SIGNED_INT_SIZE_TYPE
		using container_size_type = std::int32_t;
#else
		using container_size_type = std::size_t;
#endif

#if !defined(NDEBUG) || defined(FIXED_CONTAINER_CHECK_BOUND)
#define FIXED_CHECK(ex) (!(ex)) ? throw std::out_of_range("allocator out of range or empty container") : ((void)0)
#else //!defined(NDEBUG) || defined(FIXED_CONTAINER_CHECK_BOUND)
#define FIXED_CHECK(ex) ((void)0)
#endif //defined(NDEBUG) && !defined(FIXED_CONTAINER_CHECK_BOUND)
	}
}

#endif //!FIXED_DEFINITION_HPP