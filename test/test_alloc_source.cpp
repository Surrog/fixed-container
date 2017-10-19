

#include "fixed/impl/basic_allocation_sources.hpp"
#include "fixed/impl/fixed_def.hpp"
#include "fixed/impl/fixed_type_traits.hpp"
#include "catch.hpp"
#include "test_struct.hpp"

TEST_CASE("test fixed allocation_source", "[alloc]")
{
	static_assert(fixed::_impl::is_allocation_source<fixed::_impl::empty_source>::value, "empty source is supposed to be allocation_source");
	static_assert(fixed::_impl::is_allocation_source_v<fixed::_impl::empty_source>, "empty source is supposed to be allocation_source");

	static_assert(!fixed::_impl::is_allocation_source<test>::value, "test is not supposed to be allocation_source");
	static_assert(!fixed::_impl::is_allocation_source_v<test>, "test is not supposed to be allocation_source");

}
