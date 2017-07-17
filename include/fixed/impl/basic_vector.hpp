#ifndef BASIC_HEAPLESS_VECTOR_HPP
#define BASIC_HEAPLESS_VECTOR_HPP

#include <algorithm>
#include <iterator>
#include <cassert>

#include "fixed/impl/iterator.hpp"
#include "fixed/impl/basic_allocator.hpp"

namespace fixed
{
	namespace _impl
	{
		template <typename T, std::size_t SIZE, typename Allocator = basic_stack_allocator<T, SIZE> >
		class basic_vector
		{
		public:
			typedef T value_type;
			typedef T* pointer;
			typedef const T* const_pointer;
			typedef T& reference;
			typedef const T& const_reference;
			typedef std::size_t size_type;
			typedef std::ptrdiff_t difference_type;

			typedef pointer_iterator<T> iterator;
			typedef const_pointer_iterator<T> const_iterator;
			typedef std::reverse_iterator<iterator> reverse_iterator;
			typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

			typedef Allocator allocator_type;

			//Basic function
			~basic_vector()
			{
				for (auto& elem : *this)
				{
					elem.~T();
				}
			}

			explicit basic_vector(const Allocator& alloc = Allocator())
				: _size(0), _data_container(alloc)
			{}

			template <std::size_t RSIZE, typename RAllocator>
			basic_vector(const basic_vector<T, RSIZE, RAllocator>& orig, const Allocator& alloc = Allocator())
				: _size(0), _data_container(alloc)
			{
				uninitialized_assign(orig.begin(), orig.end());
			}

			template <std::size_t RSIZE, typename RAllocator>
			basic_vector& operator=(const basic_vector<T, RSIZE, RAllocator>& rval)
			{
				if (this != &orig)
				{ 
					auto rbeg = rval.begin();
					auto rend = rval.end();
					auto lbeg = begin();
					auto lend = end();

					while (rbeg != rend && lbeg != lend)
					{
						*lbeg = *rbeg;
						++lbeg;
						++rbeg;
					}

					while (rbeg != rend)
					{
						push_back(*rbeg);
						++rbeg;
					}

					while (lbeg != lend)
					{
						lbeg->~T();
						++lbeg;
						--size;
					}
				}
				return *this;
			}

			explicit basic_vector(size_type count, const T& value, const Allocator& alloc = Allocator())
				: _size(0), _data_container(alloc)
			{
				uninitialized_assign(count, value);
			}

			explicit basic_vector(size_type count, const Allocator& alloc = Allocator())
				: _size(count), _data_container(alloc)
			{
				uninitialized_assign(count);
			}

			template <typename InputIt>
			basic_vector(InputIt first, InputIt last, const Allocator& alloc = Allocator())
				: _size(0), _data_container(alloc)
			{
				uninitialized_assign(first, last);
			}

			basic_vector(std::initializer_list<T> list, const Allocator& alloc = Allocator())
				: _size(0), _data_container(alloc)
			{
				uninitialized_assign(list.begin(), list.end());
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
			T* data() { return _data_container.data(); }
			const T* data() const { return _data_container.data(); }

			//iterators
			iterator begin() { return { data() }; }
			const_iterator begin() const { return { data() }; }
			const_iterator cbegin() const { return { data() }; }
			iterator end() { return { data() + _size }; }
			const_iterator end() const { return const_iterator(data() + _size); }
			const_iterator cend() const { return const_iterator(data() + _size); }

			reverse_iterator rbegin() { return reverse_iterator(data()); }
			const_reverse_iterator rbegin() const { return const_reverse_iterator(data()); }
			const_reverse_iterator crbegin() const { return const_reverse_iterator(data()); }
			reverse_iterator rend() { return reverse_iterator(data() + _size); }
			const_reverse_iterator rend() const { return const_reverse_iterator(data() + _size); }
			const_reverse_iterator crend() const { return const_reverse_iterator(data() + _size); }

			//capacity
			bool empty() const { return _size == 0; }
			size_type size() const { return _size; }
			constexpr size_type max_size() const { return _data_container.max_size(); }
			constexpr void reserve(size_type) const {}
			constexpr size_type capacity() const { return max_size(); }
			constexpr void shrink_to_fit() const {}

			void resize(std::size_t n)
			{
				while (_size > n)
				{
					pop_back();
				}
				while (_size < n)
				{
					push_back(T());
				}
			}

			void resize(std::size_t n, const T& val)
			{
				while (_size > n)
				{
					pop_back();
				}
				while (_size < n)
				{
					push_back(val);
				}
			}

			//Modifiers
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

			void push_back(const T& value)
			{
				assert(_size < SIZE);
				new(&*end())T(value);
				_size++;
			}

			void push_back(T&& value)
			{
				assert(_size < SIZE);
				new(&*end())T(value);
				_size++;
			}

			void pop_back()
			{
				assert(_size > 0);
				_size--;
				end()->~T();
			}

			iterator insert(const_iterator pos, const T& value)
			{
				return insert(pos, 1, value);
			}

			iterator insert(const_iterator pos, T&& value)
			{
				auto pivot_index = _size;
				auto insert_index = distance(cbegin(), pos);
				push_back(value);
				std::rotate(begin() + insert_index, begin() + pivot_index, end());
				return begin() + insert_index;
			}

			iterator insert(const_iterator pos, size_type count, const T& value)
			{
				auto pivot_index = _size;
				auto insert_index = distance(cbegin(), pos);
				for (size_type i = 0; i < count; i++)
				{
					push_back(value);
				}
				std::rotate(begin() + insert_index, begin() + pivot_index, end());
				return begin() + insert_index;
			}

			template <class InputIt>
			iterator insert(const_iterator pos, InputIt first, InputIt last)
			{
				auto pivot_index = _size;
				auto insert_index = distance(cbegin(), pos);
				while (first != last)
				{
					push_back(*first);
					first++;
				}
				std::rotate(begin() + insert_index, begin() + pivot_index, end());
				return begin() + insert_index;
			}

			iterator insert(const_iterator pos, std::initializer_list<T> ilist)
			{
				return insert(pos, ilist.begin, ilist.end);
			}

			iterator erase(const_iterator position)
			{
				auto initial_pos = std::distance(cbegin(), position);
				std::rotate(begin() + position, begin() + position + 1, end());
				pop_back();
				return begin() + position;
			}

			iterator erase(const_iterator first, const_iterator last)
			{
				auto elem_num = std::distance(first, last);
				auto initial_pos = std::distance(cbegin(), first);
				std::rotate(iterator(&*first), iterator(&*(last + 1)), end());
				for (auto i = 0; i < elem_num; i++)
				{
					pop_back();
				}
				return begin() + initial_pos;
			}

			template <std::size_t RSIZE>
			void swap(basic_vector<T, RSIZE>& rval)
			{
				assert(rval.size() < SIZE);
				assert(size() < RSIZE);

				auto lbeg = begin();
				auto lend = end();
				auto rbeg = rval.begin();
				auto rend = rval.end();

				while (lbeg != lend && rbeg != rend)
				{
					std::swap(*lbeg, rbeg);
					++lbeg;
					++rbeg;
				}

				while (lbeg != lend)
				{
					rval.push_back(std::move(*lbeg));
					++lbeg;
				}

				while (rbeg != rend)
				{
					push_back(std::move(*rbeg));
					++rbeg;
				}
			}

			void clear() noexcept {
				std::for_each(begin(), end(), [](auto& val) { val.~T(); });
				_size = 0;
			}

			template <class... Args>
			iterator emplace(iterator position, Args&&... args)
			{
				emplace_back(args);
				std::rotate(position, end() - 1, end());
			}

			template < class... Args>
			void emplace_back(Args&&... args)
			{
				assert(_size < SIZE);
				new (&*end())T(args...);
				_size++;
			}

			template <class... Args>
			iterator emplace_back(Args&&... args)
			{
				assert(_size < SIZE);
				new (&*end())T(args...);
				auto result = end();
				_size++;
				return result;
			}


		private:
			size_type _size = 0;
			Allocator _data_container;

			template <typename It>
			void uninitialized_assign(It begin, It end)
			{
				auto new_size = std::distance(begin, end);
				assert(new_size <= SIZE);
				_size = new_size;

				for (auto& uninitialized_item : *this)
				{
					new (&uninitialized_item)(*begin);
					++begin;
				}
			}

			void uninitialized_assign(std::size_t count, const T& value)
			{
				assert(count <= SIZE);
				_size = count;

				for (auto& uninitialized_item : *this)
				{
					new (&uninitialized_item)(value);
				}
			}

			void uninitialized_assign(std::size_t count)
			{
				assert(count <= SIZE);
				_size = count;

				for (auto& uninitialized_item : *this)
				{
					new (&uninitialized_item)();
				}
			}

		};
	}

}

#endif //!BASIC_HEAPLESS_VECTOR_HPP