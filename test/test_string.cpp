
#include "catch.hpp"
#include "test_struct.hpp"
#include "fixed/impl/basic_string.hpp"
#include "fixed/impl/basic_allocation_sources.hpp"

template <template <typename, fixed::_impl::size_t, typename, 
	template <typename, fixed::_impl::size_t> typename > typename STRING_T,
	template <typename, fixed::_impl::size_t> typename Alloc_pattern>
void test_string_constructor()
{
	{
		STRING_T<char, 10, std::char_traits<char>, Alloc_pattern> string;
	}
	{
		STRING_T<wchar_t, 10, std::char_traits<wchar_t>, Alloc_pattern> string;
	}
	{
		STRING_T<char16_t, 10, std::char_traits<char16_t>, Alloc_pattern> string;
	}
	{
		STRING_T<char32_t, 10, std::char_traits<char32_t>, Alloc_pattern> string;
	}

	{
		fixed::_impl::empty_source src;
		STRING_T<char, 10, std::char_traits<char>, Alloc_pattern> string(src);
	}
	{
		fixed::_impl::empty_source src;
		STRING_T<wchar_t, 10, std::char_traits<wchar_t>, Alloc_pattern> string(src);
	}
	{
		fixed::_impl::empty_source src;
		STRING_T<char16_t, 10, std::char_traits<char16_t>, Alloc_pattern> string(src);
	}
	{
		fixed::_impl::empty_source src;
		STRING_T<char32_t, 10, std::char_traits<char32_t>, Alloc_pattern> string(src);
	}

	//{	
	//	STRING_T<char, 10, std::char_traits<char>, Alloc_pattern> string(fixed::_impl::empty_source{});
	//}
	//{
	//	STRING_T<wchar_t, 10, std::char_traits<wchar_t>, Alloc_pattern> string(fixed::_impl::empty_source{});
	//}
	//{
	//	STRING_T<char16_t, 10, std::char_traits<char16_t>, Alloc_pattern> string(fixed::_impl::empty_source{});
	//}
	//{
	//	STRING_T<char32_t, 10, std::char_traits<char32_t>, Alloc_pattern> string(fixed::_impl::empty_source{});
	//}
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