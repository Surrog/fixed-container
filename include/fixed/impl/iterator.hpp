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
		struct is_iterator : public std::false_type
		{};

		template <typename T>
		struct is_iterator<T, fixed::astd::void_t<typename std::iterator_traits<T>::iterator_category>> : public std::true_type
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
		struct const_pointer_iterator
		{
			typedef typename std::remove_pointer<T>::type rp_value_type;

		public:
			typedef typename rp_value_type value_type;
			typedef const rp_value_type* pointer;
			typedef const rp_value_type& reference;
			typedef std::ptrdiff_t difference_type;
			typedef std::random_access_iterator_tag iterator_category;

			bool operator==(const const_pointer_iterator& rval) const
			{
				return _value == rval._value;
			}

			bool operator!=(const const_pointer_iterator& rval) const
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

			const_pointer_iterator() = default;

			const_pointer_iterator(const T* val)
				: _value(const_cast<value_type*>(val))
			{}

			const_pointer_iterator(const const_pointer_iterator&) = default;
			const_pointer_iterator(const_pointer_iterator&&) noexcept = default;
			const_pointer_iterator& operator=(const const_pointer_iterator&) = default;
			const_pointer_iterator& operator=(const_pointer_iterator&&) noexcept = default;

			const_pointer_iterator(const pointer_iterator<value_type>& orig)
				: _value(&orig.get())
			{}

			const_pointer_iterator& operator++()
			{
				assert(_value != nullptr);
				++_value;
				return *this;
			}

			const_pointer_iterator operator++(int) const
			{
				assert(_value != nullptr);
				return { _value + 1 };
			}


			const_pointer_iterator& operator--()
			{
				assert(_value != nullptr);
				_value--;
				return *this;
			}

			const_pointer_iterator operator--(int) const
			{
				assert(_value != nullptr);
				return { _value - 1 };
			}

			const_pointer_iterator& operator+=(difference_type n)
			{
				assert(_value != nullptr);
				_value += n;
				return *this;
			}

			const_pointer_iterator operator+(difference_type n) const
			{
				assert(_value != nullptr);
				return { _value + n };
			}

			const_pointer_iterator& operator-=(difference_type n)
			{
				assert(_value != nullptr);
				_value -= n;
				return *this;
			}

			const_pointer_iterator operator-(difference_type n) const
			{
				assert(_value != nullptr);
				return { _value - n };
			}

			difference_type operator-(const const_pointer_iterator& rval) const
			{
				return _value - rval._value;
			}

			reference operator[](difference_type n)
			{
				assert(_value != nullptr);
				return _value[n];
			}

			bool operator<(const const_pointer_iterator& rval) const
			{
				return _value < rval._value;
			}

			bool operator>(const const_pointer_iterator& rval) const
			{
				return _value > rval._value;
			}

			bool operator<=(const const_pointer_iterator& rval) const
			{
				return _value <= rval._value;
			}

			bool operator>=(const const_pointer_iterator& rval) const
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
		auto operator+(std::ptrdiff_t lval, const const_pointer_iterator<T>& rval)
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
		struct const_wrap_pointer_iterator
		{
			typedef typename T::rp_value_type rp_value_type;

		public:
			typedef typename T::value_type value_type;
			typedef typename T::pointer pointer;
			typedef typename T::reference reference;
			typedef typename T::difference_type difference_type;
			typedef typename T::iterator_category iterator_category;

			bool operator==(const const_wrap_pointer_iterator& rval) const
			{
				return _value == rval._value;
			}

			bool operator!=(const const_wrap_pointer_iterator& rval) const
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

			const_wrap_pointer_iterator() = default;

			const_wrap_pointer_iterator(const T& val)
				: _value(val)
			{}

			const_wrap_pointer_iterator(const const_wrap_pointer_iterator&) = default;
			const_wrap_pointer_iterator(const_wrap_pointer_iterator&&) noexcept = default;
			const_wrap_pointer_iterator& operator=(const const_wrap_pointer_iterator&) = default;
			const_wrap_pointer_iterator& operator=(const_wrap_pointer_iterator&&) noexcept = default;

			const_wrap_pointer_iterator(const wrap_pointer_iterator<value_type>& orig)
				: _value(&orig.get())
			{}

			const_wrap_pointer_iterator& operator++()
			{
				assert(_value != nullptr);
				++_value;
				return *this;
			}

			const_wrap_pointer_iterator operator++(int) const
			{
				assert(_value != nullptr);
				return { _value + 1 };
			}


			const_wrap_pointer_iterator& operator--()
			{
				assert(_value != nullptr);
				_value--;
				return *this;
			}

			const_wrap_pointer_iterator operator--(int) const
			{
				assert(_value != nullptr);
				return { _value - 1 };
			}

			const_wrap_pointer_iterator& operator+=(difference_type n)
			{
				assert(_value != nullptr);
				_value += n;
				return *this;
			}

			const_wrap_pointer_iterator operator+(difference_type n) const
			{
				assert(_value != nullptr);
				return { _value + n };
			}

			const_wrap_pointer_iterator& operator-=(difference_type n)
			{
				assert(_value != nullptr);
				_value -= n;
				return *this;
			}

			const_wrap_pointer_iterator operator-(difference_type n) const
			{
				assert(_value != nullptr);
				return { _value - n };
			}

			difference_type operator-(const const_wrap_pointer_iterator& rval) const
			{
				return _value - rval._value;
			}

			reference operator[](difference_type n)
			{
				assert(_value != nullptr);
				return _value[n];
			}

			bool operator<(const const_wrap_pointer_iterator& rval) const
			{
				return _value < rval._value;
			}

			bool operator>(const const_wrap_pointer_iterator& rval) const
			{
				return _value > rval._value;
			}

			bool operator<=(const const_wrap_pointer_iterator& rval) const
			{
				return _value <= rval._value;
			}

			bool operator>=(const const_wrap_pointer_iterator& rval) const
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


		template <typename T>
		auto operator+(std::ptrdiff_t lval, const const_wrap_pointer_iterator<T>& rval)
		{
			return rval + lval;
		}

	}
}



#endif //!HEAPLESS_ITERATOR_HPP