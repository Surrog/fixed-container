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
#ifndef FIXED_CONTAINER_CHECK_BOUND
#define FIXED_CONTAINER_CHECK_BOUND 1
#endif //!FIXED_CONTAINER_CHECK_BOUND

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
#define FIXED_CHECK_INBOUND(ex)                                                \
    ((void)0)
#define FIXED_CHECK_BADRANGE(ex)                                               \
    ((void)0)
#define FIXED_CHECK_FULL(ex)                                                   \
    ((void)0)
#define FIXED_CHECK_EMPTY(ex)                                                  \
    ((void)0)
#define FIXED_CHECK_CUSTOM(ex, msg)                                            \
    ((void)0)
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

	template <typename T, class = void>
	struct is_allocation_source : public std::false_type
	{
	};

	template <typename T>
	struct is_allocation_source<T,
		fixed::astd::void_t<typename T::allocator_source>>
		: public std::true_type
	{
	};

	struct allocator_source_tag
	{
	};

	template <typename T>
	constexpr bool is_allocation_source_v = is_allocation_source<T>::value;

	template <typename T>
	constexpr bool is_nothrow_default_constructible_v = std::is_nothrow_default_constructible<T>::value;

	template <typename T>
	constexpr bool is_nothrow_copy_constructible_v = std::is_nothrow_copy_constructible<T>::value;

	template <typename T>
	constexpr bool is_nothrow_move_constructible_v = std::is_nothrow_move_constructible<T>::value;

	template <typename T>
	constexpr bool is_nothrow_move_assignable_v = std::is_nothrow_move_assignable<T>::value;

#if __cplusplus > 201602L || _HAS_CXX17 == 1

	template <typename T>
	struct _nothrow_allocator_iterator_helper
	{
		typedef const typename T::const_iterator (T::*const_function)() const;
		typedef typename T::iterator (T::*function)();
	};

	template <typename T>
	constexpr bool is_nothrow_allocator_iterator_v = 
		std::is_nothrow_invocable_r<T, decltype(
			_nothrow_allocator_iterator_helper<T>::function(&T::begin)
			)>::value
		&& std::is_nothrow_invocable_r<T, decltype(
			_nothrow_allocator_iterator_helper<T>::function(&T::end)
			)>::value
		&& std::is_nothrow_invocable_r<T, decltype(
			_nothrow_allocator_iterator_helper<T>::const_function(&T::begin)
			)>::value
		&& std::is_nothrow_invocable_r<T, decltype(
			_nothrow_allocator_iterator_helper<T>::const_function(&T::end)
			)>::value
		&& std::is_nothrow_invocable_r<T, decltype(
			_nothrow_allocator_iterator_helper<T>::const_function(&T::cbegin)
			)>::value
		&& std::is_nothrow_invocable_r<T, decltype(
			_nothrow_allocator_iterator_helper<T>::const_function(&T::cend)
			)>::value;
#else
	template <typename T>
	constexpr bool is_nothrow_allocator_iterator_v = T::noexcept_iterators::value;
#endif

	template <typename T>
	struct _alloc_pattern_contiguous_helper
	{
		typedef const typename T::aligned_type* (T::*const_function)() const ;
		typedef typename T::aligned_type* (T::*function)() ;
	};

	template <typename T>
	constexpr bool is_alloc_pattern_contiguous_v = std::is_member_function_pointer<decltype(
		_alloc_pattern_contiguous_helper<T>::function(&T::data)
		)>::value
		&& std::is_member_function_pointer<decltype(
			_alloc_pattern_contiguous_helper<T>::const_function(&T::data)
			)>::value;
}
}

#endif //! FIXED_DEFINITION_HPP