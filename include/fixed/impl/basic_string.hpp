#ifndef FIXED_BASIC_STRING_HPP
#define FIXED_BASIC_STRING_HPP

#include <string>
#include <type_traits>

#include "fixed/impl/fixed_def.hpp"
#include "fixed/vector.hpp"
#include "fixed/allocation_pattern.hpp"

namespace fixed
{
namespace _impl
{
    template <typename CHAR_T, size_t SIZE,
        typename Traits = std::char_traits<CHAR_T>,
		template <typename, size_t> typename Alloc_pattern = fixed::stack_allocator
	>
		class basic_string
	{
		static_assert(std::is_trivial<CHAR_T>::value, "basic_string require that the character type to be trivial");

		typedef fixed::vector<CHAR_T, SIZE, Alloc_pattern> data_type;
		data_type _data;

	public:
		typedef Traits traits_type;
		typedef CHAR_T value_type;

		template <typename T, size_t ASIZE>
		using allocator_type = Alloc_pattern<T, ASIZE>;

		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;

		typedef typename fixed::vector<CHAR_T, SIZE, Alloc_pattern>::iterator iterator;
		typedef typename fixed::vector<CHAR_T, SIZE, Alloc_pattern>::const_iterator const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		constexpr basic_string() noexcept(is_nothrow_default_constructible_v<data_type>)
			: _data()
		{}

		template <typename Alloc_source,
			std::enable_if_t<is_allocation_source_v<Alloc_source>, int> = 0>
		constexpr basic_string(Alloc_source& source)
			noexcept(std::is_nothrow_constructible<data_type, Alloc_source&>::value)
			: _data(source)
		{}
    };
}
}

#endif //! FIXED_BASIC_STRING_HPP