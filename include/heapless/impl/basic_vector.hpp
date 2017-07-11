#ifndef BASIC_HEAPLESS_VECTOR_HPP
#define BASIC_HEAPLESS_VECTOR_HPP

#include <algorithm>
#include <cassert>

#include "impl/iterator.hpp"

namespace heapless
{
	namespace _impl 
	{
		template <typename T, std::size_t SIZE>
		class basic_vector
		{
		public:
			static_assert(SIZE > 0, "vector needs to have a size greater than 0");

			typedef T value_type;
			typedef T* pointer;
			typedef const T* const_pointer;
			typedef T& reference;
			typedef const T& const_reference;
			typedef std::size_t size_type;
			typedef std::ptrdiff_t difference_type;

			typedef random_access_iterator<T*> iterator;
			typedef random_access_iterator<const T*> const_iterator;
			//typedef T* iterator;
			//typedef const T* const_iterator;
			typedef std::reverse_iterator<iterator> reverse_iterator;
			typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

			struct allocator_type {};

			//Basic function
			basic_vector() noexcept = default;
			basic_vector(const basic_vector& orig) noexcept
			{
				operator=(orig);
			}
			basic_vector& operator=(const basic_vector& orig) noexcept
			{
				if (this != &orig)
				{
					_size = orig._size;
					std::copy(orig.begin(), orig.end(), begin());
				}
				return *this;
			}

			explicit basic_vector(size_type count, const T& value)
			{
				assign(count, value);
			}

			explicit basic_vector(size_type count)
				: _size(count)
			{
				assert(_size <= SIZE);
				for (auto& val : *this)
				{
					new (&val)T();
				}
			}

			template <typename InputIt>
			basic_vector(InputIt first, InputIt last)
			{
				assign(first, last);
			}

			basic_vector(std::initializer_list<T> list)
			{
				assign(list);
			}

			template <std::size_t SIZE2>
			basic_vector(const basic_vector<T, SIZE2>& orig)
			{
				assign(orig.begin(), orig.end());
			}

			void assign(size_type count, const T& value)
			{
				_size = count;
				assert(_size <= SIZE);
				for (auto& val : *this)
				{
					new (&val)T(value);
				}
			}

			template <typename InputIt>
			void assign(InputIt first, InputIt last)
			{
				_size = std::distance(first, last);
				assert(_size <= SIZE);
				std::transform(first, last, begin(), [](const auto& value) { return value; })
			}

			void assign(std::initializer_list<T> list)
			{
				assert(list.size() <= SIZE);
				_size = list.size();
				std::transform(list.begin(), list.end(), begin(), [](const auto& value) { return value; });
			}

			//element access
			T& at(size_type n) { assert(n < _size); return data()[n]; }
			const T& at(size_type n) const { assert(n < _size); return data()[n]; }
			T& operator[](size_type n) { return data()[i]; }
			const T& operator[](size_type n) const { return data()[i]; }
			T& front() { return at(0); }
			const T& front() const { return at(0); }
			T& back() { assert(_size > 0); return at(_size - 1); }
			const T& back() const { assert(_size > 0); return at(_size - 1); }
			T* data() { return reinterpret_cast<T*>(_data); }
			const T* data() const { return reinterpret_cast<T*>(_data); }

			//iterators
			iterator begin() { return data(); }
			const_iterator begin() const { return data(); }
			const_iterator cbegin() const { return data(); }
			iterator end() { return data() + _size; }
			const_iterator end() const { return data() + _size; }
			const_iterator cend() const { return data() + _size; }

			reverse_iterator rbegin() { return reverse_iterator(data()); }
			const_reverse_iterator rbegin() const { return const_reverse_iterator(data()); }
			const_reverse_iterator crbegin() const { return const_reverse_iterator(data()); }
			reverse_iterator rend() { return reverse_iterator(data() + _size); }
			const_reverse_iterator rend() const { return const_reverse_iterator(data() + _size); }
			const_reverse_iterator crend() const { return const_reverse_iterator(data() + _size); }

			//capacity
			bool empty() const { return _size == 0; }
			size_type size() const { return _size; }
			constexpr size_type max_size() const { return SIZE; }
			constexpr void reserve(size_type) const {}
			constexpr size_type capacity() const { return SIZE; }
			constexpr void shrink_to_fit() const {}

			//Modifiers
			void clear() noexcept {
				std::for_each(begin(), end(), [](auto& val) { val.~T(); });
				_size = 0;
			}

			void resize(std::size_t n)
			{
				_size -= 1;
				while (_size >= n)
				{
					data()[_size].~T();
					_size--;
				}
				while (_size < n)
				{
					new (&data()[_size])();
					_size++;
				}
			}

			void resize(std::size_t n, const T& val) {
				_size -= 1;
				while (_size >= n)
				{
					data()[_size].~T();
					_size--;
				}
				while (_size < n)
				{
					new (&data()[_size])(val);
					_size++;
				}
			}

			void push_back(const T& value)
			{
				assert(_size < SIZE);
				new(end())T(value);
				_size++;
			}

			void push_back(T&& value)
			{
				assert(_size < SIZE);
				new(end())T(value);
				_size++;
			}

			template < class... Args>
			void emplace_back(Args&&... args)
			{
				assert(_size < SIZE);
				new (end())T(args...);
				_size++;
			}

			template <class... Args>
			iterator emplace_back(Args&&... args)
			{
				assert(_size < SIZE);
				new (end())T(args...);
				auto result = end();
				_size++;
				return result;
			}

		private:
			size_type _size = 0;
			typename std::aligned_storage<sizeof(T), alignof(T)>::type _data[SIZE];
		};
	}

}

#endif //!BASIC_HEAPLESS_VECTOR_HPP