# Allocation pattern

An Allocation pattern is a container design to handle memory allocation, it's lifetime and how it is allocated.

```
template <typename T, container_size_type SIZE>
struct allocator_pattern
```

## Member types
|  Member type | Definition |
| ---  | --- |
| `value_type` | `T` | 
| `size_type` |  Unsigned integer type (usually std::size_t) |
| `difference_type` |  Signed integer type (usually std::ptrdiff_t) |
| `reference` | `T&` |
| `const_reference` | `const T&` |
| `pointer` | `T*` |
| `const_pointer` | `const T*` |
| `iterator` | RandomAccessIterator |
| `const_iterator` | Constant random access iterator |

##  Member functions

```
aligned_type* data();
const aligned_type* data() const;
iterator begin();
iterator end();
const_iterator begin() const;
const_iterator end() const;
const_iterator cbegin() const;
const_iterator cend() const;
bool valid_pointer(const T* ptr) const noexcept;
container_size_type max_size() const noexcept;
```

