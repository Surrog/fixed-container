#ifndef BASIC_HEAPLESS_VECTOR_HPP
#define BASIC_HEAPLESS_VECTOR_HPP

#include <algorithm>
#include <iterator>
#include <vector>

#include "fixed/impl/iterator.hpp"
#include "fixed/impl/basic_allocation_pattern.hpp"

namespace fixed
{
	namespace _impl
	{
		template <typename T, container_size_type SIZE,
			template <typename, container_size_type> typename Alloc_pattern = basic_stack_allocator
		>
		class basic_vector
		{
		public:
			typedef T value_type;
			typedef T* pointer;
			typedef const T* const_pointer;
			typedef T& reference;
			typedef const T& const_reference;
			typedef container_size_type size_type;
			typedef std::ptrdiff_t difference_type;

			typedef pointer_iterator<T> iterator;
			typedef const_pointer_iterator<T> const_iterator;
			typedef std::reverse_iterator<iterator> reverse_iterator;
			typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

			template <typename Types, size_type ALLOC_SIZE>
			using allocator_type = Alloc_pattern<Types, ALLOC_SIZE>;

			//Basic function
			~basic_vector()
			{
				clear();
			}

			basic_vector() : _size(0), _data_container() {}

			template <typename Alloc_source = empty_source,
				std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0
			>
			explicit basic_vector(Alloc_source& alloc)
				: _size(0), _data_container(alloc)
			{}

			basic_vector(size_type count, const T& value)
				: basic_vector()
			{
				uninitialized_assign(count, value);
			}

			template <typename Alloc_source = empty_source,
				std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0
			>
			basic_vector(size_type count,
					const T& value,
					Alloc_source& alloc)
				: basic_vector(alloc)
			{
				uninitialized_assign(count, value);
			}

			explicit basic_vector(size_type count)
				: basic_vector()
			{
				uninitialized_assign(count);
			}

			template <typename Alloc_source = empty_source,
				std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0
			>
			explicit basic_vector(size_type count, Alloc_source& alloc)
				: basic_vector(alloc)
			{
				uninitialized_assign(count);
			}

			template< class InputIt,
				std::enable_if_t<is_iterator<InputIt>::value, int> = 0
			>
			basic_vector(InputIt first, InputIt last)
				: basic_vector()
			{
				uninitialized_assign(first, last);
			}

			template< class InputIt, typename Alloc_source = empty_source,
				std::enable_if_t<is_iterator<InputIt>::value, int> = 0,
				std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0
			>
			basic_vector(InputIt first, InputIt last, Alloc_source& alloc)
				: basic_vector(alloc)
			{
				uninitialized_assign(first, last);
			}

			basic_vector(const basic_vector& orig)
			{
				uninitialized_assign(orig.begin(), orig.end());
			}

			basic_vector(basic_vector&& orig)
			{
				operator=(orig);
			}

			template <size_type RSIZE, template <typename, size_type> typename RAllocator>
			basic_vector(const basic_vector<T, RSIZE, RAllocator>& orig)
				: basic_vector()
			{
				uninitialized_assign(orig.begin(), orig.end());
			}

			template <size_type RSIZE, template <typename, size_type> typename RAllocator,
				class Alloc_source = empty_source,
				std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0
			>
			basic_vector(const basic_vector<T, RSIZE, RAllocator>& orig, Alloc_source& alloc)
				: basic_vector(alloc)
			{
				uninitialized_assign(orig.begin(), orig.end());
			}

			template <size_type RSIZE>
			basic_vector(basic_vector<T, RSIZE, Alloc_pattern>&& other)
				: _size(other._size), _data_container(std::move(other._data_container))
			{}

			basic_vector(std::initializer_list<T> list)
				: basic_vector()
			{
				uninitialized_assign(list.begin(), list.end());
			}

			template <typename Alloc_source = empty_source,
				std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0
			>
			basic_vector(std::initializer_list<T> list, Alloc_source& alloc = empty_source())
				: basic_vector(alloc)
			{
				uninitialized_assign(list.begin(), list.end());
			}

			basic_vector& operator=(const basic_vector& rval)
			{
				if (this != &rval)
				{
					return operator=<SIZE, Alloc_pattern>(rval);
				}
				return *this;
			}

			basic_vector& operator=(basic_vector&& rval) noexcept
			{
				if (this != &rval)
				{
					return operator=<SIZE, Alloc_pattern>(rval);
				}
				return *this;
			}

			template <size_type RSIZE, template <typename, size_type> typename RAllocator>
			basic_vector& operator=(basic_vector<T, RSIZE, RAllocator>&& rval) noexcept
			{
				auto rbeg = rval.begin();
				auto rend = rval.end();
				auto lbeg = begin();
				auto lend = end();

				while (rbeg != rend && lbeg != lend)
				{
					*lbeg = std::move(*rbeg);
					++lbeg;
					++rbeg;
				}

				while (rbeg != rend)
				{
					push_back(std::move(*rbeg));
					++rbeg;
				}

				while (lbeg != lend)
				{
					(*lbeg).~T();
					++lbeg;
					--_size;
				}

				rval.clear();
				return *this;

			}

			template <size_type RSIZE, template <typename, size_type> typename RAllocator>
			basic_vector& operator=(const basic_vector<T, RSIZE, RAllocator>& rval)
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
					(*lbeg).~T();
					++lbeg;
					--_size;
				}
				return *this;
			}

			//element access
			T& at(size_type n) { FIXED_CHECK(n < _size); return data()[n]; }
			const T& at(size_type n) const { FIXED_CHECK(n < _size); return data()[n]; }
			T& operator[](size_type n) { return data()[n]; }
			const T& operator[](size_type n) const { return data()[n]; }
			T& front() { return at(0); }
			const T& front() const { return at(0); }
			T& back() { FIXED_CHECK(_size > 0); return at(_size - 1); }
			const T& back() const { FIXED_CHECK(_size > 0); return at(_size - 1); }
			T* data() { return _data_container.data(); }
			const T* data() const { return _data_container.data(); }

			//iterators
			iterator begin() { return { data() }; }
			const_iterator begin() const { return { data() }; }
			const_iterator cbegin() const { return { data() }; }
			iterator end() { return { data() + _size }; }
			const_iterator end() const { return const_iterator(data() + _size); }
			const_iterator cend() const { return const_iterator(data() + _size); }

			reverse_iterator rbegin() { return reverse_iterator(end()); }
			const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
			const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }
			reverse_iterator rend() { return reverse_iterator(begin()); }
			const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
			const_reverse_iterator crend() const { return const_reverse_iterator(begin()); }

			//capacity
			bool empty() const { return _size == 0; }
			size_type size() const { return _size; }
			constexpr size_type max_size() const { return _data_container.max_size(); }
			constexpr void reserve(size_type) const {}
			constexpr size_type capacity() const { return max_size(); }
			constexpr void shrink_to_fit() const {}

			void resize(size_type n)
			{
				resize(n, T());
			}

			void resize(size_type n, const T& val)
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
				FIXED_CHECK(count <= max_size());
				_size = count;
				for (auto& val : *this)
				{
					new (&val)T(value);
				}
			}

			template< class InputIt,
				std::enable_if_t<is_iterator<InputIt>::value, int> = 0
			>
				void assign(InputIt first, InputIt last)
			{
				size = std::distance(first, last);
				FIXED_CHECK(size <= max_size());
				_size = size;
				std::transform(first, last, begin(), [](const auto& value) { return value; });
			}

			void assign(std::initializer_list<T> list)
			{
				FIXED_CHECK(list.size() <= max_size());
				_size = list.size();
				std::transform(list.begin(), list.end(), begin(), [](const auto& value) { return value; });
			}

			void push_back(const T& value)
			{
				FIXED_CHECK(_size < max_size());
				new(&*end())T(value);
				_size++;
			}

			void push_back(T&& value)
			{
				FIXED_CHECK(_size < max_size());
				new(&*end())T(std::move(value));
				_size++;
			}

			void pop_back()
			{
				FIXED_CHECK(_size > 0);
				_size--;
				(*end()).~T();
			}

			iterator insert(const_iterator pos, const T& value)
			{
				return insert(pos, 1, value);
			}

			iterator insert(const_iterator pos, T&& value)
			{
				size_type pivot_index = _size;
				size_type insert_index = std::distance(cbegin(), pos);
				push_back(value);
				if (pos != cend() && pos != const_iterator())
				{
					std::rotate(begin() + insert_index, begin() + pivot_index, end());
				}
				return begin() + insert_index;
			}

			iterator insert(const_iterator pos, size_type count, const T& value)
			{
				auto pivot_index = _size;
				auto insert_index = std::distance(cbegin(), pos);
				for (size_type i = 0; i < count; i++)
				{
					push_back(value);
				}
				if (pos != cend() && pos != const_iterator())
				{
					std::rotate(begin() + insert_index, begin() + pivot_index, end());
				}
				return begin() + insert_index;
			}

			template< class InputIt,
				std::enable_if_t<is_iterator<InputIt>::value, int> = 0
			>
				iterator insert(const_iterator pos, InputIt first, InputIt last)
			{
				auto pivot_index = _size;
				auto insert_index = std::distance(cbegin(), pos);
				while (first != last)
				{
					push_back(*first);
					first++;
				}
				if (pos != cend() && pos != const_iterator())
				{
					std::rotate(begin() + insert_index, begin() + pivot_index, end());
				}
				return begin() + insert_index;
			}

			iterator insert(const_iterator pos, std::initializer_list<T> ilist)
			{
				return insert(pos, ilist.begin, ilist.end);
			}

			iterator erase(const_iterator position)
			{
				FIXED_CHECK(_size > 0);
				container_size_type index = std::distance(cbegin(), position);
				FIXED_CHECK(index < _size);
				if (index != _size - 1)
				{
					std::rotate(begin() + index, begin() + index + 1, end());
				}
				pop_back();
				return begin() + std::min(index, _size);
			}

			iterator erase(const_iterator first, const_iterator last)
			{
				FIXED_CHECK(_size > 0);
				container_size_type beg_i = std::distance(cbegin(), first);
				container_size_type end_i = std::distance(cbegin(), last);
				FIXED_CHECK(beg_i < _size);
				FIXED_CHECK(end_i <= _size);
				FIXED_CHECK(beg_i < end_i);
				if (end_i != _size)
				{
					std::rotate(begin() + beg_i, begin() + end_i, end());
				}
				for (container_size_type i = 0; i < (end_i - beg_i); i++)
				{
					pop_back();
				}
				return begin() + std::min(beg_i, _size);
			}

			template <size_type RSIZE>
			void swap(basic_vector<T, RSIZE>& rval)
			{
				FIXED_CHECK(rval.size() < SIZE);
				FIXED_CHECK(size() < RSIZE);

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
				emplace_back(args...);
				std::rotate(position, end() - 1, end());
			}

			template < class... Args>
			void emplace_back(Args&&... args)
			{
				FIXED_CHECK(_size < max_size());
				new (&*end())T(args...);
				_size++;
			}

			template <class... Args>
			iterator emplace_back(Args&&... args)
			{
				FIXED_CHECK(_size < max_size());
				new (&*end())T(args...);
				auto result = end();
				_size++;
				return result;
			}


		private:
			//datas
			size_type _size = 0;
			Alloc_pattern<T, SIZE> _data_container;

			//managing uninitialized memory
			template< class InputIt,
				std::enable_if_t<is_iterator<InputIt>::value, int> = 0
			>
			void uninitialized_assign(InputIt begin, InputIt end)
			{
				while (begin != end)
				{
					push_back(*begin);
					++begin;
				}
			}

			void uninitialized_assign(size_type count, const T& value)
			{
				FIXED_CHECK(count <= max_size());

				for (size_type i = 0; i < count; i++)
				{
					new (_data_container.data() + i)T(value);
					_size++;
				}
			}

			void uninitialized_assign(size_type count)
			{
				FIXED_CHECK(count <= max_size());

				for (size_type i = 0; i < count; i++)
				{
					new (_data_container.data() + i)T();
					_size++;
				}
			}
		};

		template <typename T, container_size_type LSIZE,
			template <typename, container_size_type> typename LALLOCATOR,
			typename STDALLOC>
		bool operator==(const basic_vector<T, LSIZE, LALLOCATOR>& lval, const std::vector<T, STDALLOC>& rval)
		{
			return lval.size() == rval.size() && std::equal(lval.begin(), lval.end(), rval.begin(), rval.end());
		}
	}
}

#endif //!BASIC_HEAPLESS_VECTOR_HPP