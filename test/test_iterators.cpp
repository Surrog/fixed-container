#include "fixed/impl/iterator.hpp"
#include "catch.hpp"

void test_pointer_allocator()
{
	{
		fixed::_impl::pointer_iterator<char> it;

		CHECK(fixed::_impl::is_iterator<fixed::_impl::pointer_iterator<char>>::value);
		CHECK(!std::is_const<fixed::_impl::pointer_iterator<char>::value_type>::value);
		CHECK(!std::is_const<fixed::_impl::pointer_iterator<char>::pointer>::value);
		CHECK(!std::is_const<fixed::_impl::pointer_iterator<char>::reference>::value);
	}

	{
		fixed::_impl::pointer_iterator<char> it;
		fixed::_impl::pointer_iterator<char> it2;
		CHECK(it == it2);
		CHECK(it != it2);
	}

	{
		char test[256] = "test";

		fixed::_impl::pointer_iterator<char> it(test);
	}
}


TEST_CASE("testing allocator", "[utility]")
{


}
