#include "catch.hpp"

#include <type_traits>

#include "fixed/allocation_pattern.hpp"
#include "fixed/impl/aligned_allocation_pattern.hpp"
#include "fixed/impl/constexpr_allocation_pattern.hpp"
#include "fixed/impl/fixed_def.hpp"
#include "test_struct.hpp"

template <template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_noexcept_constructor()
{
    static_assert(
        std::is_nothrow_default_constructible<Alloc_pattern<char, 10>>::value,
        "Not noexcept default constructible");
    static_assert(
        std::is_nothrow_move_constructible<Alloc_pattern<char, 10>>::value,
        "Not noexcept move constructible");
    static_assert(
        std::is_nothrow_move_assignable<Alloc_pattern<char, 10>>::value,
        "Not noexcept move assignable");

    static_assert(std::is_nothrow_default_constructible<
                      Alloc_pattern<test_move, 10>>::value,
        "Not noexcept default constructible");
    static_assert(
        std::is_nothrow_move_constructible<Alloc_pattern<test_move, 10>>::value,
        "Not noexcept move constructible");
    static_assert(
        std::is_nothrow_move_assignable<Alloc_pattern<test_move, 10>>::value,
        "Not noexcept move assignable");

    static_assert(std::is_nothrow_default_constructible<
                      Alloc_pattern<test_emplace_struct, 10>>::value,
        "Not noexcept default constructible");
    static_assert(std::is_nothrow_move_constructible<
                      Alloc_pattern<test_emplace_struct, 10>>::value,
        "Not noexcept move constructible");
    static_assert(std::is_nothrow_move_assignable<
                      Alloc_pattern<test_emplace_struct, 10>>::value,
        "Not noexcept move assignable");
}

template <template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_is_allocator()
{
    static_assert(fixed::_impl::is_allocation_pattern_v<Alloc_pattern<int, 10>>,
        "Not alloc pattern");

    {
        Alloc_pattern<int, 10> alloc;
        auto itbeg = alloc.begin();
        auto itend = alloc.end();
    }

    {
        Alloc_pattern<int, 10> alloc;
        auto itbeg = alloc.cbegin();
        auto itend = alloc.cend();
    }

    {
        Alloc_pattern<int, 10> alloc;
        int& i = alloc[0];
        alloc.valid_pointer(&i);
    }

    {
        Alloc_pattern<int, 10> alloc;
        CHECK(alloc.max_size() >= 10);
    }
}

template <template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_is_allocator_continous()
{
    static_assert(fixed::_impl::is_alloc_pattern_contiguous_v<Alloc_pattern<int, 10>>,
        "Not continous alloc pattern");

    {
        Alloc_pattern<int, 10> alloc;
        auto* p = alloc.data();
		p++;
    }

    {
        const Alloc_pattern<int, 10> alloc;
        const auto* p = alloc.data();
		p++;
    }
}

template <template <typename, fixed::_impl::container_size_type>
    typename Alloc_pattern>
void test_allocator_pattern()
{
    test_noexcept_constructor<Alloc_pattern>();
    test_is_allocator<Alloc_pattern>();
}

TEST_CASE("testing allocation pattern", "[alloc]")
{
    test_allocator_pattern<fixed::_impl::aligned_stack_allocator>();
    test_allocator_pattern<fixed::_impl::aligned_heap_allocator>();
    test_allocator_pattern<fixed::_impl::constexpr_stack_allocator>();
    test_allocator_pattern<fixed::stack_allocator>();
    test_allocator_pattern<fixed::heap_allocator>();

    test_is_allocator_continous<fixed::_impl::aligned_stack_allocator>();
    test_is_allocator_continous<fixed::_impl::aligned_heap_allocator>();
    test_is_allocator_continous<fixed::_impl::constexpr_stack_allocator>();
    test_is_allocator_continous<fixed::stack_allocator>();
    test_is_allocator_continous<fixed::heap_allocator>();
}
