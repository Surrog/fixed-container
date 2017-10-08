
#include "catch.hpp"
#include "fixed/vector.hpp"
#include "test_functions_unary_containers.hpp"
#include "test_struct.hpp"
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename VECTOR_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_vector_constructor()
{
    test_constructor<VECTOR_T, Alloc_pattern>();
    test_destructor<VECTOR_T, Alloc_pattern>();
    test_operator_assignment<VECTOR_T, Alloc_pattern>();
    test_assign<VECTOR_T, Alloc_pattern>();
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename VECTOR_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_vector_element_access()
{
    test_at<VECTOR_T, Alloc_pattern>();
    test_operator_array<VECTOR_T, Alloc_pattern>();
    test_front<VECTOR_T, Alloc_pattern>();
    test_back<VECTOR_T, Alloc_pattern>();
    test_vector_data<VECTOR_T, Alloc_pattern>();
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename VECTOR_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_vector_iterator()
{
    test_begin_end<VECTOR_T, Alloc_pattern>();
    test_rbegin_rend<VECTOR_T, Alloc_pattern>();
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename VECTOR_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_vector_capacity()
{
    test_generic_capacity<VECTOR_T, Alloc_pattern>();
    test_vector_capacity_func<VECTOR_T, Alloc_pattern>();
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename VECTOR_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_vector_modifier()
{
    test_clear<VECTOR_T, Alloc_pattern>();
    test_insert<VECTOR_T, Alloc_pattern>();
    test_emplace<VECTOR_T, Alloc_pattern>();
    test_erase<VECTOR_T, Alloc_pattern>();
    test_push_back<VECTOR_T, Alloc_pattern>();
    test_emplace_back<VECTOR_T, Alloc_pattern>();
    test_pop_back<VECTOR_T, Alloc_pattern>();
    test_resize<VECTOR_T, Alloc_pattern>();
    test_swap<VECTOR_T, Alloc_pattern>();
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename VECTOR_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_vector()
{
	test_noexcept_constructor<VECTOR_T, Alloc_pattern>();
    test_vector_constructor<VECTOR_T, Alloc_pattern>();
    test_vector_element_access<VECTOR_T, Alloc_pattern>();
    test_vector_iterator<VECTOR_T, Alloc_pattern>();
    test_vector_capacity<VECTOR_T, Alloc_pattern>();
    test_vector_modifier<VECTOR_T, Alloc_pattern>();
}

TEST_CASE("testing basic_vector", "[linear]")
{
    test_vector<fixed::vector, fixed::_impl::aligned_stack_allocator>();
    test_vector<fixed::vector, fixed::_impl::aligned_heap_allocator>();
}