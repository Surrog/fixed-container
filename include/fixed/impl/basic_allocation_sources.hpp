#ifndef FIXED_BASIC_ALLOCATION_SOURCES
#define FIXED_BASIC_ALLOCATION_SOURCES

#include <type_traits>

namespace fixed
{
	namespace _impl
	{
		template <typename T, class = void>
		struct is_allocator_source : public std::bool_constant<false>
		{};

		template <typename T>
		struct is_allocator_source<T, std::void_t<typename T::allocator_source>> : public std::bool_constant<true>
		{};

		struct allocator_source_tag
		{};

		struct empty_source
		{
			typedef allocator_source_tag allocator_source;
		};

	}
}

#endif //FIXED_BASIC_ALLOCATION_SOURCES