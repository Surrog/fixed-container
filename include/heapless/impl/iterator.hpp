#ifndef HEAPLESS_ITERATOR_HPP
#define HEAPLESS_ITERATOR_HPP

#include <cstddef>
#include <type_traits>
#include <iterator>

namespace heapless
{
	namespace _impl 
	{
		template <typename T, typename iterator_type>
		struct basic_input_iterator
		{
			typedef typename std::remove_pointer<T>::type value_type;
			typedef typename value_type* pointer;
			typedef typename value_type& reference;
			typedef std::ptrdiff_t difference_type;

			bool operator==(const basic_input_iterator& rval) const
			{
				return _value == rval._value;
			}

			bool operator!=(const basic_input_iterator& rval) const
			{
				return !operator==(rval);
			}

			reference get() const
			{
				return _value;
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

			basic_input_iterator& operator++()
			{
				assert(_value != T());
				_value++;
				return *this;
			}

		protected:
			typedef typename std::remove_const<T>::type inner_storage;
			inner_storage _value = T();
		};

		template <typename IT>
		IT& operator++(IT& iterator)
		{
			iterator++;
			return *this;
		}

		template <typename T, typename iterator_type>
		struct basic_forward_iterator : public basic_input_iterator<T, iterator_type>
		{
			iterator_type operator++(int) const
			{
				assert(_value != nullptr);
				return { _value++ };
			}
		};

		template <typename T, typename iterator_type>
		struct basic_bidirectional_iterator : basic_forward_iterator<T, iterator_type>
		{
			iterator_type& operator--()
			{
				assert(_value != nullptr);
				_value++;
				return *this;
			}

			iterator_type operator--(int) const
			{
				assert(_value != nullptr);
				_value++;
				return *this;
			}
		};

		template <typename T, typename iterator_type>
		struct basic_random_access_iterator : public basic_bidirectional_iterator<T, iterator_type>
		{
			iterator_type& operator+=(difference_type n)
			{
				assert(_value != T());
				_value += n;
				return *this;
			}

			iterator_type operator+(difference_type n) const
			{
				assert(_value != T());
				return { _value + n };
			}

			iterator_type& operator-=(difference_type n)
			{
				assert(_value != T());
				_value -= n;
				return *this;
			}

			iterator_type operator-(difference_type n) const
			{
				assert(_value != T());
				return { _value - n };
			}

			difference_type operator-(const iterator_type& rval) const
			{
				return _value - rval._value;
			}

			reference operator[](difference_type n)
			{
				assert(_value != T());
				return _value[n];
			}

			bool operator<(const iterator_type& rval) const
			{
				return _value < rval._value;
			}

			bool operator>(const iterator_type& rval) const
			{
				return _value > rval._value;
			}

			bool operator<=(const iterator_type& rval) const
			{
				return _value <= rval._value;
			}

			bool operator>=(const iterator_type& rval) const
			{
				return _value >= rval._value;
			}
		};

		template <typename T, typename iterator_type>
		auto operator+(std::ptrdiff_t lval, const basic_random_access_iterator<T, iterator_type>& rval)
		{
			return rval + lval;
		}

		template <typename T>
		struct input_iterator : public basic_input_iterator<T, input_iterator<T>>
		{
			typedef std::input_iterator_tag iterator_category;
		};

		template <typename T>
		struct forward_iterator : public basic_forward_iterator<T, forward_iterator<T>>
		{
			typedef std::forward_iterator_tag iterator_category;
		};

		template <typename T>
		struct bidirection_iterator : public basic_bidirectional_iterator<T, bidirection_iterator<T>>
		{
			typedef std::bidirectional_iterator_tag iterator_category;
		};

		template <typename T>
		struct random_access_iterator : public basic_random_access_iterator<T, random_access_iterator<T>>
		{
			typedef std::random_access_iterator_tag iterator_category;
		};
	}
}



#endif //!HEAPLESS_ITERATOR_HPP