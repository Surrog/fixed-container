#ifndef FIXED_BASIC_STRING_HPP
#define FIXED_BASIC_STRING_HPP

#include <string>
#include <type_traits>

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

        constexpr iterator begin() { return _data.begin(); }

        constexpr const_iterator begin() const { return cbegin(); }

        constexpr const_iterator cbegin() const { return _data.begin(); }

        constexpr iterator end() { return _data.begin() + _size; }

        constexpr const_iterator end() const { return cend(); }

        constexpr const_iterator cend() { return _data.begin() + _size; }

        constexpr aligned_type* data() { return _data.data(); }

        constexpr const aligned_type* data() const { return _data.data(); }

        constexpr size_type size() const noexcept { return _size; }

        constexpr size_type max_size() const noexcept
        {
            return _data.max_size() - 1;
        }
    };
}
}

#endif //! FIXED_BASIC_STRING_HPP
