#ifndef FIXED_BASIC_ALLOCATION_PATTERN_HPP
#define FIXED_BASIC_ALLOCATION_PATTERN_HPP

#include "basic_allocation_sources.hpp"
#include "basic_pointer_iterator.hpp"
#include "fixed_def.hpp"
#include "fixed_type_traits.hpp"
#include <cassert>
#include <memory>
#include <type_traits>

namespace fixed
{
namespace _impl
{
    struct allocation_pattern_tag
    {
    };

    template <typename T, class = void>
    struct is_allocation_pattern : public std::false_type
    {
    };

    template <typename T>
    struct is_allocation_pattern<T,
        fixed::astd::void_t<typename T::allocation_pattern>>
        : public std::true_type
    {
    };

    template <typename T, class = void>
    struct is_allocation_movable : public std::false_type
    {
    };

    template <typename T>
    struct is_allocation_movable<T,
        fixed::astd::void_t<typename T::allocation_movable>>
        : public T::allocation_movable
    {
    };

    template <typename T, class = void>
    struct is_allocation_contiguous : public std::false_type
    {
    };

    template <typename T>
    struct is_allocation_contiguous<T,
        fixed::astd::void_t<typename T::allocation_linear>>
        : public T::allocation_contiguous
    {
    };

    // Allocate your container on the stack
    template <typename T, container_size_type SIZE> struct aligned_stack_allocator
    {
    public:
        static_assert(SIZE > 0, "zero sized container not allowed !");

        aligned_stack_allocator() = default;
        aligned_stack_allocator(const empty_source&)
            : aligned_stack_allocator()
        {
        }

        typedef T value_type;
        typedef typename std::aligned_storage<sizeof(T), alignof(T)>::type
            aligned_type;
        typedef allocation_pattern_tag allocation_pattern;
        typedef std::false_type allocation_movable;
        typedef std::true_type allocation_contiguous;
        typedef pointer_iterator<T> iterator;
        typedef const_pointer_iterator<T> const_iterator;

        aligned_type* data() { return _data; }

        const aligned_type* data() const { return _data; }

        iterator begin() { return iterator(_data); }

        iterator end() { return iterator(_data + SIZE); }

        const_iterator begin() const { return const_iterator(_data); }

        const_iterator end() const { return const_iterator(_data + SIZE); }

        const_iterator cbegin() const { return const_iterator(_data); }

        const_iterator cend() const { return const_iterator(_data + SIZE); }

        T& operator[](container_size_type i)
        {
            assert(i < SIZE);
            return reinterpret_cast<T&>(_data[i]);
        }
        const T& operator[](container_size_type i) const
        {
            assert(i < SIZE);
            return reinterpret_cast<const T&>(_data[i]);
        }

        bool valid_pointer(const T* ptr)
        {
            return ptr >= data() && ptr < data() + max_size();
        }
        constexpr container_size_type max_size() const { return SIZE; }

    private:
        aligned_type _data[SIZE];
    };

    // When your size is too big to being correctly stored on the stack
    template <typename T, container_size_type SIZE> struct aligned_heap_allocator
    {
    public:
        static_assert(SIZE > 0, "zero sized container not allowed !");

        aligned_heap_allocator() = default;
        aligned_heap_allocator(aligned_heap_allocator&&) noexcept = default;
        aligned_heap_allocator& operator=(aligned_heap_allocator&&) noexcept
            = default;

        aligned_heap_allocator(const empty_source&)
            : aligned_heap_allocator()
        {
        }

        typedef allocation_pattern_tag allocation_pattern;
        typedef std::true_type allocation_movable;
        typedef std::true_type allocation_contiguous;

        typedef T value_type;
        typedef typename std::aligned_storage<sizeof(T), alignof(T)>::type
            aligned_type;
        typedef pointer_iterator<T> iterator;
        typedef const_pointer_iterator<T> const_iterator;

        aligned_type* data() { return _data.get(); }

        const aligned_type* data() const { return _data.get(); }

        iterator begin() { return iterator(_data.get()); }

        iterator end() { return iterator(_data.get() + SIZE); }

        const_iterator begin() const { return const_iterator(_data.get()); }

        const_iterator end() const
        {
            return const_iterator(_data.get() + SIZE);
        }

        const_iterator cbegin() const { return const_iterator(_data.get()); }

        const_iterator cend() const
        {
            return const_iterator(_data.get() + SIZE);
        }

        T& operator[](container_size_type i)
        {
            assert(i < SIZE);
            return reinterpret_cast<T&>(data()[i]);
        }
        const T& operator[](container_size_type i) const
        {
            assert(i < SIZE);
            return reinterpret_cast<const T&>(data()[i]);
        }

        bool valid_pointer(const T* ptr)
        {
            return ptr >= data() && ptr < data() + max_size();
        }
        constexpr container_size_type max_size() const { return SIZE; }

    private:
        std::unique_ptr<aligned_type[]> _data
            = std::make_unique<aligned_type[]>(SIZE);
    };
}
}

#endif //! FIXED_BASIC_ALLOCATION_PATTERN_HPP
