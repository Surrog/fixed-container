#ifndef HEAPLESS_ITERATOR_HPP
#define HEAPLESS_ITERATOR_HPP

#include <cstddef>
#include <type_traits>
#include <iterator>

namespace fixed
{
	namespace _impl
	{
		template <typename T, class = void>
		struct is_iterator : public std::bool_constant<false>
		{};

		template <typename T>
		struct is_iterator<T, std::void_t<typename std::iterator_traits<T>::iterator_category>> : public std::bool_constant<true>
		{};


		template <typename T>
		struct basic_iterator
		{
		protected:
			typedef typename std::remove_const<T>::type inner_storage;
			inner_storage* _value = nullptr;

		public:
			typedef typename T value_type;
			typedef typename value_type* pointer;
			typedef typename value_type& reference;
			typedef std::ptrdiff_t difference_type;

			basic_iterator() = default;
			basic_iterator(T* val) : _value(val) {}
			basic_iterator(const basic_iterator&) = default;
			basic_iterator(basic_iterator&&) noexcept = default;
			basic_iterator& operator=(const basic_iterator&) = default;
			basic_iterator& operator=(basic_iterator&&) noexcept = default;

			bool operator==(const basic_iterator& rval) const
			{
				return _value == rval._value;
			}

			bool operator!=(const basic_iterator& rval) const
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
		};

		template <typename T>
		struct basic_io_iterator : basic_iterator<T>
		{
			typedef std::input_iterator_tag iterator_category;

			basic_io_iterator() = default;
			basic_io_iterator(T val) : basic_iterator<T>(val) {}
			basic_io_iterator(const basic_io_iterator&) = default;
			basic_io_iterator(basic_io_iterator&&) noexcept = default;
			basic_io_iterator& operator=(const basic_io_iterator&) = default;
			basic_io_iterator& operator=(basic_io_iterator&&) noexcept = default;

			template <typename = std::enable_if<std::is_const<T>::value>>
			basic_io_iterator(const basic_io_iterator<inner_storage>& orig)
			{
				_value = orig.value;
			}

			basic_io_iterator& operator++()
			{
				assert(_value != T());
				_value++;
				return *this;
			}
		};

		template <typename T>
		struct basic_forward_iterator : public basic_iterator<T>
		{
			typedef std::forward_iterator_tag iterator_category;

			basic_forward_iterator() = default;
			basic_forward_iterator(T val) : basic_iterator<T>(val) {}
			basic_forward_iterator(const basic_forward_iterator&) = default;
			basic_forward_iterator& operator=(const basic_forward_iterator&) = default;
			basic_forward_iterator(basic_forward_iterator&&) noexcept = default;
			basic_forward_iterator& operator=(basic_forward_iterator&&) noexcept = default;

			template <typename = std::enable_if<std::is_const<T>::value>>
			basic_forward_iterator(const basic_forward_iterator<inner_storage>& orig)
			{
				_value = orig.value;
			}

			basic_forward_iterator& operator++()
			{
				assert(_value != T());
				_value++;
				return *this;
			}

			basic_forward_iterator operator++(int) const
			{
				assert(_value != nullptr);
				return { _value++ };
			}
		};

		template <typename T>
		struct basic_bidirectional_iterator : basic_iterator<T>
		{
			typedef std::bidirectional_iterator_tag iterator_category;

			basic_bidirectional_iterator() = default;
			basic_bidirectional_iterator(T val) : basic_iterator<T>(val) {}
			basic_bidirectional_iterator(const basic_bidirectional_iterator&) = default;
			basic_bidirectional_iterator(basic_bidirectional_iterator&&) = default;
			basic_bidirectional_iterator& operator=(basic_bidirectional_iterator&&) = default;
			basic_bidirectional_iterator& operator=(const basic_bidirectional_iterator&) = default;

			template <typename = std::enable_if<std::is_const<T>::value>>
			basic_bidirectional_iterator(const basic_bidirectional_iterator<inner_storage>& orig)
			{
				_value = orig._value;
			}

			basic_bidirectional_iterator& operator++()
			{
				assert(_value != T());
				_value++;
				return *this;
			}

			basic_bidirectional_iterator operator++(int) const
			{
				assert(_value != nullptr);
				return { _value++ };
			}


			basic_bidirectional_iterator& operator--()
			{
				assert(_value != nullptr);
				_value++;
				return *this;
			}

			basic_bidirectional_iterator operator--(int) const
			{
				assert(_value != nullptr);
				_value++;
				return *this;
			}
		};

		template <typename T>
		struct pointer_iterator
		{
			typedef typename std::remove_pointer<T>::type rp_value_type;

		public:
			typedef typename std::remove_const<rp_value_type>::type value_type;
			typedef typename rp_value_type* pointer;
			typedef typename rp_value_type& reference;
			typedef std::ptrdiff_t difference_type;
			typedef std::random_access_iterator_tag iterator_category;
			constexpr static bool is_const = std::is_const_v<rp_value_type>;

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
	}
}



#endif //!HEAPLESS_ITERATOR_HPP