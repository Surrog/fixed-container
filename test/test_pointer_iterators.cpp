#include "catch.hpp"
#include "fixed/impl/aligned_allocation_pattern.hpp"
#include "fixed/impl/basic_pointer_iterator.hpp"
#include "test_functions_iterators.hpp"
#include <type_traits>

template <typename T>
using aligned_type = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

template <template <typename, typename> typename ITERATOR_TYPE>
void test_pointer_iterator()
{
    {
        ITERATOR_TYPE<char, char> it;
        ITERATOR_TYPE<char, char> it2;
        CHECK(it == it2);
        CHECK(!(it != it2));
    }

    {
        fixed::_impl::aligned_stack_allocator<char, 256> mem;
		auto beg = mem.begin();
        beg[0] = 't';
		beg[1] = 'e';
		beg[2] = 's';
		beg[3] = 't';

        auto it = mem.begin();
        CHECK(*it == 't');
        auto it1 = it;
        CHECK(*it1 == 't');
        typename fixed::_impl::aligned_stack_allocator<char, 256>::iterator it2(std::move(it));
        CHECK(*it2 == 't');
		typename fixed::_impl::aligned_stack_allocator<char, 256>::iterator it3 = std::move(it1);
        CHECK(*it3 == 't');
    }

    {
        fixed::_impl::aligned_stack_allocator<char, 256> mem;
        mem.begin()[0] = '1';
		mem.begin()[1] = '2';
		mem.begin()[2] = '3';
		mem.begin()[3] = '4';

        auto it = mem.begin();

        CHECK(*it == '1');
        it++;
        CHECK(*it == '2');
        it--;
        CHECK(*it == '1');
        ++it;
        CHECK(*it == '2');
        --it;
        CHECK(*it == '1');

        auto next = it + 1;
        CHECK(*it == '1');
        CHECK(*next == '2');
        auto prev = next - 1;
        CHECK(*next == '2');
        CHECK(*prev == '1');
        prev += 3;
        CHECK(*prev == '4');
        prev -= 3;
        CHECK(*prev == '1');
    }

    {
        fixed::_impl::aligned_stack_allocator<char, 256> mem;
		mem.begin()[0] = '1';
		mem.begin()[1] = '2';
		mem.begin()[2] = '3';
		mem.begin()[3] = '4';

        auto it = mem.begin();
        auto it1 = mem.begin() + 1;
        CHECK((it1 - it) == 1);
        CHECK((it - it1) == -1);
    }

    {
        fixed::_impl::aligned_stack_allocator<char, 256> mem;
		mem.begin()[0] = '1';
		mem.begin()[1] = '2';
		mem.begin()[2] = '3';
		mem.begin()[3] = '4';

        auto it = mem.begin();
        CHECK(it[0] == '1');
        CHECK(it[1] == '2');
        CHECK(it[2] == '3');
        CHECK(it[3] == '4');
    }

    {
        fixed::_impl::aligned_stack_allocator<char, 256> mem;
		mem.begin()[0] = '1';
		mem.begin()[1] = '2';
		mem.begin()[2] = '3';
		mem.begin()[3] = '4';

        auto it = mem.begin();
        auto it1 = mem.begin() + 1;

        CHECK(it < it1);
        CHECK(it1 > it);
        CHECK(it1 >= it1);
        CHECK(it1 >= it);
        CHECK(it <= it);
        CHECK(it <= it1);
    }

    {
        fixed::_impl::aligned_stack_allocator<char, 256> mem;
		mem.begin()[0] = '1';
		mem.begin()[1] = '2';
		mem.begin()[2] = '3';
		mem.begin()[3] = '4';
        char test[5] = "1234";

        auto beg = mem.begin();
        auto end = mem.begin() + 4;

        CHECK(std::equal(beg, end, test, test + 4));
    }
}

TEST_CASE("testing vector iterator", "[utility]")
{
    test_not_const<fixed::_impl::pointer_iterator<char>>();
    test_not_const<fixed::_impl::const_pointer_iterator<char>>();

    test_assign<fixed::_impl::const_pointer_iterator<char>,
        fixed::_impl::pointer_iterator<char>>();

    test_pointer_iterator<fixed::_impl::pointer_iterator>();
    test_pointer_iterator<fixed::_impl::const_pointer_iterator>();

}
