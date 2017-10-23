
#include "catch.hpp"
#include "test_struct.hpp"
#include "fixed/impl/basic_string.hpp"
#include "fixed/impl/basic_allocation_sources.hpp"

template <typename CHAR_T,
	template <typename, fixed::_impl::size_t, typename,
	template <typename, fixed::_impl::size_t> typename > typename STRING_T,
	template <typename, fixed::_impl::size_t> typename Alloc_pattern>
void test_string_constructor()
{
	{
		STRING_T<CHAR_T, 10, std::char_traits<CHAR_T>, Alloc_pattern> string;
	}
	{
		fixed::_impl::empty_source src;
		STRING_T<CHAR_T, 10, std::char_traits<CHAR_T>, Alloc_pattern> string(src);
	}

	{
		STRING_T<CHAR_T, 10, std::char_traits<CHAR_T>, Alloc_pattern> string(10, 'a');

		CHECK(std::all_of(string.begin(), string.end(), [](const CHAR_T& c) { return c == CHAR_T('a'); }));
		CHECK(string.size() == 10);
		CHECK(*(string.begin() + 10) == '\0');
	}

	{
#ifdef FIXED_CONTAINER_CHECK_BOUND
		try
		{
			STRING_T<CHAR_T, 2, std::char_traits<CHAR_T>, Alloc_pattern> string(10, 'a');
			CHECK(false);
		}
		catch (std::exception& )
		{
			CHECK(true);
		}
#endif
	}
	{
		fixed::_impl::empty_source src;
		STRING_T<CHAR_T, 10, std::char_traits<CHAR_T>, Alloc_pattern> string(10, 'a', src);

		CHECK(std::all_of(string.begin(), string.end(), [](const CHAR_T& c) { return c == CHAR_T('a'); }));
		CHECK(string.size() == 10);
		CHECK(*(string.begin() + 10) == '\0');
	}
	{
#ifdef FIXED_CONTAINER_CHECK_BOUND
		try
		{
			fixed::_impl::empty_source src;
			STRING_T<CHAR_T, 2, std::char_traits<CHAR_T>, Alloc_pattern> string(10, 'a', src);
			CHECK(false);
		}
		catch (std::exception& )
		{
			CHECK(true);
		}
#endif
	}
}	

template <template <typename, fixed::_impl::size_t, typename, 
	template <typename, fixed::_impl::size_t> typename > typename STRING_T,
	template <typename, fixed::_impl::size_t> typename Alloc_pattern>
void test_string_constructor()
{
	test_string_constructor<char, STRING_T, Alloc_pattern>();
	test_string_constructor<wchar_t, STRING_T, Alloc_pattern>();
	test_string_constructor<char16_t, STRING_T, Alloc_pattern>();
	test_string_constructor<char32_t, STRING_T, Alloc_pattern>();
}

template <template <typename, fixed::_impl::size_t, typename,
	template <typename, fixed::_impl::size_t> typename > typename STRING_T,
	template <typename, fixed::_impl::size_t> typename Alloc_pattern>
void test_string_noexcept_constructor()
{
	static_assert(std::is_nothrow_default_constructible<STRING_T<char, 10, std::char_traits<char>, Alloc_pattern>>::value, "Not noexcept default constructible");
	static_assert(std::is_nothrow_move_constructible<STRING_T<char, 10, std::char_traits<char>, Alloc_pattern>>::value, "Not noexcept move constructible");
	static_assert(std::is_nothrow_move_assignable<STRING_T<char, 10, std::char_traits<char>, Alloc_pattern>>::value, "Not noexcept move assignable");

	static_assert(std::is_nothrow_default_constructible<STRING_T<int, 10, std::char_traits<int>, Alloc_pattern>>::value, "Not noexcept default constructible");
	static_assert(std::is_nothrow_move_constructible<STRING_T<int, 10, std::char_traits<int>, Alloc_pattern>>::value, "Not noexcept move constructible");
	static_assert(std::is_nothrow_move_assignable<STRING_T<int, 10, std::char_traits<int>, Alloc_pattern>>::value, "Not noexcept move assignable");
}


template <template <typename, fixed::_impl::size_t, typename,
	template <typename, fixed::_impl::size_t> typename > typename STRING_T,
	template <typename, fixed::_impl::size_t> typename Alloc_pattern>
void test_string()
{
	test_string_noexcept_constructor<STRING_T, Alloc_pattern>();
	test_string_constructor<STRING_T, Alloc_pattern>();
}


TEST_CASE("test string", "[string]")
{
	test_string<fixed::_impl::basic_string, fixed::_impl::aligned_stack_allocator>();
	test_string<fixed::_impl::basic_string, fixed::_impl::aligned_heap_allocator>();
	test_string<fixed::_impl::basic_string, fixed::_impl::constexpr_stack_allocator>();
}