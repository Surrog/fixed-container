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
- Accessing concurrently the container may create a data race situation.

## Interface

```
template <typename T, fixed::_impl:size_t SIZE,
    template <typename, fixed::_impl::size_t> typename Alloc_pattern
    = fixed::_impl::aligned_stack_allocator>
class vector;
```   
Guarrantee the container support, atleast, SIZE of elements T in continous memory.

### Member types
|  Member type | Definition |
| ---  | --- |
| `value_type` | `Alloc_pattern::value_type` | 
| `aligned_type` | `Alloc_pattern::aligned_type` | 
| `template <typename T, size_t S> allocator_type` | `Alloc_pattern` |
| `size_type` |  `Alloc_pattern::size_type` |
| `difference_type` |  `Alloc_pattern::difference_type` |
| `reference` | `Alloc_pattern::reference` |
| `const_reference` | `Alloc_pattern::const_reference` |
| `pointer` | `Alloc_pattern::pointer` |
| `const_pointer` | `Alloc_pattern::const_pointer` |
| `iterator` | RandomAccessIterator |
| `const_iterator` | Constant random access iterator |
| `reverse_iterator` | `std::reverse_iterator<iterator>` |
| `const_reverse_iterator` | `std::reverse_iterator<const_iterator>` |

### Member function

| *Member function* | *Description* | *Changed from standard interface* |
| --- | --- | --- |
| [(constructor)](https://github.com/Surrog/fixed-container/blob/master/docs/vector/constructor.md) | constructs the vector | # |
| [(destructor)](https://github.com/Surrog/fixed-container/blob/master/docs/vector/destructor.md) | destructs the vector | . |
| [operator=](https://github.com/Surrog/fixed-container/blob/master/docs/vector/operator=.md) | assigns values to the container | # |
| assign | assigns values to the container | . |
| *Element access* | *Description* | *Changed from standard interface* |
| --- | --- | --- |
| at | access specified element with bounds checking | . |
| operator[] | access specified element | . |
| front | access the first element | . |
| back | access the last element | . |
| data | direct access to the underlying array | . |
| *Iterators* | *Description* | *Changed from standard interface* |
| --- | --- | --- |
| begin cbegin | returns an iterator to the beginning | . |
| end cend | returns an iterator to the end | . |
| rbegin crbegin | returns a reverse iterator to the beginning | . |
| rend crend | returns a reverse iterator to the end | . |
| *Capacity* | *Description* | *Changed from standard interface* |
| --- | --- | --- |
| empty | checks whether the container is empty | . |
| size | returns the number of elements | . |
| max_size | returns the maximum possible number of elements | . |
| reserve | reserves storage | . |
| capacity | returns the number of elements that can be held in currently allocated storage | . |
| shrink_to_fit | reduces memory usage by freeing unused memory | . |
| *Modifiers* | *Description* | *Changed from standard interface* |
| --- | --- | --- | 
| clear | clears the contents | . |
| insert | inserts elements | . |
| emplace | constructs element in-place | . |
| erase | erases elements | . |
| push_back | adds an element to the end | . |
| emplace_back | constructs an element in-place at the end | . |
| pop_back | removes the last element | . |
| resize | changes the number of elements stored | . |
| swap | swaps the contents | . |
