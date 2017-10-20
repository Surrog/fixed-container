# Fixed-container
[![Build Status](https://travis-ci.org/Surrog/fixed-container.svg?branch=master)](https://travis-ci.org/Surrog/fixed-container)
[![Build status](https://ci.appveyor.com/api/projects/status/mieyx1cj33ckdgi0?svg=true)](https://ci.appveyor.com/project/Surrog/fixed-container)

C++ library dedicate to provide data container that work with a fixed amount of memory.  
Emphasis of this implementation are:
- No dynamic expansion in favor of simplicity and compactness.
- Allowing to customize memory sources easily.
- Providing tailored container depending the amount of elements expected.
- Most of iterators are Random-access iterators   

Containers interface mirror the standard library at the C++17 update, but are implemented with different allocator design (allowing customized memory sources).  
The container implementation does not allow dynamic expansion, it assume that you can put an upper bound on the number of element you need to work with.   
Patchs are welcome, templated tests can be found in the test/ directory.

## Usage
- Clone the project in your source file
- Add the path of the include directory to your project file
- All the major classes are now available to be include as "fixed/<class name>.hpp"

## Configuration

## Current container finished and released:
- [vector](https://github.com/Surrog/fixed-container/blob/master/docs/vector.md)
- listed_vector

## Current allocator pattern:
Presentation of the pattern [here](https://github.com/Surrog/fixed-container/blob/master/docs/allocation_pattern.md)
- aligned_stack_allocator
- aligned_heap_allocator
- constexpr_stack_allocator

## Current work on:
- string
- unordered_map
