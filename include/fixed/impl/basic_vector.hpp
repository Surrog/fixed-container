#ifndef BASIC_HEAPLESS_VECTOR_HPP
#define BASIC_HEAPLESS_VECTOR_HPP

#include <algorithm>
#include <iterator>
#include <vector>

#include "fixed/impl/basic_allocation_pattern.hpp"
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

        // Basic function
        ~basic_vector() { clear(); }

        basic_vector()
            : _size(0)
            , _data_container()
        {
        }

        template <typename Alloc_source = empty_source,
            std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0>
        explicit basic_vector(Alloc_source& alloc)
            : _size(0)
            , _data_container(alloc)
        {
        }

        basic_vector(size_type count, const T& value)
            : basic_vector()
        {
            uninitialized_assign(count, value);
        }

        template <typename Alloc_source = empty_source,
            std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0>
        basic_vector(size_type count, const T& value, Alloc_source& alloc)
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
            std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0>
        explicit basic_vector(size_type count, Alloc_source& alloc)
            : basic_vector(alloc)
        {
            uninitialized_assign(count);
        }

        template <class InputIt,
            std::enable_if_t<is_iterator<InputIt>::value, int> = 0>
        basic_vector(InputIt first, InputIt last)
            : basic_vector()
        {
            uninitialized_assign(first, last);
        }

        template <class InputIt, typename Alloc_source = empty_source,
            std::enable_if_t<is_iterator<InputIt>::value, int> = 0,
            std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0>
        basic_vector(InputIt first, InputIt last, Alloc_source& alloc)
            : basic_vector(alloc)
        {
            uninitialized_assign(first, last);
        }

        basic_vector(const basic_vector& orig)
        {
            uninitialized_assign(orig.begin(), orig.end());
        }

        basic_vector(basic_vector&& orig) { operator=(std::move(orig)); }

        template <size_type RSIZE,
            template <typename, size_type> typename RAllocator>
        basic_vector(const basic_vector<T, RSIZE, RAllocator>& orig)
            : basic_vector()
        {
            uninitialized_assign(orig.begin(), orig.end());
        }

        template <size_type RSIZE,
            template <typename, size_type> typename RAllocator,
            class Alloc_source = empty_source,
            std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0>
        basic_vector(
            const basic_vector<T, RSIZE, RAllocator>& orig, Alloc_source& alloc)
            : basic_vector(alloc)
        {
            uninitialized_assign(orig.begin(), orig.end());
        }

        template <size_type RSIZE>
        basic_vector(basic_vector<T, RSIZE, Alloc_pattern>&& other)
            : _size(0)
            , _data_container()
        {
            FIXED_CHECK_FULL(other.size() <= max_size());

            // constexpr if (Alloc_pattern::allocation_movable()){
            // _data_container = std::move(other._data_container);
            // _size = other.size();
            //} else {
            for(auto& elem : other)
            {
                push_back(std::move_if_noexcept(elem));
            }
            other.resize(0);
            //}
        }

        template <size_type RSIZE, typename Alloc_source,
            std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0>
        basic_vector(
            basic_vector<T, RSIZE, Alloc_pattern>&& other, Alloc_source& alloc)
            : _size(0)
            , _data_container(alloc)
        {
            FIXED_CHECK_FULL(other.size() <= max_size());

            // constexpr if (Alloc_pattern::allocation_movable()){
            // _data_container = std::move(other._data_container);
            // _size = other.size();
            //} else {
            for(auto& elem : other)
            {
                push_back(std::move_if_noexcept(elem));
            }
            other.clear();
            //}
        }

        basic_vector(std::initializer_list<T> list)
            : basic_vector()
        {
            uninitialized_assign(list.begin(), list.end());
        }

        template <typename Alloc_source = empty_source,
            std::enable_if_t<is_allocator_source<Alloc_source>::value, int> = 0>
        basic_vector(
            std::initializer_list<T> list, Alloc_source& alloc = empty_source())
            : basic_vector(alloc)
        {
            uninitialized_assign(list.begin(), list.end());
        }

        basic_vector& operator=(const basic_vector& rval)
        {
            if(this != &rval)
            {
                return operator=<SIZE, Alloc_pattern>(rval);
            }
            return *this;
        }

        basic_vector& operator=(basic_vector&& rval) noexcept
        {
            if(this != &rval)
            {
                return operator=<SIZE, Alloc_pattern>(std::move(rval));
            }
            return *this;
        }

        template <size_type RSIZE,
            template <typename, size_type> typename RAllocator>
        basic_vector& operator=(
            basic_vector<T, RSIZE, RAllocator>&& rval) noexcept
        {
            auto rbeg = rval.begin();
            auto rend = rval.end();
            auto lbeg = begin();
            auto lend = end();

            while(rbeg != rend && lbeg != lend)
            {
                *lbeg = std::move(*rbeg);
                ++lbeg;
                ++rbeg;
            }

            while(rbeg != rend)
            {
                push_back(std::move(*rbeg));
                ++rbeg;
            }

            while(lbeg != lend)
            {
                (*lbeg).~T();
                ++lbeg;
                --_size;
            }

            rval.clear();
            return *this;
        }

        template <size_type RSIZE,
            template <typename, size_type> typename RAllocator>
        basic_vector& operator=(const basic_vector<T, RSIZE, RAllocator>& rval)
        {
            auto rbeg = rval.begin();
            auto rend = rval.end();
            auto lbeg = begin();
            auto lend = end();

            while(rbeg != rend && lbeg != lend)
            {
                *lbeg = *rbeg;
                ++lbeg;
                ++rbeg;
            }

            while(rbeg != rend)
            {
                push_back(*rbeg);
                ++rbeg;
            }

            while(lbeg != lend)
            {
                (*lbeg).~T();
                ++lbeg;
                --_size;
            }
            return *this;
        }

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
        const_iterator begin() const { return _data_container.begin(); }
        const_iterator cbegin() const { return _data_container.cbegin(); }
        iterator end() { return begin() + _size; }
        const_iterator end() const { return begin() + _size; }
        const_iterator cend() const { return cbegin() + _size; }

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
            std::enable_if_t<is_iterator<InputIt>::value, int> = 0>
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
            std::enable_if_t<is_iterator<InputIt>::value, int> = 0>
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
            std::enable_if_t<is_iterator<InputIt>::value, int> = 0>
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