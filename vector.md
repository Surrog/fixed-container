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
template <typename T, _impl::container_size_type SIZE,
    template <typename, _impl::container_size_type> typename Alloc_pattern
    = _impl::aligned_stack_allocator>
class vector;
```   
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

### Member function

#### Constructor

```
vector() noexcept(Alloc_pattern()); //(1)
template <typename Alloc_source> vector(Alloc_source& source) noexcept(Alloc_pattern(Alloc_source&));
```
```
vector(size_type count); //(2)
`template <typename Alloc_source> vector(size_type count, Alloc_source&);
```
```
vector(size_type count, const T& value); //(3)
`template <typename Alloc_source> vector(size_type count, const T& value, Alloc_source&)
```
```
template <typename InputIt> vector(InputIt first, InputIt last) //(4)
template <typename InputIt, typename Alloc_source> vector(InputIt first, InputIt last, Alloc_source& source)
```
```
vector(const vector& other) //(5)
template <typename Alloc_source> vector(const vector& other, Alloc_source& source)
template <size_type RSIZE, RAlloc_source> vector(const vector<T, RSIZE, RAlloc_source>& other)
template <size_type RSIZE, typename RAlloc_source, typename Alloc_source> vector(const vector<T, RSIZE, RAlloc_source>& other, Alloc_source& source)
```
```
vector(vector&& other) noexcept(*see description*) //(6)
template <typename Alloc_source> vector(vector&& other, Alloc_source& source) noexcept(Alloc_pattern(Alloc_source&))
template <size_type RSIZE, RAlloc_source> vector(vector<T, RSIZE, RAlloc_source>&& other)
template <size_type RSIZE, typename RAlloc_source, typename Alloc_source> vector(vector<T, RSIZE, RAlloc_source>&& other, Alloc_source& source)
```
```
vector(std::initializer_list<T> init) //(7)
template <typename Allocator_source> vector(std::initializer_list<T> init, Allocator_source& source);
```

(1) Default constructors : construct an empty container. Should you pass a Allocation source, it will construct the Allocation pattern with it.   
(2) Constructs the container with count default-inserted instances of T. No copies are made.   
(3) Constructs the container with count copies of elements with value value.   
(4) Constructs the container with the contents of the range [first, last).   
(5) Copy constructor. Constructs a copy of the container `other` passed in parameters.   
(6) Move constructor. Constructs the container with the contents of other using move semantics.   
Should SIZE and Allocator_pattern are the same, and that Allocator_pattern offer a move constructor, it will use it instead of moving individually each elements. `other` is guaranteed to be empty at the end of move constructor.   
Noexcept is guarantee if Alloc_source is nothrow default constructible AND either Alloc_source is nothrow move assignable OR moving each element from the container is guarrantee to be noexcept.   
(7) Constructs the container with the contents of the initializer list init.

##### Parameters

- source : instance of a container that satisfy Allocator_source contract
- count : the size of the container
- value : the value to initialize elements of the container with
- first, last : the range to copy elements from, must satisfy the InputIterator contract 
- other : another instance of vector to used a source to initialize the lements of the container with
- init : initializer list to initialize the elements of the container with

##### Complexity

(1) Constant
(2-3) Linear in count
(4) Linear in distance between first and last
(5) Linear in size of other
(6) Constant if the allocator_pattern provide a move constructor, else linear
(7) Linear in size of init

#### Destructor

`~vector()`

##### Complexity
Linear in the size of the container.

#### Operator=
```
vector& operator=(const vector& other); //(1)
template <size_type RSIZE, typename Allocator_source> vector& operator=(const vector<RSIZE, Allocator_source>& other);
```
```
vector& operator=(vector&& other) noexcept(*see description*); //(2)
template <size_type RSIZE, typename RAllocator_source> vector& operator=(const vector<RSIZE, RAllocator_source>&& other);
```
```
vector& operator=(std::initializer_list<T> ilist); //(3)
```
(1) Copy assignment operator. Replaces the contents with a copy of the contents of other.
(2) Move assignment operator. Replaces the contents with those of `other` using move semantics (i.e. the data in `other` is moved from other into this container). `other` is in a valid state and empty afterwards.
(3) Replaces the contents with those identified by initializer list ilist.

##### Parameters
- other : another container to use as data source
- ilist : initialize list to use as data source

##### Return value
`*this`

##### Complexity
(1) Linear in the size of `*this` and `other`   
(2) Linear in the size of `*this` and `other`   
(3) Linear in the size of `*this` and `ilist`   

