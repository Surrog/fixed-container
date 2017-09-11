#ifndef FIXED_BASIC_LIST_HPP
#define FIXED_BASIC_LIST_HPP

#include <array>
#include <cassert>
#include "fixed/impl/basic_allocation_pattern.hpp"
#include "fixed/impl/fixed_def.hpp"
#include "iterator.hpp"
#include <utility>
#include <algorithm>

namespace fixed
{
	namespace _impl
	{
		template <typename T, container_size_type SIZE,
			template <typename, container_size_type> typename Alloc_pattern = basic_stack_allocator >
		class basic_list
		{
		public:
			typedef T value_type;
			typedef container_size_type size_type;
			typedef std::ptrdiff_t difference_type;
			typedef value_type& reference;
			typedef const value_type& const_reference;
			typedef value_type* pointer;
			typedef const value_type* const_pointer;
			typedef wrap_pointer_iterator<T, pointer_iterator<T*>> iterator;
			typedef const_wrap_pointer_iterator<T, const_pointer_iterator<T*>> const_iterator;
			typedef std::reverse_iterator<iterator> reverse_iterator;
			typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

			template <typename Types, size_type ALLOC_SIZE>
			using allocator_type = Alloc_pattern<Types, ALLOC_SIZE>;

		private:
			Alloc_pattern<T, SIZE> _data_holder;
			Alloc_pattern<T*, SIZE> _ptrs;
			size_type _size = 0;

			void push_back()
			{
				assert(_size + 1 < SIZE);
				new (_ptrs[_size]) T();
				++_size;
			}

			void set_at(size_type index, const T& value)
			{
				if (index < _size)
				{
					*_ptrs[index] = value;
				}
				else
				{
					push_back(value);
				}
			}

			void set_at(size_type index, T&& value)
			{
				if (index < _size)
				{
					*_ptrs[index] = value;
				}
				else
				{
					push_back(value);
				}
			}

		public:
			basic_list() 
				: _data_holder(), _ptrs(), _size(0)
			{
				for (size_type i = 0; i < SIZE; i++)
				{
					_ptrs[i] = _data_holder.data() + i;
				}
			}

			template <typename Alloc_source,
				std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0>
			explicit basic_list(Alloc_source& alloc)
				: _data_holder(alloc), _ptrs(), _size(0)
			{
				for (size_type i = 0; i < SIZE; i++)
				{
					_ptrs[i] = _data_holder.data() + i;
				}
			}

			explicit basic_list(size_type count)
				: basic_list()
			{
				for (size_type i = 0; i < count; i++)
				{
					push_back();
				}
			}

			template <typename Alloc_source,
				std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0>
				explicit basic_list(size_type count, Alloc_source& alloc)
				: basic_list(alloc)
			{
				for (size_type i = 0; i < count; i++)
				{
					push_back();
				}
			}

			basic_list(size_type count, const T& value)
				: basic_list()
			{
				for (size_type i = 0; i < count; i++)
				{
					push_back(value);
				}
			}

			template <typename Alloc_source,
				std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0>
			basic_list(size_type count, const T& value, Alloc_source& alloc)
				: basic_list(alloc)
			{
				for (size_type i = 0; i < count; i++)
				{
					push_back(value);
				}
			}

			template< class InputIt,
				std::enable_if_t<is_iterator<InputIt>::value, int> = 0
			>
				basic_list(InputIt first, InputIt last)
				: basic_list()
			{
				while (first != last)
				{
					push_back(*first);
					++first;
				}
			}


			template< class InputIt, class Alloc_source = empty_source,
				std::enable_if_t<is_iterator<InputIt>::value, int> = 0,
				std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0
			>
				basic_list(InputIt first, InputIt last, Alloc_source& alloc)
				: basic_list(alloc)
			{
				while (first != last)
				{
					push_back(*first);
					++first;
				}
			}

			basic_list(const basic_list& other)
				: basic_list(other.begin(), other.end())
			{}

			template <class Alloc_source = empty_source,
				std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0>
				basic_list(const basic_list& other, Alloc_source& alloc)
				: basic_list(other.begin(), other.end(), alloc)
			{}

			template <container_size_type RSIZE, template <typename, container_size_type> typename RALLOC>
			basic_list(const basic_list<T, RSIZE, RALLOC>& other)
				: basic_list(other.begin(), other.end())
			{}

			template <container_size_type RSIZE, template <typename, container_size_type> typename RALLOC,
				class Alloc_source = empty_source,
				std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0>
			basic_list(const basic_list<T, RSIZE, RALLOC>& other, Alloc_source& alloc)
				: basic_list(other.begin(), other.end(), alloc)
			{}

			basic_list(basic_list&& other)
				: basic_list()
			{
				for (auto& elem : other)
				{
					push_back(std::move(elem));
				}
				other.clear();
			}

			template < class Alloc_source = empty_source,
				std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0
			>
				basic_list(basic_list&& other, Alloc_source& alloc)
				: basic_list(alloc)
			{
				for (auto& elem : other)
				{
					push_back(std::move(elem));
				}
				other.clear();
			}

			template < container_size_type RSIZE, template <typename, container_size_type> typename RALLOC,
				class Alloc_source = empty_source,
				std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0
			>
				basic_list(basic_list<T, RSIZE, RALLOC>&& other, Alloc_source& alloc)
				: basic_list(alloc)
			{
				for (auto& elem : other)
				{
					push_back(std::move(elem));
				}
				other.clear();
			}

			template <container_size_type RSIZE, template <typename, container_size_type> typename RALLOC>
			basic_list(basic_list<T, RSIZE, RALLOC> && other)
				: basic_list(other, empty_source())
			{}


			template < class Alloc_source = empty_source,
				std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0>
			basic_list(std::initializer_list<T> init, const Alloc_source& alloc = Alloc_source())
				: basic_list(init.begin(), init.end(), alloc)
			{}

			~basic_list()
			{
				clear();
			}

			basic_list& operator=(const basic_list& other)
			{
				if (this != &other)
				{
					size_type i = 0;
					for (auto& val : other)
					{
						set_at(i, val);
						++i;
					}
				}
				return *this;
			}

			template <container_size_type RSIZE, template <typename, container_size_type> typename RALLOC>
			basic_list& operator=(const basic_list<T, RSIZE, RALLOC>& other)
			{
				if (this != &other)
				{
					size_type i = 0;
					for (auto& val : other)
					{
						set_at(i, val);
						++i;
					}
				}
				return *this;
			}

			basic_list& operator=(basic_list&& other) noexcept
			{
				if (this != &other)
				{
					size_type i = 0;

					for (auto& val : other)
					{
						set_at(i, std::move(val));
						++i;
					}
				}
				return *this;
			}

			template <container_size_type RSIZE, template <typename, container_size_type> typename RALLOC>
			basic_list& operator=(basic_list<T, RSIZE, RALLOC>&& other) noexcept
			{
				if (this != &other)
				{
					size_type i = 0;

					for (auto& val : other)
					{
						set_at(i, std::move(val));
						++i;
					}
				}
				return *this;
			}


			basic_list& operator=(std::initializer_list<T> ilist)
			{
				size_type i = 0;
				for (auto& val : ilist)
				{
					set_at(i, std::move(val));
					++i;
				}
				return *this;
			}

			void assign(size_type count, const T& value)
			{
				for (size_type i = 0; i < count; i++)
				{
					set_at(i, value);
				}
			}

			template< class InputIt,
				std::enable_if_t<is_iterator<InputIt>::value, int> = 0
			>
				void assign(InputIt first, InputIt last)
			{
				size_type i = 0;
				while (first != last)
				{
					set_at(i, *first);
					++i;
					++first;
				}
			}

			void assign(std::initializer_list<T> ilist)
			{
				assign(ilist.begin(), ilist.end());
			}

			//Element access
			reference front()
			{
				assert(_size > 0);
				return *_ptrs[0];
			}

			const_reference front() const
			{
				assert(_size > 0);
				return *_ptrs[0];
			}

			reference back()
			{
				assert(_size > 0);
				return *_ptrs[_size - 1];
			}

			const_reference back() const
			{
				assert(_size > 0);
				return *_ptrs[_size - 1];
			}

			//Iterators
			iterator begin() noexcept
			{
				if (_size > 0)
					return iterator(_ptrs.data());
				return end();
			}

			const_iterator begin() const noexcept
			{
				return cbegin();
			}

			const_iterator cbegin() const noexcept
			{
				if (_size > 0)
					return const_iterator(_ptrs.data());
				return end();
			}

			iterator end() noexcept
			{
				if (_size > 0)
					return iterator(_ptrs.data() + _size);
				return iterator();
			}

			const_iterator end() const noexcept
			{
				return cend();
			}

			const_iterator cend() const noexcept
			{
				if (_size > 0)
					return const_iterator(_ptrs.data() + _size);
				return const_iterator();
			}

			reverse_iterator rbegin()
			{
				return reverse_iterator(begin());
			}

			const_reverse_iterator rbegin() const
			{
				return crbegin();
			}

			const_reverse_iterator crbegin()
			{
				return const_reverse_iterator(cbegin());
			}

			reverse_iterator rend()
			{
				return reverse_iterator(end());
			}

			const_reverse_iterator rend() const
			{
				return crend();
			}

			const_reverse_iterator crend()
			{
				return const_reverse_iterator(cend());
			}

			//Capacity
			bool empty() const noexcept
			{
				return _size == 0;
			}

			size_type size() const noexcept
			{
				return _size;
			}

			constexpr size_type max_size() const noexcept
			{
				return SIZE;
			}

			//Modifiers
			void clear()
			{
				for (size_type i = 0; i < _size; i++)
				{
					_ptrs[i]->~T();
				}
				_size = 0;
			}

			iterator insert(const_iterator pos, const T& value)
			{
				insert(pos, 1, value);
			}

			iterator insert(const_iterator pos, T&& value)
			{
				auto index = pos - begin();
				assert(index <= _size);
				auto old_size = _size;
				push_back(value);
				if (index != _size)
					std::rotate(_ptrs + index, _ptrs + old_size, _ptrs + old_size + 1);
				return iterator(_ptrs + index);
			}

			iterator insert(const_iterator pos, size_type count, const T& value)
			{
				auto index = pos - begin();
				assert(index <= _size);
				auto old_size = _size;
				for (size_type i = 0; i < count; i++)
					push_back(value);
				if (index != _size && count != 0)
					std::rotate(_ptrs + index, _ptrs + old_size, _ptrs + old_size + count);
				return iterator(_ptrs + index);
			}

			template< class InputIt,
				std::enable_if_t<is_iterator<InputIt>::value, int> = 0
			>
				void insert(const_iterator pos, InputIt first, InputIt last)
			{
				auto index = pos - begin();
				assert(index <= _size);
				auto old_size = _size;
				size_type size_inserted = 0;
				while (first != last)
				{
					push_back(*first);
					++size_inserted;
					++first;
				}
				if (index != _size && size_inserted != 0)
				{
					std::rotate(_ptrs + index, _ptrs + old_size, _ptrs + old_size + size_inserted);
				}
			}

			iterator insert(const_iterator pos, std::initializer_list<T> ilist)
			{
				return insert(pos, ilist.begin(), ilist.end());
			}

			template <class... Args>
			iterator emplace(const_iterator pos, Args&&... args)
			{
				auto index = pos - begin();
				assert(index <= _size);
				auto old_size = _size;
				emplace_back(args...);
				if (index != _size)
					std::rotate(_ptrs + index, _ptrs + old_size, _ptrs + old_size + 1);
				return iterator(_ptrs + index);
			}

			iterator erase(const_iterator pos)
			{
				assert(false);//TODO: implement this
			}

			iterator erase(const_iterator first, const_iterator last)
			{
				assert(false);//TODO: implement this
			}

			void push_back(const T& value)
			{
				assert(_size + 1 < SIZE);
				new (_ptrs[_size]) T(value);
				++_size;
			}

			void push_back(T&& value)
			{
				assert(_size + 1 < max_size());
				new (_ptrs[_size]) T(value);
				++_size;
			}

			template< class... Args>
			reference emplace_back(Args&&... args)
			{
				assert(_size + 1 < SIZE);
				new(_ptrs[_size]) T(args...);
				++_size;
				return back();
			}

			void pop_back()
			{
				assert(_size > 0);
				_ptrs[_size - 1]->~T();
				--_size;
			}

			void push_front(const T& value)
			{
				insert(cbegin(), value);
			}

			void push_front(T&& value)
			{
				insert(cbegin(), value);
			}

			template<class... Args>
			reference emplace_front(Args&&... args)
			{
				emplace_back(args...);
				if (_size > 1)
					std::rotate(_ptrs, _ptrs + _size - 2, _ptrs + size);
				return front();
			}

			void pop_front()
			{
				assert(_size > 0);
				erase(begin());
			}

			void resize(size_type count, const value_type& value)
			{
				while (_size > count)
				{
					pop_back();
				}

				while (_size < count)
				{
					push_back(value);
				}
			}

			void resize(size_type count)
			{
				resize(count, value_type());
			}

			template <container_size_type RSIZE, template <typename, container_size_type> typename RALLOC>
			void swap(basic_list<T, RSIZE, RALLOC>& other)
			{
				size_type i = 0;
				size_type other_size = other._size;
				while (i < _size && i < other_size)
				{
					std::swap(*_ptrs[i], *other._ptrs[i]);
					++i;
				}

				while (i < _size)
				{
					other.push_back(std::move(*_ptrs[i]));
					++i;
				}

				while (i < other_size)
				{
					push_back(std::move(*other._ptrs[i]));
					++i;
				}

				std::swap(_size, other._size);
			}

			//Operations
			template <container_size_type RSIZE, template <typename, container_size_type> typename RALLOC>
			void merge(basic_list<T, RSIZE, RALLOC>&& other)
			{
				merge(other, std::less<T>());
			}

			template <class Compare, container_size_type RSIZE, template <typename, container_size_type> typename RALLOC>
			void merge(basic_list<T, RSIZE, RALLOC>&& other, Compare comp)
			{
				assert(size() + other.size() <= max_size());
				assert(&other != this);
				for (auto& item : other)
				{
					insert(std::lower_bound(begin(), end(), item, comp), std::move(item));
				}
				other.clear();
			}

			template <container_size_type RSIZE, template <typename, container_size_type> typename RALLOC>
			void splice(const_iterator pos, basic_list<T, RSIZE, RALLOC>&& other)
			{
				splice(pos, other, other.begin(), other.end());
			}

			template <container_size_type RSIZE, template <typename, container_size_type> typename RALLOC>
			void splice(const_iterator pos, basic_list<T, RSIZE, RALLOC>&& other, const_iterator it)
			{
				assert(this != &other);
				insert(pos, std::move(it));
				other.erase(it);
			}

			template <container_size_type RSIZE, template <typename, container_size_type> typename RALLOC>
			void splice(const_iterator pos, basic_list<T, RSIZE, RALLOC>&& other, const_iterator first, const_iterator last)
			{
				assert(this != &other);
				auto index = pos - begin();
				assert(index <= _size);
				auto old_size = _size;
				size_type size_inserted = 0;
				while (first != last)
				{
					push_back(std::move(*first));
					++size_inserted;
					++first;
				}
				if (index != _size && size_inserted != 0)
				{
					std::rotate(_ptrs + index, _ptrs + old_size, _ptrs + old_size + size_inserted);
				}
				other.erase(first, last);
			}

			void remove(const T& value)
			{
				remove_if([&value](const auto& v) { return v == value; });
			}

			template <class UnaryPredicate>
			void remove_if(UnaryPredicate p)
			{
				auto it_ptr = std::find_if(_ptrs.data(), _ptrs.data() + _size, [&p](const auto& ptr) { return p(*ptr); });
				if (it_ptr != _ptrs.data())
				{
					it_ptr->~T();
					if (it_ptr != _ptrs.data() + _size - 1)
					{
						std::rotate(it_ptr, it_ptr + 1, _ptrs.data() + _size);
					}
					_size--;
				}
			}

			void reverse() noexcept
			{
				std::reverse(_ptrs.data(), _ptrs.data() + _size);
			}

			void unique()
			{
				unique(std::equal_to<T>());
			}

			template <class BinaryPredicate>
			void unique(BinaryPredicate p)
			{
				if (_size > 1)
				{
					auto beg = _ptrs.data();
					auto end = _ptrs.data() + _size;
					while (beg < end - 2)
					{
						while (p(*beg, (*beg + 1)))
						{
							erase_local_ptr(beg + 1);
							end--;
						}
						beg++;
					}
				}
			}

			void sort()
			{
				sort(std::less<T>());
			}

			template <class Compare>
			void sort(Compare comp)
			{
				std::sort(_ptrs.data(), _ptrs.data() + _size, [&comp](auto& lval, auto& rval)
				{
					return comp(*lval, *rval);
				});
			}

		private:
			void erase_local_ptr(T** ptr)
			{
				assert(_size > 0);
				(*ptr)->~T();
				if (ptr != _ptrs.data() + _size - 1)
				{
					std::rotate(ptr, ptr + 1, _ptrs.data() + _size - 1);
				}
				_size--;
			}
		};
	}
}

#endif //!FIXED_BASIC_LIST_HPP