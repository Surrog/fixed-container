# fixed::vector constructors

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

## Parameters

- source : instance of a container that satisfy Allocator_source contract
- count : the size of the container
- value : the value to initialize elements of the container with
- first, last : the range to copy elements from, must satisfy the InputIterator contract 
- other : another instance of vector to used a source to initialize the lements of the container with
- init : initializer list to initialize the elements of the container with

## Complexity

(1) Constant
(2-3) Linear in count
(4) Linear in distance between first and last
(5) Linear in size of other
(6) Constant if the allocator_pattern provide a move constructor, else linear
(7) Linear in size of init
