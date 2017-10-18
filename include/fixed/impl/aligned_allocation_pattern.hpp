#ifndef FIXED_ALIGNED_ALLOCATION_PATTERN_HPP
#define FIXED_ALIGNED_ALLOCATION_PATTERN_HPP

#include <memory>

#include "fixed/impl/basic_allocation_sources.hpp"
#include "fixed/impl/basic_listed_vector_iterator.hpp"
#include "fixed/impl/basic_pointer_iterator.hpp"
#include "fixed/impl/fixed_def.hpp"

namespace fixed
{
namespace _impl
{
    // Allocate your container on the stack
    template <typename T, container_size_type SIZE>
    struct aligned_stack_allocator
    {
        typedef typename std::aligned_storage<sizeof(T), alignof(T)>::type
            inner_type;

    public:
        static_assert(SIZE > 0, "zero sized container not allowed !");

        aligned_stack_allocator() noexcept
            : _data()
        {
        }

        aligned_stack_allocator(const empty_source&) noexcept
            : aligned_stack_allocator()
        {
        }

        typedef T value_type;
        typedef typename std::conditional<
            sizeof(typename std::aligned_storage<sizeof(T), alignof(T)>::type)
                == sizeof(T),
            T, typename std::aligned_storage<sizeof(T), alignof(T)>::type>::type
            aligned_type;
		typedef container_size_type size_type;
        typedef allocation_pattern_tag allocation_pattern;
        typedef std::true_type noexcept_iterators;
        typedef pointer_iterator<T, aligned_type> iterator;
        typedef const_pointer_iterator<T, aligned_type> const_iterator;

        aligned_type* data() noexcept
        {
            return reinterpret_cast<aligned_type*>(_data);
        }
        const aligned_type* data() const noexcept
        {
            return reinterpret_cast<const aligned_type*>(_data);
        }

        iterator begin() noexcept { return iterator(data()); }
        iterator end() noexcept { return iterator(data() + SIZE); }

        const_iterator begin() const noexcept { return cbegin(); }
        const_iterator end() const noexcept { return cend(); }
        const_iterator cbegin() const noexcept
        {
            return const_iterator(data());
        }
        const_iterator cend() const noexcept
        {
            return const_iterator(data() + SIZE);
        }

        bool valid_pointer(const T* ptr) const noexcept
        {
            return ptr >= data() && ptr < data() + max_size();
        }
        constexpr size_type max_size() const noexcept { return SIZE; }

    private:
        inner_type _data[SIZE];
    };

    // When your size is too big to being correctly stored on the stack
    template <typename T, container_size_type SIZE>
    struct aligned_heap_allocator
    {
        typedef typename std::aligned_storage<sizeof(T), alignof(T)>::type
            inner_type;

    public:
        static_assert(SIZE > 0, "zero sized container not allowed !");

        aligned_heap_allocator() noexcept = default;
        aligned_heap_allocator(aligned_heap_allocator&&) noexcept = default;
        aligned_heap_allocator& operator=(aligned_heap_allocator&&) noexcept
            = default;

        aligned_heap_allocator(const empty_source&) noexcept
            : aligned_heap_allocator()
        {
        }

        typedef allocation_pattern_tag allocation_pattern;
        typedef std::false_type noexcept_iterators;

        typedef T value_type;
        typedef typename std::conditional<
            sizeof(typename std::aligned_storage<sizeof(T), alignof(T)>::type)
                == sizeof(T),
            T, typename std::aligned_storage<sizeof(T), alignof(T)>::type>::type
            aligned_type;
		typedef container_size_type size_type;
        typedef pointer_iterator<T, aligned_type> iterator;
        typedef const_pointer_iterator<T, aligned_type> const_iterator;

        aligned_type* data()
        {
            if(!_data)
                resize(SIZE);
            return reinterpret_cast<aligned_type*>(_data.get());
        }

        const aligned_type* data() const
        {
            return reinterpret_cast<const aligned_type*>(_data.get());
        }

        iterator begin() { return iterator(data()); }

        iterator end() { return iterator(data() + SIZE); }

        const_iterator cbegin() const { return const_iterator(data()); }
        const_iterator cend() const { return const_iterator(data() + SIZE); }
        const_iterator begin() const { return cbegin(); }
        const_iterator end() const { return cend(); }

        bool valid_pointer(const T* ptr) const noexcept
        {
            return ptr >= data() && ptr < data() + max_size();
        }

        void resize(size_type to_size)
        {
            to_size = std::max(SIZE, to_size);
            _data = std::make_unique<inner_type[]>(to_size);
        }

        constexpr size_type max_size() const { return SIZE; }

    private:
        std::unique_ptr<inner_type[]> _data;
    };
}
}

#endif //! FIXED_ALIGNED_ALLOCATION_PATTERN_HPP
