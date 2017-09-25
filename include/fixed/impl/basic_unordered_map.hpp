#ifndef HEAPLESS_BASIC_UNORDERED_MAP_HPP
#define HEAPLESS_BASIC_UNORDERED_MAP_HPP

#include <algorithm>
#include <array>
#include <numeric>
#include <utility>

#include "fixed/impl/basic_allocation_pattern.hpp"
#include "fixed/impl/fixed_def.hpp"
#include "fixed/impl/iterator.hpp"

namespace fixed
{
namespace _impl
{
    template <typename Key, typename T, container_size_type SIZE,
        class Hash = std::hash<Key>, class Pred = std::equal_to<Key>,
        template <typename, container_size_type> typename Alloc_pattern
        = basic_stack_allocator>
    class basic_unordered_map
    {
        // typedefs
    public:
        typedef Key key_type;
        typedef T mapped_type;
        typedef std::pair<const key_type, mapped_type> value_type;
        typedef Hash hasher;
        typedef Pred key_equal;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef container_size_type size_type;
        typedef std::ptrdiff_t difference_type;

        typedef pointer_iterator<T> iterator;
        typedef pointer_iterator<const T> const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        template <typename Types, size_type ALLOC_SIZE>
        using allocator_type = Alloc_pattern<Types, ALLOC_SIZE>;

    private:
        struct node
        {
            value_type* item;
            node* next_node;
        };

        Alloc_pattern<node*, SIZE> _buckets; // real hash map to node
        Alloc_pattern<node, SIZE>
            _bucket_data; // compact index to data, handle memory recycling
        Alloc_pattern<value_type, SIZE> _data; // sparse data holder
        size_type _size
            = 0; // total size of initialized object & index to the next object
        hasher _hash; // hasher to compute index
        key_equal _equal; // compare to Key

    public:
        //! Warning : does nothing, no reallocation possible
        constexpr void reserve(size_type) {}
    };
}
}

#endif //! HEAPLESS_BASIC_UNORDERED_MAP_HPP