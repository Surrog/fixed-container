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
    // Allocate your container on the stack
    template <typename T, container_size_type SIZE>
    struct basic_stack_allocator
    {
    public:
        static_assert(SIZE > 0, "zero sized container not allowed !");

        basic_stack_allocator() = default;
        basic_stack_allocator(const empty_source&)
            : aligned_stack_allocator()
        {
        }

        typedef T value_type;
        typedef T aligned_type;
        typedef allocation_pattern_tag allocation_pattern;
		typedef std::true_type noexcept_iterators;
        typedef pointer_iterator<T> iterator;
        typedef const_pointer_iterator<T> const_iterator;

        aligned_type* data() noexcept { return _data; }

        const aligned_type* data() const noexcept { return _data; }

        iterator begin() noexcept { return iterator(_data); }

        iterator end() noexcept { return iterator(_data + SIZE); }

        const_iterator begin() const noexcept { return const_iterator(_data); }

        const_iterator end() const noexcept
        {
            return const_iterator(_data + SIZE);
        }

        const_iterator cbegin() const noexcept { return const_iterator(_data); }

        const_iterator cend() const noexcept
        {
            return const_iterator(_data + SIZE);
        }

        T& operator[](container_size_type i)
        {
            FIXED_CHECK_INBOUND(i < SIZE);
            return reinterpret_cast<T&>(_data[i]);
        }
        const T& operator[](container_size_type i) const
        {
            FIXED_CHECK_INBOUND(i < SIZE);
            return reinterpret_cast<const T&>(_data[i]);
        }

        bool valid_pointer(const T* ptr) const noexcept
        {
            return ptr >= data() && ptr < data() + max_size();
        }
        constexpr container_size_type max_size() const noexcept { return SIZE; }

    private:
        aligned_type _data[SIZE];
    };

    // When your size is too big to being correctly stored on the stack
    template <typename T, container_size_type SIZE>
    struct basic_heap_allocator
    {
    public:
        static_assert(SIZE > 0, "zero sized container not allowed !");

		basic_heap_allocator() noexcept = default;
		basic_heap_allocator(basic_heap_allocator&&) noexcept = default;
		basic_heap_allocator& operator=(basic_heap_allocator&&) noexcept
            = default;

		basic_heap_allocator(const empty_source&) noexcept
            : aligned_heap_allocator()
        {
        }

        typedef allocation_pattern_tag allocation_pattern;
		typedef std::false_type noexcept_iterators;

        typedef T value_type;
        typedef T aligned_type;
        typedef pointer_iterator<T> iterator;
        typedef const_pointer_iterator<T> const_iterator;

        aligned_type* data()
        {
            if(!_data)
                _data = std::make_unique<aligned_type[]>(SIZE);
            return _data.get();
        }

        const aligned_type* data() const { return _data.get(); }

        iterator begin()
        {
            if(!_data)
                _data = std::make_unique<aligned_type[]>(SIZE);
            return iterator(_data.get());
        }

        iterator end()
        {
            if(!_data)
                _data = std::make_unique<aligned_type[]>(SIZE);
            return iterator(_data.get() + SIZE);
        }

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
			FIXED_CHECK_INBOUND(i < SIZE);
			if (!_data)
				_data = std::make_unique<aligned_type[]>(SIZE);
			return reinterpret_cast<T&>(data()[i]);
        }
        const T& operator[](container_size_type i) const
        {
            FIXED_CHECK_INBOUND(i < SIZE);
            return reinterpret_cast<const T&>(data()[i]);
        }

        bool valid_pointer(const T* ptr) const
        {
            return ptr >= data() && ptr < data() + max_size();
        }

        constexpr container_size_type max_size() const { return SIZE; }

    private:
        std::unique_ptr<aligned_type[]> _data;
    };
}
}

#endif //! FIXED_BASIC_ALLOCATION_PATTERN_HPP
