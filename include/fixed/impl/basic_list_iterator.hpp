#ifndef FIXED_BASIC_LIST_ITERATOR_HPP
#define FIXED_BASIC_LIST_ITERATOR_HPP

#include <cassert>
#include <type_traits>

namespace fixed
{
namespace _impl
{
    template <typename T, typename STORAGE_T> struct basic_list_iterator
    {
    public:
        typedef T value_type;
        typedef typename std::aligned_storage<sizeof(value_type),
            alignof(value_type)>::type aligned_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef std::ptrdiff_t difference_type;
        typedef std::random_access_iterator_tag iterator_category;

        bool operator==(const basic_list_iterator& rval) const
        {
            return _value == rval._value;
        }

        bool operator!=(const basic_list_iterator& rval) const
        {
            return !operator==(rval);
        }

        const STORAGE_T& get() const { return _value; }

        reference operator*() const
        {
            assert(_value != nullptr);
            return reinterpret_cast<reference>(**_value);
        }

        pointer operator->() const
        {
            return reinterpret_cast<pointer>(*_value);
        }

        basic_list_iterator() = default;

        basic_list_iterator(const STORAGE_T& val)
            : _value(val)
        {}

		basic_list_iterator(STORAGE_T&& val)
			: _value(std::move(val))
		{}

        basic_list_iterator(const basic_list_iterator&) = default;
        basic_list_iterator(basic_list_iterator&&) noexcept = default;
        basic_list_iterator& operator=(const basic_list_iterator&) = default;
        basic_list_iterator& operator=(basic_list_iterator&&) noexcept
            = default;

        basic_list_iterator& operator++()
        {
            assert(_value != nullptr);
            ++_value;
            return *this;
        }

        basic_list_iterator operator++(int)
        {
            assert(_value != nullptr);
            basic_list_iterator result = (*this);
            ++(*this);
            return result;
        }

        basic_list_iterator& operator--()
        {
            assert(_value != nullptr);
            _value--;
            return *this;
        }

        basic_list_iterator operator--(int)
        {
            assert(_value != nullptr);
            basic_list_iterator result = (*this);
            --(*this);
            return result;
        }

        basic_list_iterator& operator+=(difference_type n)
        {
            assert(_value != nullptr);
            _value += n;
            return *this;
        }

        basic_list_iterator operator+(difference_type n) const
        {
            assert(_value != nullptr);
            return {_value + n};
        }

        basic_list_iterator& operator-=(difference_type n)
        {
            assert(_value != nullptr);
            _value -= n;
            return *this;
        }

        basic_list_iterator operator-(difference_type n) const
        {
            assert(_value != nullptr);
            return {_value - n};
        }

        difference_type operator-(const basic_list_iterator& rval) const
        {
            return _value - rval._value;
        }

        reference operator[](difference_type n)
        {
            assert(_value != nullptr);
            return reinterpret_cast<reference>(*_value[n]);
        }

        bool operator<(const basic_list_iterator& rval) const
        {
            return _value < rval._value;
        }

        bool operator>(const basic_list_iterator& rval) const
        {
            return _value > rval._value;
        }

        bool operator<=(const basic_list_iterator& rval) const
        {
            return _value <= rval._value;
        }

        bool operator>=(const basic_list_iterator& rval) const
        {
            return _value >= rval._value;
        }

    private:
        STORAGE_T _value = STORAGE_T();
    };

    template <typename T, typename STORAGE_T> struct const_basic_list_iterator
    {
    public:
        typedef T value_type;
        typedef const value_type* pointer;
        typedef const value_type& reference;
        typedef std::ptrdiff_t difference_type;
        typedef std::random_access_iterator_tag iterator_category;

        bool operator==(const const_basic_list_iterator& rval) const
        {
            return _value == rval._value;
        }

        bool operator!=(const const_basic_list_iterator& rval) const
        {
            return !operator==(rval);
        }

        const STORAGE_T* get() const { return _value; }

        reference operator*() const
        {
            assert(_value != nullptr);
            return **_value;
        }

        pointer operator->() const
        {
            return *_value;
        }

        const_basic_list_iterator() = default;

        const_basic_list_iterator(const STORAGE_T val)
            : _value(val)
        {
        }

        const_basic_list_iterator(const const_basic_list_iterator&) = default;
        const_basic_list_iterator(const_basic_list_iterator&&) noexcept = default;
        const_basic_list_iterator& operator=(const const_basic_list_iterator&) = default;
        const_basic_list_iterator& operator=(const_basic_list_iterator&&) noexcept = default;

		template <typename STO>
        const_basic_list_iterator(const basic_list_iterator<T, STO>& orig)
            : _value(orig.get())
        {
        }

		template <typename STO>
		const_basic_list_iterator(basic_list_iterator<T, STO>&& orig)
			: _value(orig.get())
		{
		}



        const_basic_list_iterator& operator++()
        {
            assert(_value != nullptr);
            ++_value;
            return *this;
        }

        const_basic_list_iterator operator++(int)
        {
            assert(_value != nullptr);
            const_basic_list_iterator result = (*this);
            ++(*this);
            return result;
        }

        const_basic_list_iterator& operator--()
        {
            assert(_value != nullptr);
            --_value;
            return *this;
        }

        const_basic_list_iterator operator--(int)
        {
            assert(_value != nullptr);
            const_basic_list_iterator result = (*this);
            --(*this);
            return result;
        }

        const_basic_list_iterator& operator+=(difference_type n)
        {
            assert(_value != nullptr);
            _value += n;
            return *this;
        }

        const_basic_list_iterator operator+(difference_type n) const
        {
            assert(_value != nullptr);
            return {_value + n};
        }

        const_basic_list_iterator& operator-=(difference_type n)
        {
            assert(_value != nullptr);
            _value -= n;
            return *this;
        }

        const_basic_list_iterator operator-(difference_type n) const
        {
            assert(_value != nullptr);
            return const_basic_list_iterator{_value - n};
        }

        difference_type operator-(const const_basic_list_iterator& rval) const
        {
            return _value - rval._value;
        }

        reference operator[](difference_type n)
        {
            assert(_value != nullptr);

            return *_value[n];
        }

        bool operator<(const const_basic_list_iterator& rval) const
        {
            return _value < rval._value;
        }

        bool operator>(const const_basic_list_iterator& rval) const
        {
            return _value > rval._value;
        }

        bool operator<=(const const_basic_list_iterator& rval) const
        {
            return _value <= rval._value;
        }

        bool operator>=(const const_basic_list_iterator& rval) const
        {
            return _value >= rval._value;
        }

    private:
        STORAGE_T _value;
    };

    template <typename T, typename STO>
    auto operator+(std::ptrdiff_t lval, const basic_list_iterator<T, STO>& rval)
    {
        return rval + lval;
    }

    template <typename T, typename STO>
    auto operator+(
        std::ptrdiff_t lval, const const_basic_list_iterator<T, STO>& rval)
    {
        return rval + lval;
    }
}
}

#endif //! FIXED_BASIC_LIST_ITERATOR_HPP