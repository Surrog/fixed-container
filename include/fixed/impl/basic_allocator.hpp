#ifndef BASIC_HEAPLESS_ALLOCATOR_HPP
#define BASIC_HEAPLESS_ALLOCATOR_HPP

#include <cstddef>
#include <type_traits>
#include <memory>

namespace fixed
{
	namespace _impl
	{
		//Allocate your container on the stack
		template <typename T, std::size_t SIZE>
		struct basic_stack_allocator
		{
			static_assert(SIZE > 0, "zero sized container not allowed !");

			typedef T value_type;
			typedef T* pointer;
			typedef const T* const_pointer;

			pointer data() { return reinterpret_cast<T*>(_data); }
			const_pointer data() const { return reinterpret_cast<const T*>(_data); }
			bool valid_pointer(const T* ptr) { return ptr >= data() && ptr < data() + max_size(); }
			constexpr std::size_t max_size() const { return SIZE; }
		private:
			typename std::aligned_storage<sizeof(T), alignof(T)>::type _data[SIZE];
		};

		//When your size is too big to being correctly stored on the stack
		template <typename T, std::size_t SIZE>
		struct basic_heap_allocator
		{
			static_assert(SIZE > 0, "zero sized container not allowed !");

			typedef T value_type;
			typedef T* pointer;
			typedef const T* const_pointer;

			pointer data() { return reinterpret_cast<T*>(_data); }
			const_pointer data() const { return reinterpret_cast<const T*>(_data); }
			bool valid_pointer(const T* ptr) { return ptr >= data() && ptr < data() + max_size(); }
			constexpr std::size_t max_size() const { return SIZE; }
		private:
			std::unique_ptr<typename std::aligned_storage<sizeof(T), alignof(T)>::type> _data = std::make_unique<std::aligned_storage<sizeof(T), alignof(T)>::type>();
 		};


		//
		template <typename T, std::size_t SIZE>
		struct basic_block_allocator
		{

		};
	}
}

#endif //!BASIC_HEAPLESS_ALLOCATOR_HPP
