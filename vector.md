# Vector

fixed::vector is a sequence container that encapsulates dynamic number of element in a fixed size array
The elements are stored contiguously, which means that elements can be accessed not only through iterators, but also using offsets to regular pointers to elements. This means that a pointer to an element of a vector may be passed to any function that expects a pointer to an element of an array.
Subsequently fixed::vector require a allocation pattern that satisfy the continous storage contract.

The complexity (efficiency) of common operations on vectors is as follows:
- Random access - constant O(1)
- Insertion or removal of elements at the end - amortized constant O(1)
- Insertion or removal of elements - linear in the distance to the end of the vector O(n) 

## Interface

`
template <typename T, _impl::container_size_type SIZE,
    template <typename, _impl::container_size_type> typename Alloc_pattern
    = _impl::aligned_stack_allocator>
class vector;
`