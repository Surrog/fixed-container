#include "catch.hpp"
#include "test_functions_iterators.hpp"
#include "fixed/impl/basic_listed_vector_iterator.hpp"
#include "fixed/impl/aligned_allocation_pattern.hpp"

template <template <typename, typename> typename ITERATOR_TYPE>
void test_basic_list_iterator()
{

	fixed::_impl::aligned_stack_allocator<char, 4> mem;
	typedef typename fixed::_impl::aligned_stack_allocator<char, 4>::iterator mem_iterator;
	fixed::_impl::aligned_stack_allocator<mem_iterator, 4> ref_test;
	typedef typename fixed::_impl::aligned_stack_allocator<mem_iterator, 4>::iterator ref_iterator;

	typedef typename ITERATOR_TYPE<char, ref_iterator> iterator;
	 
	auto ref_it = ref_test.begin();
	auto mem_it = mem.begin();
	for (std::size_t i = 0u; i < 4u; i++)
	{
		*mem_it = '1' + char(i);
		*ref_it = mem_it;
		std::advance(ref_it, 1);
		std::advance(mem_it, 1);
	}

	{
		iterator it1;
		iterator it2;
		CHECK(it1 == it2);
		CHECK(!(it1 != it2));
	}

	{
		iterator it(ref_test.begin());
		CHECK(*it == '1');
		iterator copy(it);
		CHECK(*copy == '1');
		iterator move(std::move(it));
		CHECK(*move == '1');
	}

	{
		iterator it(ref_test.begin());
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
		iterator it(ref_test.begin());
		CHECK(*it == '1');
		it += 2;
		CHECK(*it == '3');
		it -= 1;
		CHECK(*it == '2');
		iterator copy = it + 2;
		CHECK(*copy == '4');
		CHECK(*it == '2');
		copy = it - 1;
		CHECK(*copy == '1');
		CHECK(*it == '2');
	}

	{
		iterator it0(ref_test.begin());
		iterator it1(ref_test.begin() + 1);
		CHECK((it1 - it0) == 1);
		CHECK((it0 - it1) == -1);
	}

	{
		iterator it(ref_test.begin());
		CHECK(*it == '1');
		CHECK(it[0] == '1');
		CHECK(it[1] == '2');
		CHECK(it[2] == '3');
		CHECK(it[3] == '4');
	}

	{
		iterator it(ref_test.begin());
		iterator it1(ref_test.begin() + 1);

		CHECK(it < it1);
		CHECK(it1 > it);
		CHECK(it1 >= it1);
		CHECK(it1 >= it);
		CHECK(it <= it);
		CHECK(it <= it1);
	}

	{
		iterator beg(ref_test.begin());
		iterator end(beg + 4);
		CHECK(std::equal(beg, end, mem.begin(), mem.begin() + 4));
	}
}


TEST_CASE("testing basic_list iterator", "[utility]")
{
	test_not_const<fixed::_impl::basic_listed_vector_iterator<
		char, char*>>();
	test_not_const<fixed::_impl::const_basic_list_iterator<
		char, char*>>();

	test_assign<fixed::_impl::const_basic_list_iterator<char, char*>,
		fixed::_impl::basic_listed_vector_iterator<char, char*>>();

	test_basic_list_iterator<fixed::_impl::basic_listed_vector_iterator>();
	test_basic_list_iterator<fixed::_impl::const_basic_list_iterator>();

}