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

	template <typename T>
	constexpr bool is_allocation_pattern_v = is_allocation_pattern<T>::value;

	template <typename T>
	constexpr bool is_nothrow_default_constructible_v = std::is_nothrow_default_constructible<T>::value;

	template <typename T>
	constexpr bool is_nothrow_copy_constructible_v = std::is_nothrow_copy_constructible<T>::value;

	template <typename T>
	constexpr bool is_nothrow_move_constructible_v = std::is_nothrow_move_constructible<T>::value;

#if __cplusplus > 201602L
	template <typename T>
	constexpr bool is_nothrow_allocator_iterator_v = std::is_nothrow_invocable_r<T, decltype(&T::begin)>::value
		&& std::is_nothrow_invocable_r<T, decltype(&T::end)>::value
		&& std::is_nothrow_invocable_r<T, decltype(&T::cbegin)>::value
		&& std::is_nothrow_invocable_r<T, decltype(&T::cend)>::value;
#else
	template <typename T>
	constexpr bool is_nothrow_allocator_iterator_v = T::noexcept_iterators::value;
#endif

	template <typename T>
	struct continous_helper
	{
		typedef typename const T::aligned_type* (T::*const_function)() const ;
		typedef typename T::aligned_type* (T::*function)() ;
	};

	template <typename T>
	constexpr bool is_alloc_pattern_contiguous_v = std::is_member_function_pointer<decltype(
		continous_helper<T>::function(&T::data)
		)>::value
		&& std::is_member_function_pointer<decltype(
			continous_helper<T>::const_function(&T::data)
			)>::value;
}
}

#endif //! FIXED_DEFINITION_HPP