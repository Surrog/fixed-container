# Vector

fixed::vector is a sequence container that encapsulates dynamic number of element in a fixed size array.
The elements are stored contiguously, which means that elements can be accessed not only through iterators, but also using offsets to regular pointers to elements. This means that a pointer to an element of a vector may be passed to any function that expects a pointer to an element of an array.   
Subsequently fixed::vector require a allocation pattern that satisfy the continous storage contract.   
Current interface follow the C++17 version of std vector, but minor changes have been done to handle properly the fixed allocator pattern. 

The complexity (efficiency) of common operations on vectors is as follows:
- Random access - constant O(1)
- Insertion or removal of elements at the end - amortized constant O(1)
- Insertion or removal of elements - linear in the distance to the end of the vector O(n) 

Iterator invalidation:
- Iterators of a non empty container will point to a valid memory, and a full constructed element T
- Iterators before inserting or removing elements will still be valid.

Thread safety:
- Accessing concurrently the container will create a data race situation.

## Interface

`
template <typename T, _impl::container_size_type SIZE,
    template <typename, _impl::container_size_type> typename Alloc_pattern
    = _impl::aligned_stack_allocator>
class vector;
`
Guarrantee the container support, atleast, SIZE of elements T in continous memory.

### Member types
|  Member type | Definition |
| ---  | --- |
| `value_type` | `T` | 
| `template <typename T, container_size_type S> allocator_type` | `Alloc_pattern` |
| `size_type` |  Unsigned integer type (usually std::size_t) |
| `difference_type` |  Signed integer type (usually std::ptrdiff_t) |
| `reference` | `T&` |
| `const_reference` | `const T&` |
| `pointer` | `T*` |
| `const_pointer` | `const T*` |
| `iterator` | RandomAccessIterator |
| `const_iterator` | Constant random access iterator |
| `reverse_iterator` | `std::reverse_iterator<iterator>` |
| `const_reverse_iterator` | `std::reverse_iterator<const_iterator>` |
