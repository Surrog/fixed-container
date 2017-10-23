# Allocation pattern

An Allocation pattern is an object designed to handle memory allocation: it's lifetime and how it is allocated.

## Allocation pattern categories

Pattern are classified into 2 categories depending on the functionnality they implement:   

```
|-----------------|    |------------|   
| Allocation base |<---| Contiguous |   
|-----------------|    |------------|   
```


## Member types
|  Member type | Definition |
| ---  | --- |
| `value_type` | `T` | 
| `aligned_type` | `<implementation defined>` | 
| `size_type` |  Unsigned integer type (usually std::size_t) |
| `difference_type` |  Signed integer type (usually std::ptrdiff_t) |
| `reference` | `T&` |
| `const_reference` | `const T&` |
| `pointer` | `T*` |
| `const_pointer` | `const T*` |
| `iterator` | RandomAccessIterator |
| `const_iterator` | Constant random access iterator |

##  Member functions

### Allocation base interface

Expected signature:
```
template <typename T, fixed::_impl::size_t SIZE>
struct allocator_pattern
```

Expected method:
```
iterator begin();
iterator end();
const_iterator begin() const;
const_iterator end() const;
const_iterator cbegin() const;
const_iterator cend() const;
bool valid_pointer(const T* ptr) const noexcept;
container_size_type max_size() const noexcept;
```

### Allocation contiguous interface

A contiguous contract implement the Allocation base contract and also respect the following one.

Expected method:
```
aligned_type* data();
const aligned_type* data() const;
```

## Classes

| Current released implementation |
| ------------------------------- |
| aligned_stack_allocator         |
| aligned_heap_allocator          |
| constexpr_stack_allocator       |