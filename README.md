# Fixed-container
[![Build Status](https://travis-ci.org/Surrog/fixed-container.svg?branch=master)](https://travis-ci.org/Surrog/fixed-container)
[![Build status](https://ci.appveyor.com/api/projects/status/mieyx1cj33ckdgi0?svg=true)](https://ci.appveyor.com/project/Surrog/fixed-container)

C++ library dedicate to provide data container that work with a fixed amount of memory.  
Emphasis of this implementation are:
- No dynamic expansion but emphasis on simplicity and compactness.
- Allowing to customize memory sources easily.
- Providing tailored container depending the amount of elements expected.

Containers interface mirror the standard library at the C++17 update, but are implemented with different allocator design (allowing customized memory sources).  
The container implementation does not allow dynamic expansion, it assume that you can put an upper bound on the number of element you need to work with.

## Current container finished and released:
- vector
- list

## Current work on:
- string
- unordered_map
