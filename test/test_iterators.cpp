#include "fixed/impl/iterator.hpp"
#include "catch.hpp"

template <template <typename> typename ITERATOR_TYPE>
void test_pointer_iterator()
{
	{
		ITERATOR_TYPE<char> it;
		ITERATOR_TYPE<char> it2;
		CHECK(it == it2);
		CHECK(!(it != it2));
	}

	{
		char test[256] = "test";

		ITERATOR_TYPE<char> it(test);
		CHECK(*it == 't');
		ITERATOR_TYPE<char> it1 = test;
		CHECK(*it1 == 't');
		ITERATOR_TYPE<char> it2(std::move(test));
		CHECK(*it2 == 't');
		ITERATOR_TYPE<char> it3 = std::move(test);
		CHECK(*it3 == 't');
	}

	{
		char test[256] = "1234";

		ITERATOR_TYPE<char> it(test);

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
		char test[256] = "1234";

		ITERATOR_TYPE<char> it(test);
		ITERATOR_TYPE<char> it1(test + 1);
		CHECK((it1 - it) == 1);
		CHECK((it - it1) == -1);
	}

	{
		char test[256] = "1234";

		ITERATOR_TYPE<char> it(test);
		CHECK(it[0] == '1');
		CHECK(it[1] == '2');
		CHECK(it[2] == '3');
		CHECK(it[3] == '4');
	}

	{
		char test[256] = "1234";

		ITERATOR_TYPE<char> it(test);
		ITERATOR_TYPE<char> it1(test + 1);

		CHECK(it < it1);
		CHECK(it1 > it);
		CHECK(it1 >= it1);
		CHECK(it1 >= it);
		CHECK(it <= it);
		CHECK(it <= it1);
	}
}

void test_wrap_iterator()
{
	{
		fixed::_impl::wrap_pointer_iterator<char, fixed::_impl::pointer_iterator<char*>> it1;
		fixed::_impl::wrap_pointer_iterator<char, fixed::_impl::pointer_iterator<char*>> it2;
		CHECK(it1 == it2);
		CHECK(!(it1 != it2));
	}

	{
		char test[5] = "1234";
		char* ref_test[5] = { test + 0, test + 1, test + 2, test + 3, test + 4 };


	}
}

template <typename ITERATOR_TYPE>
void test_not_const()
{
	CHECK(fixed::_impl::is_iterator<ITERATOR_TYPE>::value);
	CHECK(!std::is_const<ITERATOR_TYPE::value_type>::value);
	CHECK(!std::is_const<ITERATOR_TYPE::pointer>::value);
	CHECK(!std::is_const<ITERATOR_TYPE::reference>::value);
}

template <typename ITERATOR_TYPE>
void test_is_const()
{
	CHECK(fixed::_impl::is_iterator<ITERATOR_TYPE>::value);
	CHECK(std::is_const<ITERATOR_TYPE::value_type>::value);
	CHECK(std::is_const<ITERATOR_TYPE::pointer>::value);
	CHECK(std::is_const<ITERATOR_TYPE::reference>::value);
}

template <typename LIT_T, typename RIT_T>
void test_assign()
{
	RIT_T rval;
	LIT_T lval(rval);
	LIT_T lval2(RIT_T{});
}

TEST_CASE("testing allocator", "[utility]")
{
	test_not_const<fixed::_impl::pointer_iterator<char>>();
	test_not_const<fixed::_impl::const_pointer_iterator<char>>();

	test_assign<fixed::_impl::const_pointer_iterator<char>,
		fixed::_impl::pointer_iterator<char>>();

	test_pointer_iterator<fixed::_impl::pointer_iterator>();
	test_pointer_iterator<fixed::_impl::const_pointer_iterator>();

	test_not_const<fixed::_impl::wrap_pointer_iterator<char, fixed::_impl::pointer_iterator<char>>>();
	test_not_const<fixed::_impl::const_wrap_pointer_iterator<char, fixed::_impl::const_pointer_iterator<char>>>();

	test_assign<fixed::_impl::const_wrap_pointer_iterator<char, fixed::_impl::const_pointer_iterator<char>>
		, fixed::_impl::wrap_pointer_iterator<char, fixed::_impl::pointer_iterator<char>>>();

	test_wrap_iterator();
}
