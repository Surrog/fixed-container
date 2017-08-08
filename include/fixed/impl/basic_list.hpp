#ifndef FIXED_BASIC_LIST_HPP
#define FIXED_BASIC_LIST_HPP

#include <array>
#include <cassert>
#include "basic_allocator.hpp"
#include "fixed_def.hpp"

namespace fixed
{
	namespace _impl
	{
		template <typename T, size_type SIZE,
			typename Allocator = basic_stack_allocator<T, SIZE> >
		class basic_list
		{
			Allocator _data_holder;
			std::array<T*, SIZE> _ptrs;
			size_type _size;
			
			void push_back()
			{
				assert(_size + 1 < SIZE);
				new (_ptrs[_size]) T();
				++_size;
			}

		public:
			typedef T value_type;
			typedef Allocator allocator_type;
			typedef size_type size_type;
			typedef std::ptrdiff_t difference_type;
			typedef value_type& reference;
			typedef const value_type& const_reference;
			typedef value_type* pointer;
			typedef const value_type* const_pointer;


			basic_list() : basic_list(Allocator()) {}
			explicit basic_list(const Allocator& alloc)
				: _data_holder(alloc), _ptrs(), _size(0)
			{
				for (size_type i = 0; i < SIZE; i++)
				{
					_ptrs[i] = _data_holder.data() + i;
				}
			}
			
			basic_list(size_type count, const T& value = T(), const Allocator& alloc = Allocator())
				: basic_list(alloc)
			{
				for (size_type i = 0; i < count; i++)
				{
					push_back(value);
				}
			}

			explicit basic_list(size_type count, const Allocator& alloc = Allocator())
				: basic_list(alloc)
			{
				for (size_type i = 0; i < count; i++)
				{
					push_back();
				}
			}

			template <class InputIt>
			basic_list(InputIt first, InputIt last, const Allocator& alloc = Allocator())
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

			basic_list(const basic_list& other, const Allocator& alloc)
				: basic_list(other.begin(), other.end(), alloc)
			{}

			basic_list(basic_list&& other, const Allocator& alloc)
				: basic_list(alloc)
			{
				for (auto& elem : other)
				{
					push_back(std::move(elem));
				}
			}

			basic_list(basic_list&& other)
				: basic_list(other, Allocator())
			{}

			basic_list(std::initializer_list<T> init, const Allocator& alloc = Allocator())
				: basic_list(init.begin(), init.end(), alloc)
			{}

			~basic_list()
			{}

			void push_back(const T& value)
			{
				assert(_size + 1 < SIZE);
				new (_ptrs[_size]) T(value);
				++_size;
			}

			void push_back(T&& value)
			{
				assert(_size + 1 < SIZE);
				new (_ptrs[_size]) T(value);
				++_size;
			}
		};
	}
}

#endif //!FIXED_BASIC_LIST_HPP