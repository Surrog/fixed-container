
#include "catch.hpp"
#include "fixed/list.hpp"

void test_list()
{
	{ //constructor definition check
		auto expected = { 1, 2, 3, 4, 5 };

		fixed::list<int, 20> l;
		
		fixed::list<int, 30, fixed::_impl::basic_heap_allocator> list_on_heap;
		l = list_on_heap;
		fixed::_impl::empty_source alloc_source_inst;
		fixed::list<int, 20> l_with_alloc_source(alloc_source_inst);
		fixed::list<int, 20> l_diff_with_alloc_source(list_on_heap, alloc_source_inst);
		l = l_with_alloc_source;
		l = expected;
		CHECK(std::equal(l.begin(), l.end(), expected.begin(), expected.end()));
		list_on_heap = expected;

		fixed::list<int, 20> l_diff_with_alloc_source_not_empty(list_on_heap, alloc_source_inst);
		CHECK(std::equal(l_diff_with_alloc_source_not_empty.begin(), l_diff_with_alloc_source_not_empty.end(), expected.begin(), expected.end()));


		fixed::list<int, 20> l_copy(l);
		CHECK(std::equal(l_copy.begin(), l_copy.end(), expected.begin(), expected.end()));

		fixed::list<int, 20> l_copy_alloc(l, alloc_source_inst);
		CHECK(std::equal(l_copy_alloc.begin(), l_copy_alloc.end(), expected.begin(), expected.end()));

		fixed::list<int, 20> l_move(std::move(l));
		CHECK(std::equal(l_move.begin(), l_move.end(), expected.begin(), expected.end()));
		CHECK(l.size() == 0);
		CHECK(l.empty());

		fixed::list<int, 20> l_move_alloc(std::move(l_move), alloc_source_inst);
		CHECK(std::equal(l_move_alloc.begin(), l_move_alloc.end(), expected.begin(), expected.end()));
		CHECK(l_move.size() == 0);
		CHECK(l_move.empty());

		fixed::list<int, 20> l_with_size(5);
		CHECK(l_with_size.size() == 5);
		fixed::list<int, 20> l_with_size_alloc(5, alloc_source_inst);
		CHECK(l_with_size_alloc.size() == 5);

		fixed::list<int, 20> l_with_values(5, 2);
		CHECK(l_with_values.size() == 5);
		CHECK(std::all_of(l_with_values.begin(), l_with_values.end(), [](const auto& v) {return v == 2; }));
		fixed::list<int, 20> l_with_values_alloc(5, 2, alloc_source_inst);
		CHECK(l_with_values_alloc.size() == 5);
		CHECK(std::all_of(l_with_values.begin(), l_with_values.end(), [](const auto& v) {return v == 2; }));

		fixed::list<int, 20> l_with_iterator(expected.begin(), expected.end());
		CHECK(l_with_iterator.size() == expected.size());
		CHECK(std::equal(l_with_iterator.begin(), l_with_iterator.end(), expected.begin(), expected.end()));
		fixed::list<int, 20> l_with_iterator_alloc(expected.begin(), expected.end(), alloc_source_inst);
		CHECK(l_with_iterator_alloc.size() == expected.size());
		CHECK(std::equal(l_with_iterator_alloc.begin(), l_with_iterator_alloc.end(), expected.begin(), expected.end()));

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

	{
		fixed::list<int, 30> l;
		auto expected = { 1, 1, 1, 1, 1 };
		l.assign(5, 1);
		CHECK(l.size() == 5);
		CHECK(std::equal(l.begin(), l.end(), expected.begin(), expected.end()));
	}

	{
		fixed::list<int, 30> l;
		auto expected = { 1, 1, 1, 1, 1 };
		l.assign(expected.begin(), expected.end());
		CHECK(l.size() == 5);
		CHECK(std::equal(l.begin(), l.end(), expected.begin(), expected.end()));
	}

	{
		fixed::list<int, 30> l;
		auto expected = { 1, 1, 1, 1, 1 };
		l.assign(expected);
		CHECK(l.size() == 5);
		CHECK(std::equal(l.begin(), l.end(), expected.begin(), expected.end()));
	}

	{
		fixed::list<int, 30> l;
		auto expected = { 1, 2, 3, 4, 5 };
		l.assign(expected.begin(), expected.end());
		CHECK(l.size() == 5);
		CHECK(std::equal(l.begin(), l.end(), expected.begin(), expected.end()));
		CHECK(l.front() == 1);
		CHECK(l.back() == 5);
		auto reverse_expected = { 5, 4, 3, 2, 1 };
		CHECK(std::equal(l.rbegin(), l.rend(), reverse_expected.begin(), reverse_expected.end()));
	}
}

void test_modifiers()
{
	{
		fixed::list<int, 30> l;
		CHECK(l.empty());
		auto expected = { 1, 2, 3, 4, 5 };
		l.assign(expected.begin(), expected.end());
		CHECK(!l.empty());
		l.clear();
		CHECK(l.empty());
	}

	{
		fixed::list<int, 30> l;
		auto expected = { 1, 2, 3, 4, 5 };
		l.assign(expected);

		l.insert(l.begin(), 0);
		auto exp1 = { 0, 1, 2, 3, 4, 5 };
		CHECK(l.size() == exp1.size());
		CHECK(std::equal(l.begin(), l.end(), exp1.begin(), exp1.end()));

		l.insert(l.end(), 6);
		auto exp2 = { 0, 1, 2, 3, 4, 5, 6 };
		CHECK(l.size() == exp2.size());
		CHECK(std::equal(l.begin(), l.end(), exp2.begin(), exp2.end()));

		l.insert(l.begin() + 1, 10);
		auto exp3 = { 0, 10, 1, 2, 3, 4, 5, 6 };
		CHECK(l.size() == exp3.size());
		CHECK(std::equal(l.begin(), l.end(), exp3.begin(), exp3.end()));
	}

	{
	}
}

TEST_CASE("testing lists", "[linear]")
{
	test_list();
	test_modifiers();
}
