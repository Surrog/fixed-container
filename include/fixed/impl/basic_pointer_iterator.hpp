#ifndef FIXED_VECTOR_ITERATORS_HPP
#define FIXED_VECTOR_ITERATORS_HPP

#include "fixed/impl/fixed_def.hpp"

namespace fixed
{
namespace _impl
{
    template <typename T, typename POINTER = T> struct pointer_iterator
    {
    public:
        typedef T value_type;
        typedef POINTER inner_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef std::ptrdiff_t difference_type;
        typedef std::random_access_iterator_tag iterator_category;

        constexpr bool operator==(const pointer_iterator& rval) const
        {
            return _value == rval._value;
        }

		constexpr bool operator!=(const pointer_iterator& rval) const
        {
            return !operator==(rval);
        }

		constexpr value_type* get() { return reinterpret_cast<value_type*>(_value); }
		constexpr const value_type* get() const
        {
            return reinterpret_cast<const value_type*>(_value);
        }

		constexpr reference operator*() const
        {
            FIXED_CHECK_ITERATOR(_value != nullptr);
            return reinterpret_cast<reference>(*_value);
        }

		constexpr value_type* operator->() { return get(); }
		constexpr const value_type* operator->() const { return get(); }

        constexpr pointer_iterator() noexcept = default;

		constexpr pointer_iterator(inner_type* val) noexcept
            : _value(val)
        {}

		constexpr pointer_iterator(const pointer_iterator&) noexcept = default;
		constexpr pointer_iterator(pointer_iterator&&) noexcept = default;
		constexpr pointer_iterator& operator=(const pointer_iterator&) noexcept = default;
		constexpr pointer_iterator& operator=(pointer_iterator&&) noexcept = default;

		constexpr pointer_iterator& operator++()
        {
            FIXED_CHECK_ITERATOR(_value != nullptr);
            ++_value;
            return *this;
        }

		constexpr pointer_iterator operator++(int)
        {
            FIXED_CHECK_ITERATOR(_value != nullptr);
            pointer_iterator result = *this;
            ++(*this);
            return result;
        }

		constexpr pointer_iterator& operator--()
        {
            FIXED_CHECK_ITERATOR(_value != nullptr);
            --_value;
            return *this;
        }

		constexpr pointer_iterator operator--(int)
        {
            FIXED_CHECK_ITERATOR(_value != nullptr);
            pointer_iterator result = *this;
            --(*this);
            return result;
        }

		constexpr pointer_iterator& operator+=(difference_type n)
        {
            FIXED_CHECK_ITERATOR(_value != nullptr || n == 0);
            _value += n;
            return *this;
        }

		constexpr pointer_iterator operator+(difference_type n) const
        {
            FIXED_CHECK_ITERATOR(_value != nullptr || n == 0);
            return pointer_iterator(_value + n);
        }

		constexpr pointer_iterator& operator-=(difference_type n)
        {
            FIXED_CHECK_ITERATOR(_value != nullptr || n == 0);
            _value -= n;
            return *this;
        }

		constexpr pointer_iterator operator-(difference_type n) const
        {
            FIXED_CHECK_ITERATOR(_value != nullptr || n == 0);
			return pointer_iterator(_value - n);
        }

		constexpr difference_type operator-(const pointer_iterator& rval) const
        {
            return _value - rval._value;
        }

		constexpr reference operator[](difference_type n)
        {
            FIXED_CHECK_ITERATOR(_value != nullptr);
            return reinterpret_cast<reference>(_value[n]);
        }

		constexpr bool operator<(const pointer_iterator& rval) const
        {
            return _value < rval._value;
        }

		constexpr bool operator>(const pointer_iterator& rval) const
        {
            return _value > rval._value;
        }

		constexpr bool operator<=(const pointer_iterator& rval) const
        {
            return _value <= rval._value;
        }

		constexpr bool operator>=(const pointer_iterator& rval) const
        {
            return _value >= rval._value;
        }

    private:
        inner_type* _value = nullptr;
    };

    template <typename T, typename POINTER = T> struct const_pointer_iterator
    {
    public:
        typedef T value_type;
        typedef POINTER inner_type;
        typedef const T* pointer;
        typedef const T& reference;
        typedef std::ptrdiff_t difference_type;
        typedef std::random_access_iterator_tag iterator_category;

		constexpr bool operator==(const const_pointer_iterator& rval) const
        {
            return _value == rval._value;
        }

		constexpr bool operator!=(const const_pointer_iterator& rval) const
        {
            return !operator==(rval);
        }

		constexpr const value_type* get() const { return reinterpret_cast<value_type*>(_value); }

		constexpr const reference operator*() const
        {
            FIXED_CHECK_ITERATOR(_value != nullptr);
            return reinterpret_cast<const reference>(*_value);
        }

		constexpr const value_type* operator->() const { return get(); }

		constexpr const_pointer_iterator() = default;

		constexpr const_pointer_iterator(const inner_type* val)
            : _value(const_cast<inner_type*>(val))
        {}

		constexpr const_pointer_iterator(const const_pointer_iterator&) = default;
		constexpr const_pointer_iterator(const_pointer_iterator&&) noexcept = default;
		constexpr const_pointer_iterator& operator=(const const_pointer_iterator&)
            = default;
		constexpr const_pointer_iterator& operator=(const_pointer_iterator&&) noexcept
            = default;

		constexpr const_pointer_iterator(pointer_iterator<value_type>&& orig)
			: const_pointer_iterator(reinterpret_cast<const inner_type*>(orig.get()))
		{
			orig = pointer_iterator<value_type>();
		}

		constexpr const_pointer_iterator(const pointer_iterator<value_type>& orig)
            : const_pointer_iterator(reinterpret_cast<const inner_type*>(orig.get()))
        {
        }

		constexpr const_pointer_iterator& operator++()
        {
            FIXED_CHECK_ITERATOR(_value != nullptr);
            ++_value;
            return *this;
        }

		constexpr const_pointer_iterator operator++(int)
        {
            FIXED_CHECK_ITERATOR(_value != nullptr);
            const_pointer_iterator result = (*this);
            ++(*this);
            return result;
        }

		constexpr const_pointer_iterator& operator--()
        {
            FIXED_CHECK_ITERATOR(_value != nullptr);
            _value--;
            return *this;
        }

		constexpr const_pointer_iterator operator--(int)
        {
            FIXED_CHECK_ITERATOR(_value != nullptr);
            const_pointer_iterator result = (*this);
            --(*this);
            return result;
        }

		constexpr const_pointer_iterator& operator+=(difference_type n)
        {
            FIXED_CHECK_ITERATOR(_value != nullptr || n == 0);
            _value += n;
            return *this;
        }

		constexpr const_pointer_iterator operator+(difference_type n) const
        {
            FIXED_CHECK_ITERATOR(_value != nullptr || n == 0);
            return {_value + n};
        }

		constexpr const_pointer_iterator& operator-=(difference_type n)
        {
            FIXED_CHECK_ITERATOR(_value != nullptr || n == 0);
            _value -= n;
            return *this;
        }

		constexpr const_pointer_iterator operator-(difference_type n) const
        {
            FIXED_CHECK_ITERATOR(_value != nullptr || n == 0);
            return const_pointer_iterator(_value - n);
        }

		constexpr difference_type operator-(const const_pointer_iterator& rval) const
        {
            return _value - rval._value;
        }

		constexpr reference operator[](difference_type n)
        {
            FIXED_CHECK_ITERATOR(_value != nullptr);
            return reinterpret_cast<reference>(_value[n]);
        }

		constexpr bool operator<(const const_pointer_iterator& rval) const
        {
            return _value < rval._value;
        }

		constexpr bool operator>(const const_pointer_iterator& rval) const
        {
            return _value > rval._value;
        }

		constexpr bool operator<=(const const_pointer_iterator& rval) const
        {
            return _value <= rval._value;
        }

		constexpr bool operator>=(const const_pointer_iterator& rval) const
        {
            return _value >= rval._value;
        }

    private:
        inner_type* _value = nullptr;
    };

	template <typename T>
	auto operator+(std::ptrdiff_t lval, const pointer_iterator<T>& rval)
	{
		return rval + lval;
	}

	template <typename T>
	auto operator+(std::ptrdiff_t lval, const const_pointer_iterator<T>& rval)
	{
		return rval + lval;
	}

}
}

#endif //! FIXED_VECTOR_ITERATORS_HPP