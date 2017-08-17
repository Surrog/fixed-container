#ifndef BASIC_HEAPLESS_ALLOCATOR_HPP
#define BASIC_HEAPLESS_ALLOCATOR_HPP

#include <type_traits>
#include <memory>
#include "fixed_def.hpp"

namespace fixed
{
	namespace _impl
	{
		template <typename T, class = void>
		struct is_allocator_source : public std::bool_constant<false>
		{};

		template <typename T>
		struct is_allocator_source<T, std::void_t<typename T::allocator_source>> : public std::bool_constant<true>
		{};

		struct allocator_source_tag
		{};

		struct empty_source
		{
			typedef allocator_source_tag allocator_source;
		};

		struct allocation_pattern_tag
		{};

		template <typename T, class = void>
		struct is_allocation_pattern : public std::bool_constant<false>
		{};

		template <typename T>
		struct is_allocation_pattern<T, std::void_t<typename T::allocation_pattern>> : public std::bool_constant<true>
		{};

		//Allocate your container on the stack
		template <typename T, size_type SIZE>
		struct basic_stack_allocator
		{
			static_assert(SIZE > 0, "zero sized container not allowed !");

			basic_stack_allocator() = default;
			basic_stack_allocator(const basic_stack_allocator&) = default;
			basic_stack_allocator& operator=(const basic_stack_allocator&) = default;
			basic_stack_allocator(basic_stack_allocator&&) noexcept= default;
			basic_stack_allocator& operator=(basic_stack_allocator&&) noexcept= default;

			basic_stack_allocator(const empty_source&): basic_stack_allocator() {}

			typedef T value_type;
			typedef T* pointer;
			typedef T& reference;
			typedef const T* const_pointer;
			typedef const T& const_reference;
			typedef allocation_pattern_tag allocation_pattern;

			pointer data() { return reinterpret_cast<T*>(_data); }
			const_pointer data() const { return reinterpret_cast<const T*>(_data); }
			reference operator[](size_type i) { assert(i < SIZE); return data()[i]; }
			const_reference operator[](size_type i) const { assert(i < SIZE); return data()[i]; }

			bool valid_pointer(const T* ptr) { return ptr >= data() && ptr < data() + max_size(); }
			constexpr size_type max_size() const { return SIZE; }
		private:
			typename std::aligned_storage<sizeof(T), alignof(T)>::type _data[SIZE];
		};

		//When your size is too big to being correctly stored on the stack
		template <typename T, size_type SIZE>
		struct basic_heap_allocator
		{
			static_assert(SIZE > 0, "zero sized container not allowed !");

			basic_heap_allocator() = default;
			basic_heap_allocator(basic_heap_allocator&&) = default;
			basic_heap_allocator& operator=(basic_heap_allocator&&) = default;

			basic_heap_allocator(const empty_source& ) : basic_heap_allocator() {}

			typedef T value_type;
			typedef T* pointer;
			typedef T& reference;
			typedef const T* const_pointer;
			typedef const T& const_reference;
			typedef allocation_pattern_tag allocation_pattern;

			pointer data() { return reinterpret_cast<T*>(_data); }
			const_pointer data() const { return reinterpret_cast<const T*>(_data); }
			reference operator[](size_type i) { assert(i < SIZE); return data()[i]; }
			const_reference operator[](size_type i) const { assert(i < SIZE); return data()[i]; }

			bool valid_pointer(const T* ptr) { return ptr >= data() && ptr < data() + max_size(); }
			constexpr size_type max_size() const { return SIZE; }
		private:
			std::unique_ptr<typename std::aligned_storage<sizeof(T), alignof(T)>::type[]> _data = std::make_unique<typename std::aligned_storage<sizeof(T), alignof(T)>::type[]>(SIZE);
 		};
	}
}

#endif //!BASIC_HEAPLESS_ALLOCATOR_HPP
