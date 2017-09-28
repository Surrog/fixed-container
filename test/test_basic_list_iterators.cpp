#include "catch.hpp"
#include "test_functions_iterators.hpp"
#include "fixed/impl/basic_list_iterator.hpp"
#include "fixed/impl/basic_allocation_pattern.hpp"

template <template <typename, typename> typename ITERATOR_TYPE>
void test_basic_list_iterator()
{

	fixed::_impl::aligned_stack_allocator<char, 4> mem;
	mem[0] = '1';
	mem[1] = '2';
	mem[2] = '3';
	mem[3] = '4';
	fixed::_impl::aligned_stack_allocator<char*, 4> ref_test;
	ref_test[0] = &mem[0];
	ref_test[1] = &mem[1];
	ref_test[2] = &mem[2];
	ref_test[3] = &mem[3];

	typedef typename fixed::_impl::aligned_stack_allocator<char*, 4>::iterator iterator;

	using Iterator = ITERATOR_TYPE<char, iterator>;
	{
		Iterator it1;
		Iterator it2;
		CHECK(it1 == it2);
		CHECK(!(it1 != it2));
	}

	{
		Iterator it(ref_test.begin());
		CHECK(*it == '1');
		Iterator copy(it);
		CHECK(*copy == '1');
		Iterator move(std::move(it));
		CHECK(*move == '1');
	}

	{
		Iterator it(ref_test.begin());
		CHECK(*it == '1');
		it++;
		CHECK(*it == '2');
		++it;
		CHECK(*it == '3');
		it--;
		CHECK(*it == '2');
		--it;
		CHECK(*it == '1');
	}

	{
		Iterator it(ref_test.begin());
		CHECK(*it == '1');
		it += 2;
		CHECK(*it == '3');
		it -= 1;
		CHECK(*it == '2');
		Iterator copy = it + 2;
		CHECK(*copy == '4');
		CHECK(*it == '2');
		copy = it - 1;
		CHECK(*copy == '1');
		CHECK(*it == '2');
	}

	{
		Iterator it0(ref_test.begin());
		Iterator it1(ref_test.begin() + 1);
		CHECK((it1 - it0) == 1);
		CHECK((it0 - it1) == -1);
	}

	{
		Iterator it(ref_test.begin());
		CHECK(*it == '1');
		CHECK(it[0] == '1');
		CHECK(it[1] == '2');
		CHECK(it[2] == '3');
		CHECK(it[3] == '4');
	}

	{
		Iterator it(ref_test.begin());
		Iterator it1(ref_test.begin() + 1);

		CHECK(it < it1);
		CHECK(it1 > it);
		CHECK(it1 >= it1);
		CHECK(it1 >= it);
		CHECK(it <= it);
		CHECK(it <= it1);
	}

	{
		Iterator beg(ref_test.begin());
		Iterator end(beg + 4);
		CHECK(std::equal(beg, end, mem.begin(), mem.begin() + 4));
	}
}


TEST_CASE("testing basic_list iterator", "[utility]")
{
	test_not_const<fixed::_impl::basic_list_iterator<
		char, char*>>();
	test_not_const<fixed::_impl::const_basic_list_iterator<
		char, char*>>();

	test_assign<fixed::_impl::const_basic_list_iterator<char, char*>,
		fixed::_impl::basic_list_iterator<char, char*>>();

	test_basic_list_iterator<fixed::_impl::basic_list_iterator>();
	test_basic_list_iterator<fixed::_impl::const_basic_list_iterator>();

}