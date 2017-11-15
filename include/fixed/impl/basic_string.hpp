#ifndef FIXED_BASIC_STRING_HPP
#define FIXED_BASIC_STRING_HPP

#include <string>
#include <type_traits>
#include <utility>

#include "fixed/allocation_pattern.hpp"
#include "fixed/impl/fixed_def.hpp"
#include "fixed/impl/fixed_type_traits.hpp"
#include "fixed/vector.hpp"

namespace fixed
{
namespace _impl
{
    template <typename CHAR_T, size_t SIZE,
        typename Traits = std::char_traits<CHAR_T>,
        template <typename, size_t> typename Alloc_pattern
        = fixed::stack_allocator>
    class basic_string
    {
    public:
        typedef Traits traits_type;

        template <typename T, size_t ASIZE>
        using allocator_type = Alloc_pattern<T, ASIZE>;

    private:
        typedef allocator_type<CHAR_T, SIZE + 1> data_type;

    public:
        typedef typename data_type::value_type value_type;
        typedef typename data_type::aligned_type aligned_type;
        typedef typename data_type::size_type size_type;
        typedef typename data_type::difference_type difference_type;
        typedef typename data_type::reference reference;
        typedef typename data_type::const_reference const_reference;
        typedef typename data_type::pointer pointer;
        typedef typename data_type::const_pointer const_pointer;
        typedef typename data_type::iterator iterator;
        typedef typename data_type::const_iterator const_iterator;

        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    private:
        data_type _data;
        size_type _size;

        static_assert(std::is_trivial<CHAR_T>::value,
            "character type required to be trivial");
        static_assert(fixed::_impl::is_alloc_pattern_contiguous_v<data_type>,
            "Alloc_pattern required to be continuous for this container");

    public:
        constexpr basic_string() noexcept(
            is_nothrow_default_constructible_v<data_type>)
            : _data()
        {
        }

        template <typename Alloc_source,
            std::enable_if_t<is_allocation_source_v<Alloc_source>, int> = 0>
        constexpr basic_string(Alloc_source& source) noexcept(
            std::is_nothrow_constructible<data_type, Alloc_source&>::value)
            : _data(source)
        {
        }

        constexpr basic_string(size_type count, value_type ch)
            : _data()
        {
            assign(count, ch);
        }

        template <typename Alloc_source,
            std::enable_if_t<is_allocation_source_v<Alloc_source>, int> = 0>
        constexpr basic_string(
            size_type count, value_type ch, Alloc_source& source)
            : _data(source)
        {
            assign(count, ch);
        }

        constexpr basic_string& assign(size_type count, value_type ch)
        {
            FIXED_CHECK_FULL(count <= max_size());

            auto itbeg = _data.begin();
            for(size_type i = 0; i < count; i++)
            {
                *itbeg = ch;
                ++itbeg;
            }
            *itbeg = '\0';
            _size = count;
            return *this;
        }

		constexpr basic_string(const basic_string& str)
			: basic_string()
		{
			std::copy(str.begin(), str.end(), _data.begin());
		}

		constexpr basic_string& operator=(const basic_string& str)
		{
			if (this != &str)
			{
				std::copy(str.begin(), str.end(), _data.begin());
			}
			return *this;
		}

		constexpr basic_string(basic_string&& other) noexcept(
			is_nothrow_default_constructible_v<data_type> 
			&& (is_nothrow_move_constructible_v<data_type> 
				|| (is_nothrow_allocator_iterator_v<data_type> && is_nothrow_cpy_move_constructible_v<CHAR_T>)))
			: basic_string()
		{
			operator=(std::move(other));
		}

		constexpr basic_string& operator=(basic_string&& other) noexcept(
			is_nothrow_default_constructible_v<data_type>
			&& (is_nothrow_move_constructible_v<data_type>
				|| (is_nothrow_allocator_iterator_v<data_type> && is_nothrow_cpy_move_constructible_v<CHAR_T>)))
		{
			if (this != &other)
			{
				fixed::astd::constexpr_if<
					std::is_nothrow_move_constructible<data_type>::value>(
						[this, &other]() {
					std::swap(_data, other._data);
					std::swap(_size, other._size);
				})
					._else([this, &other]() {
					clear();
					for (auto& val : other)
					{
						push_back(std::move_if_noexcept(val));
					}
					other.clear();
				});
			}
			return *this;
		}


		//Iterators
        constexpr iterator begin() noexcept { return _data.begin(); }

        constexpr const_iterator begin() const noexcept { return cbegin(); }

        constexpr const_iterator cbegin() const noexcept { return _data.begin(); }

        constexpr iterator end() noexcept { return _data.begin() + _size; }

        constexpr const_iterator end() const noexcept { return cend(); }

        constexpr const_iterator cend() noexcept { return _data.begin() + _size; }

		constexpr reverse_iterator rbegin() noexcept {
			return reverse_iterator(end());
		}

		constexpr reverse_iterator rend() noexcept {
			return reverse_iterator(begin());
		}

		constexpr const_reverse_iterator rbegin() const noexcept {
			return const_reverse_iterator(cend());
		}

		constexpr const_reverse_iterator rend() const noexcept {
			return const_reverse_iterator(cbegin());
		}


		constexpr const_reverse_iterator crbegin() const noexcept {
			return const_reverse_iterator(cend());
		}

		constexpr const_reverse_iterator crend() const noexcept {
			return const_reverse_iterator(cbegin());
		}

		//Capacity
        constexpr size_type size() const noexcept { return _size; }

		constexpr size_type length() const noexcept { return size(); }

        constexpr size_type max_size() const noexcept { return _data.max_size() - 1; }

		void resize(size_type s)
		{
			resize(s, CHAR_T());
		}
		
		void resize(size_type s, const CHAR_T& val)
		{
			while(_size > s)
			{
				pop_back();
			}

			while(_size < s)
			{
				push_back(val);
			}
		}

		constexpr size_type capacity() const noexcept
		{
			return max_size() - size();
		}

		constexpr void clear() const noexcept
		{
			while (_size)
			{
				pop_back();
			}
		}

		constexpr bool empty() const noexcept
		{
			return size() == 0;
		}

		constexpr void shrink_to_fit() const noexcept
		{
		//nop
		}

		//String operations
		constexpr aligned_type* data() { return _data.data(); }

		constexpr const aligned_type* data() const { return _data.data(); }

	};
}
}

#endif //! FIXED_BASIC_STRING_HPP
