#ifndef HEAPLESS_ITERATOR_HPP
#define HEAPLESS_ITERATOR_HPP

#include <cstddef>
#include <type_traits>
#include <iterator>
#include "fixed_type_traits.hpp"

namespace fixed
{
	namespace _impl
	{
		template <typename T, class = void>
		struct is_iterator : public std::bool_constant<false>
		{};

		template <typename T>
		struct is_iterator<T, astd::void_t<typename std::iterator_traits<T>::iterator_category>> : public std::bool_constant<true>
		{};

		template <typename T>
		struct pointer_iterator
		{
			typedef typename std::remove_pointer<T>::type rp_value_type;

		public:
			typedef typename std::remove_const<rp_value_type>::type value_type;
			typedef rp_value_type* pointer;
			typedef rp_value_type& reference;
			typedef std::ptrdiff_t difference_type;
			typedef std::random_access_iterator_tag iterator_category;
			constexpr static bool is_const = astd::is_const_v<rp_value_type>;

			bool operator==(const pointer_iterator& rval) const
			{
				return _value == rval._value;
			}

			bool operator!=(const pointer_iterator& rval) const
			{
				return !operator==(rval);
			}

			reference get() const
			{
				return *_value;
			}

			reference operator*() const
			{
				assert(_value != nullptr);
				return *_value;
			}

			pointer operator->() const
			{
				return _value;
			}

			pointer_iterator() = default;

			pointer_iterator(const T* val) 
				: _value(const_cast<value_type*>(val))
			{}

			pointer_iterator(const pointer_iterator&) = default;
			pointer_iterator(pointer_iterator&&) noexcept = default;
			pointer_iterator& operator=(const pointer_iterator&) = default;
			pointer_iterator& operator=(pointer_iterator&&) noexcept = default;

			template <class = std::enable_if_t<is_const>>
			pointer_iterator(const pointer_iterator<value_type>& orig)
				: _value(&orig.get())
			{}

			pointer_iterator& operator++()
			{
				assert(_value != nullptr);
				++_value;
				return *this;
			}

			pointer_iterator operator++(int) const
			{
				assert(_value != nullptr);
				return { _value + 1 };
			}


			pointer_iterator& operator--()
			{
				assert(_value != nullptr);
				_value--;
				return *this;
			}

			pointer_iterator operator--(int) const 
			{
				assert(_value != nullptr);
				return { _value - 1 };
			}

			pointer_iterator& operator+=(difference_type n)
			{
				assert(_value != nullptr);
				_value += n;
				return *this;
			}

			pointer_iterator operator+(difference_type n) const
			{
				assert(_value != nullptr);
				return { _value + n };
			}

			pointer_iterator& operator-=(difference_type n)
			{
				assert(_value != nullptr);
				_value -= n;
				return *this;
			}

			pointer_iterator operator-(difference_type n) const
			{
				assert(_value != nullptr);
				return { _value - n };
			}

			difference_type operator-(const pointer_iterator& rval) const
			{
				return _value - rval._value;
			}

			reference operator[](difference_type n)
			{
				assert(_value != nullptr);
				return _value[n];
			}

			bool operator<(const pointer_iterator& rval) const
			{
				return _value < rval._value;
			}

			bool operator>(const pointer_iterator& rval) const
			{
				return _value > rval._value;
			}

			bool operator<=(const pointer_iterator& rval) const
			{
				return _value <= rval._value;
			}

			bool operator>=(const pointer_iterator& rval) const
			{
				return _value >= rval._value;
			}

		private:
			value_type* _value = nullptr;
		};

		template <typename T>
		auto operator+(std::ptrdiff_t lval, const pointer_iterator<T>& rval)
		{
			return rval + lval;
		}

		template <typename T>
		struct wrap_pointer_iterator
		{
			typedef typename T::rp_value_type rp_value_type;

		public:
			typedef typename T::value_type value_type;
			typedef typename T::pointer pointer;
			typedef typename T::reference reference;
			typedef typename T::difference_type difference_type;
			typedef typename T::iterator_category iterator_category;
			constexpr static bool is_const = T::is_const;

			bool operator==(const wrap_pointer_iterator& rval) const
			{
				return _value == rval._value;
			}

			bool operator!=(const wrap_pointer_iterator& rval) const
			{
				return !operator==(rval);
			}

			reference get() const
			{
				return *_value;
			}

			reference operator*() const
			{
				assert(_value != nullptr);
				return *_value;
			}

			pointer operator->() const
			{
				return _value;
			}

			wrap_pointer_iterator() = default;

			wrap_pointer_iterator(const T& val)
				: _value(val)
			{}

			wrap_pointer_iterator(const wrap_pointer_iterator&) = default;
			wrap_pointer_iterator(wrap_pointer_iterator&&) noexcept = default;
			wrap_pointer_iterator& operator=(const wrap_pointer_iterator&) = default;
			wrap_pointer_iterator& operator=(wrap_pointer_iterator&&) noexcept = default;

			template <class = std::enable_if_t<is_const>>
			wrap_pointer_iterator(const wrap_pointer_iterator<value_type>& orig)
				: _value(&orig.get())
			{}

			wrap_pointer_iterator& operator++()
			{
				assert(_value != nullptr);
				++_value;
				return *this;
			}

			wrap_pointer_iterator operator++(int) const
			{
				assert(_value != nullptr);
				return { _value + 1 };
			}


			wrap_pointer_iterator& operator--()
			{
				assert(_value != nullptr);
				_value--;
				return *this;
			}

			wrap_pointer_iterator operator--(int) const
			{
				assert(_value != nullptr);
				return { _value - 1 };
			}

			wrap_pointer_iterator& operator+=(difference_type n)
			{
				assert(_value != nullptr);
				_value += n;
				return *this;
			}

			wrap_pointer_iterator operator+(difference_type n) const
			{
				assert(_value != nullptr);
				return { _value + n };
			}

			wrap_pointer_iterator& operator-=(difference_type n)
			{
				assert(_value != nullptr);
				_value -= n;
				return *this;
			}

			wrap_pointer_iterator operator-(difference_type n) const
			{
				assert(_value != nullptr);
				return { _value - n };
			}

			difference_type operator-(const wrap_pointer_iterator& rval) const
			{
				return _value - rval._value;
			}

			reference operator[](difference_type n)
			{
				assert(_value != nullptr);
				return _value[n];
			}

			bool operator<(const wrap_pointer_iterator& rval) const
			{
				return _value < rval._value;
			}

			bool operator>(const wrap_pointer_iterator& rval) const
			{
				return _value > rval._value;
			}

			bool operator<=(const wrap_pointer_iterator& rval) const
			{
				return _value <= rval._value;
			}

			bool operator>=(const wrap_pointer_iterator& rval) const
			{
				return _value >= rval._value;
			}

		private:
			T _value = T();
		};

		template <typename T>
		auto operator+(std::ptrdiff_t lval, const wrap_pointer_iterator<T>& rval)
		{
			return rval + lval;
		}

	}
}



#endif //!HEAPLESS_ITERATOR_HPP