#ifndef BASIC_HEAPLESS_VECTOR_HPP
#define BASIC_HEAPLESS_VECTOR_HPP

#include <algorithm>
#include <iterator>
#include <vector>

#include "fixed/impl/aligned_allocation_pattern.hpp"
#include "fixed/impl/basic_pointer_iterator.hpp"

namespace fixed
{
namespace _impl
{
    template <typename T, container_size_type SIZE,
        template <typename, container_size_type> typename Alloc_pattern
        = aligned_stack_allocator>
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

        template <typename Types, size_type ALLOC_SIZE>
        using allocator_type = Alloc_pattern<Types, ALLOC_SIZE>;

    private:
        // datas

        typedef Alloc_pattern<T, SIZE> allocator_type_impl;

        size_type _size = 0;
        allocator_type_impl _data_container;

    public:
        typedef typename allocator_type_impl::aligned_type aligned_type;
        typedef typename allocator_type_impl::iterator iterator;
        typedef typename allocator_type_impl::const_iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        // destructor
        ~basic_vector() { clear(); }
        //! destructor
        // default constructor
        basic_vector() 
			noexcept(is_nothrow_default_constructible_v<allocator_type_impl>)
            : _size(0)
            , _data_container()
        {
        }

        template <typename Alloc_source,
            std::enable_if_t<fixed::_impl::is_allocation_source_v<Alloc_source>, int> = 0>
        basic_vector(Alloc_source& alloc)
			noexcept(std::is_nothrow_constructible<allocator_type_impl, Alloc_source&>::value)
            : _size(0)
            , _data_container(alloc)
        {
        }
        //! default constructor
        // constructor with count copies
        basic_vector(size_type count, const T& value)
            : basic_vector()
        {
            FIXED_CHECK_FULL(count <= max_size());
            for(size_type i = 0; i < count; i++)
            {
                push_back(value);
            }
        }

        template <typename Alloc_source,
            std::enable_if_t<is_allocation_source_v<Alloc_source>, int> = 0>
        basic_vector(size_type count, const T& value, Alloc_source& source)
            : basic_vector(source)
        {
            FIXED_CHECK_FULL(count <= max_size());
            for(size_type i = 0; i < count; i++)
            {
                push_back(value);
            }
        }

        basic_vector(size_type count)
            : basic_vector(count, T())
        {
        }

        template <typename Alloc_source,
            std::enable_if_t<is_allocation_source_v<Alloc_source>, int> = 0>
        basic_vector(size_type count, Alloc_source& alloc)
            : basic_vector(count, T(), alloc)
        {
        }
        //! constructor with count copies
        // constructor with iterators
        template <typename InputIt,
            std::enable_if_t<fixed::astd::is_iterator_v<InputIt>, int> = 0>
        basic_vector(InputIt first, InputIt last)
            : basic_vector()
        {
            FIXED_CHECK_BADRANGE(std::distance(first, last) >= 0);
            FIXED_CHECK_FULL(
                size_type(std::distance(first, last)) <= max_size());

            while(first != last)
            {
                push_back(*first);
                ++first;
            }
        }

        template <typename InputIt, typename Alloc_source,
            std::enable_if_t<fixed::astd::is_iterator_v<InputIt>, int> = 0,
            std::enable_if_t<is_allocation_source_v<Alloc_source>, int> = 0>
        basic_vector(InputIt first, InputIt last, Alloc_source& source)
            : basic_vector(source)
        {
            FIXED_CHECK_BADRANGE(std::distance(first, last) >= 0);
            FIXED_CHECK_FULL(
                size_type(std::distance(first, last)) <= max_size());

            while(first != last)
            {
                push_back(*first);
                ++first;
            }
        }
        //! constructor with iterators
        // copy constructors
        basic_vector(const basic_vector& other) 
			noexcept(is_nothrow_default_constructible_v<allocator_type_impl>
				&& is_nothrow_copy_constructible_v<T>
				&& is_nothrow_allocator_iterator_v<allocator_type_impl>)
            : basic_vector()
        {
            for(const auto& val : other)
            {
                push_back(val);
            }
        }

        template <typename Alloc_source,
            std::enable_if_t<is_allocation_source_v<Alloc_source>, int> = 0>
        basic_vector(const basic_vector& other, Alloc_source& source)
            : basic_vector(source)
        {
            for(const auto& val : other)
            {
                push_back(val);
            }
        }

        template <container_size_type RSIZE,
            template <typename, container_size_type> typename RAlloc_pattern>
        basic_vector(
            const basic_vector<T, RSIZE, RAlloc_pattern>& other) 
			noexcept(SIZE >= RSIZE
            && is_nothrow_default_constructible_v<allocator_type_impl>
            && is_nothrow_allocator_iterator_v<allocator_type_impl>
            && is_nothrow_copy_constructible_v<T>)
            : basic_vector()
        {
            fixed::astd::constexpr_if<(SIZE < RSIZE)>(
                [ rs = other.size(), mxs = max_size() ]() {
                    FIXED_CHECK_FULL(rs <= mxs);
                });

            for(const auto& val : other)
            {
                push_back(val);
            }
        }

        template <container_size_type RSIZE,
            template <typename, container_size_type> typename RAlloc_pattern,
            typename Alloc_source,
            std::enable_if_t<is_allocation_source_v<Alloc_source>, int> = 0>
        basic_vector(const basic_vector<T, RSIZE, RAlloc_pattern>& other,
            Alloc_source& source)
            : basic_vector(source)
        {
            fixed::astd::constexpr_if<(SIZE < RSIZE)>(
                [ rs = other.size(), mxs = max_size() ]() {
                    FIXED_CHECK_FULL(rs <= mxs);
                });
            for(const auto& val : other)
            {
                push_back(val);
            }
        }
        //! copy constructors

        // move constructors
        basic_vector(basic_vector&& other) noexcept(
            is_nothrow_default_constructible_v<allocator_type_impl>
            && (is_nothrow_move_constructible_v<allocator_type_impl>
                   || (is_nothrow_allocator_iterator_v<allocator_type_impl>
                          && (is_nothrow_move_constructible_v<T>
                                 || is_nothrow_copy_constructible_v<T>))))
            : basic_vector()
        {
			if (this != &other)
			{
				fixed::astd::constexpr_if<std::is_nothrow_move_constructible<allocator_type_impl>::value>(
					[this, &other]() {
					std::swap(_data_container, other._data_container);
					std::swap(_size, other._size);
				}
					)._else([this, &other]() {
					for (auto& val : other)
					{
						push_back(std::move_if_noexcept(val));
					}
					other.clear();
				});
			}
        }

        template <typename Alloc_source,
            std::enable_if_t<is_allocation_source_v<Alloc_source>, int> = 0>
        basic_vector(basic_vector&& other, Alloc_source& source)
            : basic_vector(source)
        {
            for(auto& val : other)
            {
                push_back(std::move_if_noexcept(val));
            }
            other.clear();
        }

        template <container_size_type RSIZE,
            template <typename, container_size_type> typename RAlloc_pattern>
        basic_vector(basic_vector<T, RSIZE, RAlloc_pattern>&& other) noexcept(
            SIZE >= RSIZE
            && std::is_nothrow_constructible<allocator_type_impl>::value
            && (std::is_nothrow_move_constructible<T>::value
                   || std::is_nothrow_copy_constructible<T>::value))
            : basic_vector()
        {
            fixed::astd::constexpr_if<(SIZE < RSIZE)>(
                [ rs = other.size(), mxs = max_size() ]() {
                    FIXED_CHECK_FULL(rs <= mxs);
                });
            for(auto& val : other)
            {
                push_back(std::move_if_noexcept(val));
            }
            other.clear();
        }

        template <container_size_type RSIZE,
            template <typename, container_size_type> typename RAlloc_pattern,
            typename Alloc_source,
            std::enable_if_t<is_allocation_source_v<Alloc_source>, int> = 0>
        basic_vector(basic_vector<T, RSIZE, RAlloc_pattern>&& other,
            Alloc_source& source)
            : basic_vector(source)
        {
            fixed::astd::constexpr_if<(SIZE < RSIZE)>(
                [ rs = other.size(), mxs = max_size() ]() {
                    FIXED_CHECK_FULL(rs <= mxs);
                });
            for(auto& val : other)
            {
                push_back(std::move_if_noexcept(val));
            }
            other.clear();
        }
        //! move contructors
        // init_list constructors
        basic_vector(std::initializer_list<T> init)
            : basic_vector()
        {
            FIXED_CHECK_FULL(init.size() <= max_size());
            for(const auto& val : init)
            {
                push_back(val);
            }
        }

        template <typename Alloc_source,
            std::enable_if_t<is_allocation_source_v<Alloc_source>, int> = 0>
        basic_vector(std::initializer_list<T> init, Alloc_source& source)
            : basic_vector(source)
        {
            FIXED_CHECK_FULL(init.size() <= max_size());
            for(const auto& val : init)
            {
                push_back(val);
            }
        }
        //! init_list constructors

        // copy operator assign
        basic_vector& operator=(const basic_vector& rval) noexcept(
            std::is_nothrow_copy_constructible<T>::value)
        {
            if(this != &rval)
            {
                size_type i = 0;
                for(auto& val : rval)
                {
                    set_at(i, val);
                    ++i;
                }
                resize(rval.size());
            }
            return *this;
        }

        template <container_size_type RSIZE,
            template <typename, container_size_type> typename RAlloc_pattern>
        basic_vector&
        operator=(const basic_vector<T, RSIZE, RAlloc_pattern>& rval) noexcept(
            SIZE >= RSIZE && std::is_nothrow_copy_constructible<T>::value)
        {
            size_type i = 0;
            for(auto& val : rval)
            {
                set_at(i, val);
                ++i;
            }
            resize(rval.size());
            return *this;
        }
        //! copy operator assign
        // move operator assign
        basic_vector& operator=(basic_vector&& rval) noexcept(
            std::is_nothrow_copy_constructible<T>::value
            || std::is_nothrow_move_constructible<T>::value)
        {
            if(this != &rval)
            {
                size_type i = 0;
                for(auto& val : rval)
                {
                    set_at(i, std::move(val));
                    ++i;
                }
                while(size() > rval.size())
                    pop_back();
                rval.clear();
            }
            return *this;
        }

        template <size_type RSIZE,
            template <typename, size_type> typename RAllocator>
        basic_vector& operator=(
            basic_vector<T, RSIZE, RAllocator>&& rval) noexcept(SIZE >= RSIZE
            && (std::is_nothrow_copy_constructible<T>::value
                   || std::is_nothrow_move_constructible<T>::value))
        {
            fixed::astd::constexpr_if<(SIZE < RSIZE)>(
                [ rs = rval.size(), mxs = max_size() ]() {
                    FIXED_CHECK_FULL(rs <= mxs);
                });
            size_type i = 0;
            for(auto& val : rval)
            {
                set_at(i, std::move(val));
                ++i;
            }
            while(size() > rval.size())
                pop_back();
            rval.clear();
            return *this;
        }
        //! move operator assign

        // element access
        T& at(size_type n)
        {
            FIXED_CHECK_INBOUND(n < _size);
            return begin()[n];
        }
        const T& at(size_type n) const
        {
            FIXED_CHECK_INBOUND(n < _size);
            return begin()[n];
        }
        T& operator[](size_type n) { return begin()[n]; }
        const T& operator[](size_type n) const { return begin()[n]; }
        T& front() { return at(0); }
        const T& front() const { return at(0); }
        T& back()
        {
            FIXED_CHECK_EMPTY(_size > 0);
            return at(_size - 1);
        }
        const T& back() const
        {
            FIXED_CHECK_EMPTY(_size > 0);
            return at(_size - 1);
        }

        aligned_type* data() { return _data_container.data(); }
        const aligned_type* data() const { return _data_container.data(); }

        // iterators
        iterator begin() { return _data_container.begin(); }
		const_iterator begin() const { return cbegin(); }
        const_iterator cbegin() const { return _data_container.cbegin(); }
        iterator end() { return iterator(_data_container.data() + _size); }
		const_iterator end() const { return cend(); }
        const_iterator cend() const { return const_iterator(_data_container.data() + _size); }

        reverse_iterator rbegin() { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator(end());
        }
        const_reverse_iterator crbegin() const
        {
            return const_reverse_iterator(end());
        }
        reverse_iterator rend() { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const
        {
            return const_reverse_iterator(begin());
        }
        const_reverse_iterator crend() const
        {
            return const_reverse_iterator(begin());
        }

        // capacity
        bool empty() const { return _size == 0; }
        size_type size() const { return _size; }
        constexpr size_type max_size() const
        {
            return _data_container.max_size();
        }
        constexpr void reserve(size_type) const {}
        constexpr size_type capacity() const { return max_size(); }
        constexpr void shrink_to_fit() const {}

        void resize(size_type n) { resize(n, T()); }

        void resize(size_type n, const T& val)
        {
            while(_size > n)
            {
                pop_back();
            }
            while(_size < n)
            {
                push_back(val);
            }
        }

        // Modifiers
        void assign(size_type count, const T& value)
        {
            FIXED_CHECK_FULL(count <= max_size());
            _size = count;
            for(auto& val : *this)
            {
                new(&val) T(value);
            }
        }

        template <class InputIt,
            std::enable_if_t<fixed::astd::is_iterator_v<InputIt>, int> = 0>
        void assign(InputIt first, InputIt last)
        {
            container_size_type size = std::distance(first, last);
            FIXED_CHECK_FULL(size <= max_size());
            size_type i = 0;
            while(first != last)
            {
                set_at(i, *first);
                ++i;
                ++first;
            }
            _size = size;
        }

        void assign(std::initializer_list<T> list)
        {
            FIXED_CHECK_FULL(list.size() <= max_size());
            size_type i = 0;
            for(const T& val : list)
            {
                set_at(i, val);
                ++i;
            }
            _size = list.size();
        }

        void push_back(const T& value)
        {
            FIXED_CHECK_FULL(_size < max_size());
            new(&*end()) T(value);
            _size++;
        }

        void push_back(T&& value)
        {
            FIXED_CHECK_FULL(_size < max_size());
            new(&*end()) T(std::move(value));
            _size++;
        }

        void pop_back()
        {
            FIXED_CHECK_EMPTY(_size > 0);
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
            push_back(std::move(value));
            if(pos != cend() && pos != const_iterator())
            {
                std::rotate(
                    begin() + insert_index, begin() + pivot_index, end());
            }
            return begin() + insert_index;
        }

        iterator insert(const_iterator pos, size_type count, const T& value)
        {
            auto pivot_index = _size;
            auto insert_index = std::distance(cbegin(), pos);
            for(size_type i = 0; i < count; i++)
            {
                push_back(value);
            }
            if(pos != cend() && pos != const_iterator())
            {
                std::rotate(
                    begin() + insert_index, begin() + pivot_index, end());
            }
            return begin() + insert_index;
        }

        template <class InputIt,
            std::enable_if_t<fixed::astd::is_iterator_v<InputIt>, int> = 0>
        iterator insert(const_iterator pos, InputIt first, InputIt last)
        {
            auto pivot_index = _size;
            auto insert_index = std::distance(cbegin(), pos);
            while(first != last)
            {
                push_back(*first);
                first++;
            }
            if(pos != cend() && pos != const_iterator())
            {
                std::rotate(
                    begin() + insert_index, begin() + pivot_index, end());
            }
            return begin() + insert_index;
        }

        iterator insert(const_iterator pos, std::initializer_list<T> ilist)
        {
            return insert(pos, ilist.begin(), ilist.end());
        }

        iterator erase(const_iterator position)
        {
            FIXED_CHECK_EMPTY(_size > 0);
            container_size_type index = std::distance(cbegin(), position);
            FIXED_CHECK_INBOUND(index < _size);
            if(index != _size - 1)
            {
                std::rotate(begin() + index, begin() + index + 1, end());
            }
            pop_back();
            return begin() + std::min(index, _size);
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
            {
                std::rotate(begin() + beg_i, begin() + end_i, end());
            }
            for(container_size_type i = 0; i < (end_i - beg_i); i++)
            {
                pop_back();
            }
            return begin() + std::min(beg_i, _size);
        }

        template <size_type RSIZE,
            template <typename, container_size_type> typename RAlloc_pattern>
        void swap(basic_vector<T, RSIZE, RAlloc_pattern>& rval)
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

        void clear() noexcept
        {
            std::for_each(begin(), end(), [](auto& val) { val.~T(); });
            _size = 0;
        }

        template <class... Args>
        iterator emplace(const_iterator position, Args&&... args)
        {
            container_size_type index = std::distance(cbegin(), position);
            FIXED_CHECK_INBOUND(index <= _size);
            emplace_back(std::forward<Args>(args)...);
            std::rotate(begin() + index, end() - 1, end());
            return begin() + index;
        }

        template <class... Args> reference emplace_back(Args&&... args)
        {
            FIXED_CHECK_FULL(_size < max_size());
            new(&*end()) T(std::forward<Args>(args)...);
            _size++;
            return back();
        }

    private:
        // managing uninitialized memory
        template <class InputIt,
            std::enable_if_t<fixed::astd::is_iterator_v<InputIt>, int> = 0>
        void uninitialized_assign(InputIt begin, InputIt end)
        {
            FIXED_CHECK_FULL(std::distance(end, begin) <= int(max_size()));
            while(begin != end)
            {
                push_back(*begin);
                ++begin;
            }
        }

        void set_at(size_type index, const T& value)
        {
            if(index < _size)
            {
                at(index) = value;
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
                at(index) = std::move(value);
            }
            else
            {
                push_back(std::move(value));
            }
        }

        void uninitialized_assign(size_type count, const T& value)
        {
            FIXED_CHECK_FULL(count <= max_size());

            for(size_type i = 0; i < count; i++)
            {
                new(_data_container.data() + i) T(value);
                _size++;
            }
        }

        void uninitialized_assign(size_type count)
        {
            FIXED_CHECK_FULL(count <= max_size());

            for(size_type i = 0; i < count; i++)
            {
                new(_data_container.data() + i) T();
                _size++;
            }
        }
    };

    template <typename T, container_size_type LSIZE,
        template <typename, container_size_type> typename LALLOCATOR,
        typename STDALLOC>
    bool operator==(const basic_vector<T, LSIZE, LALLOCATOR>& lval,
        const std::vector<T, STDALLOC>& rval)
    {
        return lval.size() == rval.size()
            && std::equal(lval.begin(), lval.end(), rval.begin(), rval.end());
    }
}
}

#endif //! BASIC_HEAPLESS_VECTOR_HPP