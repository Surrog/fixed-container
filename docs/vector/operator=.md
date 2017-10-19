# fixed::vector::Operator=
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

## Parameters
- other : another container to use as data source
- ilist : initialize list to use as data source

## Return value
`*this`

## Complexity
(1) Linear in the size of `*this` and `other`   
(2) Linear in the size of `*this` and `other`   
(3) Linear in the size of `*this` and `ilist`   

