
#include "catch.hpp"
#include "fixed/list.hpp"

void test_list()
{
	{
		fixed::list<int, 20> l;
		fixed::list<int, 30, fixed::_impl::basic_heap_allocator> list_on_heap;

		fixed::list<int, 20> l_with_alloc_source(fixed::_impl::empty_source());
		fixed::list<int, 20> l_with_values(5, 2, fixed::_impl::empty_source());
		fixed::list<int, 20> l_with_size(5, fixed::_impl::empty_source());
		auto values = { 1, 2, 3, 4, 5 };
		fixed::list<int, 20> l_with_iterator(values.begin(), values.end(), fixed::_impl::empty_source());
	}

	{
		fixed::list<int, 20> l1 = { 1, 2, 3, 4, 5 };
		auto expected = { 1, 2, 3, 4, 5 };
		CHECK(std::equal(l1.begin(), l1.end(), expected.begin(), expected.end()));
		fixed::list<int, 20> l2 = l1;
		CHECK(std::equal(l1.begin(), l1.end(), expected.begin(), expected.end()));
		CHECK(std::equal(l2.begin(), l2.end(), expected.begin(), expected.end()));
		fixed::list<int, 20> l3 = std::move(l2);
		CHECK(std::equal(l3.begin(), l3.end(), expected.begin(), expected.end()));
		CHECK(l2.empty());

		fixed::list<int, 30> l4 = l1;
	}

}

TEST_CASE("testing lists", "[linear]")
{
	test_list();
}
