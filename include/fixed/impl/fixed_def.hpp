#ifndef FIXED_DEFINITION_HPP
#define FIXED_DEFINITION_HPP

#include <cstddef>
#include <cstdint>
#include <exception>

#include "fixed/impl/fixed_type_traits.hpp"

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
#define FIXED_CHECK_INBOUND(ex)                                                \
    (!(ex)) ? throw std::out_of_range("Iterator out of range") : ((void)0)
#define FIXED_CHECK_BADRANGE(ex)                                               \
    (!(ex)) ? throw std::out_of_range("Bad iterator range") : ((void)0)
#define FIXED_CHECK_FULL(ex)                                                   \
    (!(ex)) ? throw std::out_of_range("Container full") : ((void)0)
#define FIXED_CHECK_EMPTY(ex)                                                  \
    (!(ex)) ? throw std::out_of_range("Empty container") : ((void)0)
#define FIXED_CHECK_CUSTOM(ex, msg)                                            \
    (!(ex)) ? throw std::out_of_range(msg) : ((void)0)

#else //! defined(NDEBUG) || defined(FIXED_CONTAINER_CHECK_BOUND)
#define FIXED_CHECK(ex) ((void)0)
#endif // defined(NDEBUG) && !defined(FIXED_CONTAINER_CHECK_BOUND)

	struct allocation_pattern_tag
	{
	};

	template <typename T, class = void>
	struct is_allocation_pattern : public std::false_type
	{
	};

	template <typename T>
	struct is_allocation_pattern<T,
		fixed::astd::void_t<typename T::allocation_pattern>>
		: public std::true_type
	{
	};

	template <typename T, class = void>
	struct is_allocation_movable : public std::false_type
	{
	};

	template <typename T>
	struct is_allocation_movable<T,
		fixed::astd::void_t<typename T::allocation_movable>>
		: public T::allocation_movable
	{
	};

	template <typename T, class = void>
	struct is_allocation_contiguous : public std::false_type
	{
	};

	template <typename T>
	struct is_allocation_contiguous<T,
		fixed::astd::void_t<typename T::allocation_linear>>
		: public T::allocation_contiguous
	{
	};

}
}

#endif //! FIXED_DEFINITION_HPP