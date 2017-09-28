
#include "catch.hpp"
#include "fixed/impl/fixed_def.hpp"
#include "fixed/list.hpp"
#include <cstring>
#include <list>
#include <string>

#include "test_functions_unary_containers.hpp"
#include "test_struct.hpp"

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename LIST_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_list()
{
    { // constructor definition check
        auto expected = {1, 2, 3, 4, 5};

        LIST_T<int, 20, Alloc_pattern> l;

        LIST_T<int, 30, fixed::_impl::aligned_heap_allocator> list_on_heap;
        l = list_on_heap;
        fixed::_impl::empty_source alloc_source_inst;
        LIST_T<int, 20, Alloc_pattern> l_with_alloc_source(alloc_source_inst);
        LIST_T<int, 20, Alloc_pattern> l_diff_with_alloc_source(
            list_on_heap, alloc_source_inst);
        l = l_with_alloc_source;
        l = expected;
        CHECK(std::equal(l.begin(), l.end(), expected.begin(), expected.end()));
        list_on_heap = expected;

        LIST_T<int, 20, Alloc_pattern> l_diff_with_alloc_source_not_empty(
            list_on_heap, alloc_source_inst);
        CHECK(std::equal(l_diff_with_alloc_source_not_empty.begin(),
            l_diff_with_alloc_source_not_empty.end(), expected.begin(),
            expected.end()));

        LIST_T<int, 20, Alloc_pattern> l_copy(l);
        CHECK(std::equal(
            l_copy.begin(), l_copy.end(), expected.begin(), expected.end()));

        LIST_T<int, 20, Alloc_pattern> l_copy_alloc(l, alloc_source_inst);
        CHECK(std::equal(l_copy_alloc.begin(), l_copy_alloc.end(),
            expected.begin(), expected.end()));

        LIST_T<int, 20, Alloc_pattern> l_move(std::move(l));
        CHECK(std::equal(
            l_move.begin(), l_move.end(), expected.begin(), expected.end()));
        CHECK(l.size() == 0);
        CHECK(l.empty());

        LIST_T<int, 20, Alloc_pattern> l_move_alloc(
            std::move(l_move), alloc_source_inst);
        CHECK(std::equal(l_move_alloc.begin(), l_move_alloc.end(),
            expected.begin(), expected.end()));
        CHECK(l_move.size() == 0);
        CHECK(l_move.empty());

        LIST_T<int, 20, Alloc_pattern> l_with_size(5);
        CHECK(l_with_size.size() == 5);
        LIST_T<int, 20, Alloc_pattern> l_with_size_alloc(5, alloc_source_inst);
        CHECK(l_with_size_alloc.size() == 5);

        LIST_T<int, 20, Alloc_pattern> l_with_values(5, 2);
        CHECK(l_with_values.size() == 5);
        CHECK(std::all_of(l_with_values.begin(), l_with_values.end(),
            [](const auto& v) { return v == 2; }));
        LIST_T<int, 20, Alloc_pattern> l_with_values_alloc(
            5, 2, alloc_source_inst);
        CHECK(l_with_values_alloc.size() == 5);
        CHECK(std::all_of(l_with_values.begin(), l_with_values.end(),
            [](const auto& v) { return v == 2; }));

        LIST_T<int, 20, Alloc_pattern> l_with_iterator(
            expected.begin(), expected.end());
        CHECK(l_with_iterator.size() == expected.size());
        CHECK(std::equal(l_with_iterator.begin(), l_with_iterator.end(),
            expected.begin(), expected.end()));
        LIST_T<int, 20, Alloc_pattern> l_with_iterator_alloc(
            expected.begin(), expected.end(), alloc_source_inst);
        CHECK(l_with_iterator_alloc.size() == expected.size());
        CHECK(std::equal(l_with_iterator_alloc.begin(),
            l_with_iterator_alloc.end(), expected.begin(), expected.end()));
    }

    {
        LIST_T<int, 20, Alloc_pattern> l1 = {1, 2, 3, 4, 5};
        auto expected = {1, 2, 3, 4, 5};
        CHECK(
            std::equal(l1.begin(), l1.end(), expected.begin(), expected.end()));
        LIST_T<int, 20, Alloc_pattern> l2 = l1;
        CHECK(
            std::equal(l1.begin(), l1.end(), expected.begin(), expected.end()));
        CHECK(
            std::equal(l2.begin(), l2.end(), expected.begin(), expected.end()));
        LIST_T<int, 20, Alloc_pattern> l3 = std::move(l2);
        CHECK(
            std::equal(l3.begin(), l3.end(), expected.begin(), expected.end()));
        CHECK(l2.empty());

        LIST_T<int, 30, Alloc_pattern> l4 = l1;
    }
}

template <template <typename, fixed::_impl::container_size_type,
              template <typename, fixed::_impl::container_size_type> typename>
          typename LIST_T,
    template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_modifiers()
{

    test_assign<LIST_T, Alloc_pattern>();
    test_capacity<LIST_T, Alloc_pattern>();
    test_insert<LIST_T, Alloc_pattern>();
    test_erase<LIST_T, Alloc_pattern>();
    test_push_back<LIST_T, Alloc_pattern>();
    test_emplace<LIST_T, Alloc_pattern>();
    test_emplace_back<LIST_T, Alloc_pattern>();
    test_pop_back<LIST_T, Alloc_pattern>();
    test_resize<LIST_T, Alloc_pattern>();
    test_push_front<LIST_T, Alloc_pattern>();
    test_emplace_front<LIST_T, Alloc_pattern>();
    test_swap<LIST_T, Alloc_pattern>();
    test_merge<LIST_T, Alloc_pattern>();
}

TEST_CASE("testing lists", "[linear]")
{
    test_list<fixed::list, fixed::_impl::aligned_stack_allocator>();
    test_modifiers<fixed::list, fixed::_impl::aligned_stack_allocator>();
    test_list_constant_reference<fixed::list,
        fixed::_impl::aligned_stack_allocator>();

    test_list<fixed::list, fixed::_impl::aligned_heap_allocator>();
    test_modifiers<fixed::list, fixed::_impl::aligned_heap_allocator>();
    test_list_constant_reference<fixed::list,
        fixed::_impl::aligned_heap_allocator>();
}
