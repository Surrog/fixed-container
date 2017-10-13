#ifndef FIXED_BASIC_LIST_HPP
#define FIXED_BASIC_LIST_HPP

#include "basic_listed_vector_iterator.hpp"
#include "fixed/impl/aligned_allocation_pattern.hpp"
#include "fixed/impl/fixed_def.hpp"
#include <algorithm>
#include <array>
#include <utility>

namespace fixed
{
namespace _impl
{
    template <typename T, container_size_type SIZE,
        template <typename, container_size_type> typename Alloc_pattern
        = aligned_stack_allocator>
    class basic_listed_vector
    {
    public:
        typedef T value_type;
        typedef container_size_type size_type;
        typedef std::ptrdiff_t difference_type;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;

        template <typename Types, size_type ALLOC_SIZE>
        using allocator_type = Alloc_pattern<Types, ALLOC_SIZE>;

    private:
        typedef Alloc_pattern<T, SIZE> allocator_type_data_impl;
        typedef Alloc_pattern<typename allocator_type_data_impl::iterator, SIZE>
            allocator_type_ptrs_impl;

        allocator_type_data_impl _data_holder;
        allocator_type_ptrs_impl _ptrs;
        size_type _size = 0;

        void push_back()
        {
            FIXED_CHECK_FULL(_size < max_size());
			if (_size == 0) initialize_ptrs();
            new(_ptrs[_size].get()) T();
            ++_size;
        }

        void set_at(size_type index, const T& value)
        {
            if(index < _size)
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
            if(index < _size)
            {
                *_ptrs[index] = std::move(value);
            }
            else
            {
                push_back(std::move(value));
            }
        }

		void initialize_ptrs() 
			noexcept(is_nothrow_allocator_iterator_v<allocator_type_data_impl>)
		{
			for (size_type i = 0; i < _data_holder.max_size(); i++)
			{
				_ptrs[i] = _data_holder.begin() + i;
			}
		}

    public:
        typedef basic_listed_vector_iterator<T,
            typename allocator_type_ptrs_impl::iterator>
            iterator;
        typedef const_basic_list_iterator<T,
            typename allocator_type_ptrs_impl::const_iterator>
            const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		// default constructor
        basic_listed_vector() noexcept(
            std::is_nothrow_default_constructible<allocator_type_data_impl>::value &&
                std::is_nothrow_default_constructible<allocator_type_ptrs_impl>::value)
            : _data_holder()
            , _ptrs()
            , _size(0)
        {
        }

        template <typename Alloc_source,
            std::enable_if_t<is_allocation_source_v<Alloc_source>, int> = 0>
        explicit basic_listed_vector(Alloc_source& alloc)
			noexcept(
				std::is_nothrow_constructible<allocator_type_data_impl, Alloc_source&>::value &&
				std::is_nothrow_constructible<allocator_type_ptrs_impl, Alloc_source&>::value)
            : _data_holder(alloc)
            , _ptrs(alloc)
            , _size(0)
        {}
		//!default constructor
		// constructor with count copies
        explicit basic_listed_vector(size_type count)
            : basic_listed_vector()
        {
            for(size_type i = 0; i < count; i++)
            {
                push_back();
            }
        }

        template <typename Alloc_source,
            std::enable_if_t<is_allocation_source_v<Alloc_source>, int> = 0>
        explicit basic_listed_vector(size_type count, Alloc_source& alloc)
            : basic_listed_vector(alloc)
        {
            for(size_type i = 0; i < count; i++)
            {
                push_back();
            }
        }

        basic_listed_vector(size_type count, const T& value)
            : basic_listed_vector()
        {
            for(size_type i = 0; i < count; i++)
            {
                push_back(value);
            }
        }

        template <typename Alloc_source,
            std::enable_if_t<is_allocation_source_v<Alloc_source>, int> = 0>
        basic_listed_vector(
            size_type count, const T& value, Alloc_source& alloc)
            : basic_listed_vector(alloc)
        {
            for(size_type i = 0; i < count; i++)
            {
                push_back(value);
            }
        }
		//!constructor with count copies
		// constructor with iterators
        template <class InputIt,
            std::enable_if_t<fixed::astd::is_iterator_v<InputIt>, int> = 0>
        basic_listed_vector(InputIt first, InputIt last)
            : basic_listed_vector()
        {
            while(first != last)
            {
                push_back(*first);
                ++first;
            }
        }

        template <class InputIt, class Alloc_source = empty_source,
            std::enable_if_t<fixed::astd::is_iterator_v<InputIt>, int> = 0,
            std::enable_if_t<is_allocation_source_v<Alloc_source>, int> = 0>
        basic_listed_vector(InputIt first, InputIt last, Alloc_source& alloc)
            : basic_listed_vector(alloc)
        {
            while(first != last)
            {
                push_back(*first);
                ++first;
            }
        }
		//!constructor with iterators
		// copy constructors
        basic_listed_vector(const basic_listed_vector& other)
			noexcept(is_nothrow_default_constructible_v<allocator_type_data_impl>
				&& is_nothrow_default_constructible_v<allocator_type_ptrs_impl>
				&& is_nothrow_copy_constructible_v<T>
				&& is_nothrow_allocator_iterator_v<allocator_type_data_impl>
				&& is_nothrow_allocator_iterator_v<allocator_type_ptrs_impl>)
			: basic_listed_vector()
        {
			for (const auto& val : other)
			{
				push_back(val);
			}
        }

        template <class Alloc_source,
            std::enable_if_t<is_allocation_source_v<Alloc_source>, int> = 0>
        basic_listed_vector(
            const basic_listed_vector& other, Alloc_source& alloc)
            : basic_listed_vector(alloc)
        {
			for (const auto& val : other)
			{
				push_back(val);
			}
        }

        template <container_size_type RSIZE,
            template <typename, container_size_type> typename RALLOC>
        basic_listed_vector(const basic_listed_vector<T, RSIZE, RALLOC>& other)
			noexcept(SIZE >= RSIZE &&
				is_nothrow_default_constructible_v<allocator_type_data_impl>
				&& is_nothrow_default_constructible_v<allocator_type_ptrs_impl>
				&& is_nothrow_copy_constructible_v<T>
				&& is_nothrow_allocator_iterator_v<allocator_type_data_impl>
				&& is_nothrow_allocator_iterator_v<allocator_type_ptrs_impl>)
			: basic_listed_vector()
        {
			fixed::astd::constexpr_if<(SIZE < RSIZE)>(
				[rs = other.size(), mxs = max_size()]() {
				FIXED_CHECK_FULL(rs <= mxs);
			});

			for (const auto& val : other)
			{
				push_back(val);
			}
        }

        template <container_size_type RSIZE,
            template <typename, container_size_type> typename RALLOC,
            class Alloc_source,
            std::enable_if_t<is_allocation_source_v<Alloc_source>, int> = 0>
        basic_listed_vector(const basic_listed_vector<T, RSIZE, RALLOC>& other,
            Alloc_source& alloc)
            : basic_listed_vector(alloc)
        {
			fixed::astd::constexpr_if<(SIZE < RSIZE)>(
				[rs = other.size(), mxs = max_size()]() {
				FIXED_CHECK_FULL(rs <= mxs);
			});
			for (const auto& val : other)
			{
				push_back(val);
			}
        }

        basic_listed_vector(basic_listed_vector&& other) noexcept
            : basic_listed_vector()
        {
            for(T& elem : other)
            {
                push_back(std::move_if_noexcept(elem));
            }
            other.clear();
        }

        template <class Alloc_source = empty_source,
            std::enable_if_t<is_allocation_source_v<Alloc_source>, int> = 0>
        basic_listed_vector(
            basic_listed_vector&& other, Alloc_source& alloc) noexcept
            : basic_listed_vector(alloc)
        {
            for(auto& elem : other)
            {
                push_back(std::move_if_noexcept(elem));
            }
            other.clear();
        }

        template <container_size_type RSIZE,
            template <typename, container_size_type> typename RALLOC,
            class Alloc_source = empty_source,
            std::enable_if_t<is_allocation_source_v<Alloc_source>, int> = 0>
        basic_listed_vector(
            basic_listed_vector<T, RSIZE, RALLOC>&& other, Alloc_source& alloc)
            : basic_listed_vector(alloc)
        {
            FIXED_CHECK_FULL(other.size() <= max_size());

            for(auto& elem : other)
            {
                push_back(std::move_if_noexcept(elem));
            }
            other.clear();
        }

        template <container_size_type RSIZE,
            template <typename, container_size_type> typename RALLOC>
        basic_listed_vector(basic_listed_vector<T, RSIZE, RALLOC>&& other)
            : basic_listed_vector()
        {
            FIXED_CHECK_FULL(other.size() <= max_size());
            for(auto& elem : other)
            {
                push_back(std::move_if_noexcept(elem));
            }
            other.clear();
        }

        // init list constructor
        basic_listed_vector(std::initializer_list<T> init)
            : basic_listed_vector(init.begin(), init.end())
        {
        }

        template <class Alloc_source,
            std::enable_if_t<is_allocation_source_v<Alloc_source>, int> = 0>
        basic_listed_vector(std::initializer_list<T> init, Alloc_source& alloc)
            : basic_listed_vector(init.begin(), init.end(), alloc)
        {
        }
        //! init list constructor

        ~basic_listed_vector() { clear(); }

        basic_listed_vector& operator=(const basic_listed_vector& other)
        {
            if(this != &other)
            {
                size_type i = 0;
                for(auto& val : other)
                {
                    set_at(i, val);
                    ++i;
                }
                resize(other.size());
            }
            return *this;
        }

        template <container_size_type RSIZE,
            template <typename, container_size_type> typename RALLOC>
        basic_listed_vector& operator=(
            const basic_listed_vector<T, RSIZE, RALLOC>& other)
        {
            size_type i = 0;
            for(auto& val : other)
            {
                set_at(i, val);
                ++i;
            }
            resize(other.size());
            return *this;
        }

        basic_listed_vector& operator=(basic_listed_vector&& other) noexcept
        {
            if(this != &other)
            {
                size_type i = 0;

                for(auto& val : other)
                {
                    set_at(i, std::move_if_noexcept(val));
                    ++i;
                }
            }
            other.clear();
            return *this;
        }

        template <container_size_type RSIZE,
            template <typename, container_size_type> typename RALLOC>
        basic_listed_vector& operator=(
            basic_listed_vector<T, RSIZE, RALLOC>&& other)
        {
            FIXED_CHECK_FULL(other.size() <= max_size());
            if((void*)this != (void*)&other)
            {
                size_type i = 0;

                for(auto& val : other)
                {
                    set_at(i, std::move_if_noexcept(val));
                    ++i;
                }
            }
            other.clear();
            return *this;
        }

        basic_listed_vector& operator=(std::initializer_list<T> ilist)
        {
            FIXED_CHECK_FULL(ilist.size() <= std::size_t(max_size()));

            size_type i = 0;
            for(auto& val : ilist)
            {
                set_at(i, val);
                ++i;
            }
            return *this;
        }

        void assign(size_type count, const T& value)
        {
            for(size_type i = 0; i < count; i++)
            {
                set_at(i, value);
            }
        }

        template <class InputIt,
            std::enable_if_t<fixed::astd::is_iterator_v<InputIt>, int> = 0>
        void assign(InputIt first, InputIt last)
        {
            size_type i = 0;
            while(first != last)
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

        // Element access
        reference front()
        {
            FIXED_CHECK_EMPTY(_size > 0);
            return *_ptrs[0];
        }

        const_reference front() const
        {
            FIXED_CHECK_EMPTY(_size > 0);
            return *_ptrs[0];
        }

        reference back()
        {
            FIXED_CHECK_EMPTY(_size > 0);
            return *_ptrs[_size - 1];
        }

        const_reference back() const
        {
            FIXED_CHECK_EMPTY(_size > 0);
            return *_ptrs[_size - 1];
        }

        // Iterators
        iterator begin() noexcept
        {
            if(_size > 0)
                return iterator(_ptrs.begin());
            return end();
        }

        const_iterator begin() const noexcept { return cbegin(); }

        const_iterator cbegin() const noexcept
        {
            if(_size > 0)
                return const_iterator(_ptrs.begin());
            return end();
        }

        iterator end() noexcept
        {
            if(_size > 0)
                return iterator(_ptrs.begin() + _size);
            return iterator();
        }

        const_iterator end() const noexcept { return cend(); }

        const_iterator cend() const noexcept
        {
            if(_size > 0)
                return const_iterator(_ptrs.begin() + _size);
            return const_iterator();
        }

        reverse_iterator rbegin() { return reverse_iterator(end()); }

        const_reverse_iterator rbegin() const { return crbegin(); }

        const_reverse_iterator crbegin()
        {
            return const_reverse_iterator(cend());
        }

        reverse_iterator rend() { return reverse_iterator(begin()); }

        const_reverse_iterator rend() const { return crend(); }

        const_reverse_iterator crend()
        {
            return const_reverse_iterator(cbegin());
        }

        // Capacity
        bool empty() const noexcept { return _size == 0; }

        size_type size() const noexcept { return _size; }

        constexpr size_type max_size() const noexcept { return SIZE; }

        // Modifiers
        void clear()
        {
            for(size_type i = 0; i < _size; i++)
            {
                _ptrs[i].get()->~T();
            }
            _size = 0;
        }

        iterator insert(const_iterator pos, const T& value)
        {
            return insert(pos, 1, value);
        }

        iterator insert(const_iterator pos, T&& value)
        {
            container_size_type index = std::distance(cbegin(), pos);
            FIXED_CHECK_INBOUND(index <= _size);
            auto old_size = _size;
            push_back(std::move(value));
            if(index != _size)
                std::rotate(_ptrs.begin() + index, _ptrs.begin() + old_size,
                    _ptrs.begin() + old_size + 1);
            return iterator(_ptrs.begin() + index);
        }

        iterator insert(const_iterator pos, size_type count, const T& value)
        {
            container_size_type index = std::distance(cbegin(), pos);
            FIXED_CHECK_INBOUND(index <= _size);
            auto old_size = _size;
            for(size_type i = 0; i < count; i++)
                push_back(value);
            if(index != _size && count != 0)
                std::rotate(_ptrs.begin() + index, _ptrs.begin() + old_size,
                    _ptrs.begin() + old_size + count);
            return iterator(_ptrs.begin() + index);
        }

        template <class InputIt,
            std::enable_if_t<fixed::astd::is_iterator_v<InputIt>, int> = 0>
        iterator insert(const_iterator pos, InputIt first, InputIt last)
        {
            container_size_type index = std::distance(cbegin(), pos);
            FIXED_CHECK_INBOUND(index <= _size);
            auto old_size = _size;
            size_type size_inserted = 0;
            while(first != last)
            {
                push_back(*first);
                ++size_inserted;
                ++first;
            }
            if(index != _size && size_inserted != 0)
            {
                std::rotate(_ptrs.begin() + index, _ptrs.begin() + old_size,
                    _ptrs.begin() + old_size + size_inserted);
            }
            return iterator(_ptrs.begin() + index);
        }

        iterator insert(const_iterator pos, std::initializer_list<T> ilist)
        {
            return insert(pos, ilist.begin(), ilist.end());
        }

        template <class... Args>
        iterator emplace(const_iterator pos, Args&&... args)
        {
            container_size_type index = std::distance(cbegin(), pos);
            FIXED_CHECK_INBOUND(index <= _size);
            auto old_size = _size;
            emplace_back(std::forward<Args>(args)...);
            if(index != _size)
                std::rotate(_ptrs.begin() + index, _ptrs.begin() + old_size,
                    _ptrs.begin() + old_size + 1);
            return iterator(_ptrs.begin() + index);
        }

        iterator erase(const_iterator pos)
        {
            FIXED_CHECK_EMPTY(_size > 0);
            container_size_type index = std::distance(cbegin(), pos);
            FIXED_CHECK_INBOUND(index < _size);
            if(index != _size - 1) // move object to the back
                std::rotate(_ptrs.begin() + index, _ptrs.begin() + index + 1,
                    _ptrs.begin() + _size);
            pop_back(); // pop it !
            return iterator(_ptrs.begin() + std::min(index, _size));
        }

        iterator erase(const_iterator first, const_iterator last)
        {
            FIXED_CHECK_EMPTY(_size > 0);
            container_size_type beg_i = std::distance(cbegin(), first);
            container_size_type end_i = std::distance(cbegin(), last);
            FIXED_CHECK_INBOUND(beg_i < _size);
            FIXED_CHECK_INBOUND(end_i <= _size);
            FIXED_CHECK_BADRANGE(beg_i < end_i);
            if(end_i != _size)
                std::rotate(_ptrs.begin() + beg_i, _ptrs.begin() + end_i,
                    _ptrs.begin() + _size);
            for(container_size_type i = 0; i < (end_i - beg_i); ++i)
            {
                pop_back();
            }
            if(_size)
                return begin() + std::min(beg_i, _size);
            return end();
        }

        void push_back(const T& value)
        {
			if (_size == 0) initialize_ptrs();
            FIXED_CHECK_FULL(_size < max_size());
            new(_ptrs[_size].get()) T(value);
            ++_size;
        }

        void push_back(T&& value)
        {
			if (_size == 0) initialize_ptrs();
            FIXED_CHECK_FULL(_size < max_size());
            new(_ptrs[_size].get()) T(std::move(value));
            ++_size;
        }

        template <class... Args> reference emplace_back(Args&&... args)
        {
			if (_size == 0) initialize_ptrs();
            FIXED_CHECK_FULL(_size < max_size());
            new(_ptrs[_size].get()) T(std::forward<Args>(args)...);
            ++_size;
            return back();
        }

        void pop_back()
        {
            FIXED_CHECK_EMPTY(_size > 0);
            _ptrs[_size - 1].get()->~T();
            --_size;
        }

        void push_front(const T& value) { insert(cbegin(), value); }

        void push_front(T&& value) { insert(cbegin(), std::move(value)); }

        template <class... Args> reference emplace_front(Args&&... args)
        {
            emplace_back(args...);
            if(_size > 1)
                std::rotate(_ptrs.begin(), _ptrs.begin() + _size - 1,
                    _ptrs.begin() + _size);
            return front();
        }

        void pop_front()
        {
            FIXED_CHECK_EMPTY(_size > 0);
            erase(begin());
        }

        void resize(size_type count, const value_type& value)
        {
            while(_size > count)
            {
                pop_back();
            }

            while(_size < count)
            {
                push_back(value);
            }
        }

        void resize(size_type count) { resize(count, value_type()); }

        template <container_size_type RSIZE,
            template <typename, container_size_type> typename RALLOC>
        void swap(basic_listed_vector<T, RSIZE, RALLOC>& rval)
        {
            FIXED_CHECK_FULL(rval.size() < max_size());
            FIXED_CHECK_FULL(size() < rval.max_size());

            size_type lsize = size();
            size_type rsize = rval.size();

            auto lbeg = begin();
            auto lend = end();
            auto rbeg = rval.begin();
            auto rend = rval.end();

            while(lbeg != lend && rbeg != rend)
            {
                std::swap(*lbeg, *rbeg);
                ++lbeg;
                ++rbeg;
            }

            if(lbeg != lend)
            {
                while(lbeg != lend)
                {
                    rval.push_back(std::move_if_noexcept(*lbeg));
                    ++lbeg;
                }
                resize(rsize);
            }
            else if(rbeg != rend)
            {
                while(rbeg != rend)
                {
                    push_back(std::move_if_noexcept(*rbeg));
                    ++rbeg;
                }
                rval.resize(lsize);
            }
        }

        // Operations
        template <container_size_type RSIZE,
            template <typename, container_size_type> typename RALLOC>
        void merge(basic_listed_vector<T, RSIZE, RALLOC>&& other)
        {
            merge(std::move(other), std::less<T>());
        }

        template <class Compare, container_size_type RSIZE,
            template <typename, container_size_type> typename RALLOC>
        void merge(basic_listed_vector<T, RSIZE, RALLOC>&& other, Compare comp)
        {
            if(static_cast<void*>(&other) != static_cast<void*>(this))
            {
                FIXED_CHECK_FULL(size() + other.size() <= max_size());
                for(auto& item : other)
                {
                    insert(std::lower_bound(begin(), end(), item, comp),
                        std::move(item));
                }
                other.clear();
            }
        }

        template <container_size_type RSIZE,
            template <typename, container_size_type> typename RALLOC>
        void splice(
            const_iterator pos, basic_listed_vector<T, RSIZE, RALLOC>&& other)
        {
            splice(pos, std::move(other), other.cbegin(), other.cend());
        }

        template <container_size_type RSIZE,
            template <typename, container_size_type> typename RALLOC>
        void splice(const_iterator pos,
            basic_listed_vector<T, RSIZE, RALLOC>&& other, const_iterator it)
        {
            FIXED_CHECK_CUSTOM(
                (void*)this != (void*)&other, "Splice with itself");
            auto elem_i = std::distance(other.cbegin(), it);
            FIXED_CHECK_INBOUND(elem_i > 0);
            FIXED_CHECK_INBOUND(container_size_type(elem_i) <= other.size());

            insert(pos, std::move(*(other.begin() + elem_i)));
            other.erase(it);
        }

        template <container_size_type RSIZE,
            template <typename, container_size_type> typename RALLOC>
        void splice(const_iterator pos,
            basic_listed_vector<T, RSIZE, RALLOC>&& other, const_iterator first,
            const_iterator last)
        {
            FIXED_CHECK_CUSTOM(this != &other, "Splice with itself");

            auto pos_i = std::distance(cbegin(), pos);
            auto initial_size = size();
            FIXED_CHECK_INBOUND(pos_i >= 0);
            FIXED_CHECK_INBOUND(container_size_type(pos_i) <= _size);

            auto first_i = std::distance(other.cbegin(), first);
            auto last_i = std::distance(other.cbegin(), last);
            auto range_size = std::distance(first, last);
            FIXED_CHECK_BADRANGE(range_size >= 0);
            if(range_size > 0)
            {
                FIXED_CHECK_FULL(size() + range_size <= max_size());
                FIXED_CHECK_INBOUND(first_i >= 0);
                FIXED_CHECK_INBOUND(container_size_type(first_i) <= other.size());
                FIXED_CHECK_INBOUND(last_i >= 0);
                FIXED_CHECK_INBOUND(container_size_type(last_i) <= other.size());

                auto first_m_it = other.begin() + first_i;
                auto last_m_it = other.begin() + last_i;

                while(first_m_it != last_m_it)
                {
                    push_back(std::move_if_noexcept(*first_m_it));
                    ++first_m_it;
                }
                std::rotate(_ptrs.begin() + pos_i, _ptrs.begin() + initial_size,
                    _ptrs.begin() + size());
                other.erase(first, last);
            }
        }

        void remove(const T& value)
        {
            remove_if([&value](const auto& v) { return v == value; });
        }

        template <class UnaryPredicate> void remove_if(UnaryPredicate p)
        {
            auto found = std::find_if(begin(), end(), p);
            while(found != end())
            {
                erase(found);
                found = std::find_if(begin(), end(), p);
            }
        }

        void reverse() noexcept
        {
            std::reverse(_ptrs.begin(), _ptrs.begin() + _size);
        }

        void unique() { unique(std::equal_to<T>()); }

        template <class BinaryPredicate> void unique(BinaryPredicate p)
        {
            auto it_beg = begin();
            auto it_end = end();

            while(it_beg != it_end && it_beg + 1 != it_end)
            {
                while(it_beg + 1 != it_end && p(*it_beg, *(it_beg + 1)))
                {
                    erase(it_beg + 1);
                    --it_end;
                }
                ++it_beg;
            }
        }

        void sort() { sort(std::less<T>()); }

        template <class Compare> void sort(Compare comp)
        {
            std::sort(_ptrs.begin(), _ptrs.begin() + _size,
                [&comp](auto& lval, auto& rval) { return comp(*lval, *rval); });
        }

    private:
        void erase_local_ptr(T** ptr)
        {
            FIXED_CHECK_EMPTY(_size > 0);
            (*ptr)->~T();
            if(ptr != _ptrs.begin() + _size - 1)
            {
                std::rotate(ptr, ptr + 1, _ptrs.begin() + _size - 1);
            }
            _size--;
        }
    };
}
}

#endif //! FIXED_BASIC_LIST_HPP