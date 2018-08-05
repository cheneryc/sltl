# Shading Language Template Library (SLTL)

SLTL is an embedded domain specific language (eDSL) for writing shaders with C++14

## About

SLTL is a C++ library that is used to write meta-shaders. Invoking a SLTL meta-shader creates a data structure similar to an abstract syntax tree (AST). This AST can then be traversed to generate shader source code in a specified shader language (although only GLSL output is currently supported).

__Advantages:__

* Leverage the C++ type system to discover shader errors and incompatibilities at compile time.
* Write generic shader code and maximise code reuse without resorting to preprocessor macros.
* Avoid learning multiple shader languages for cross-platform development. 
* Run time generation of optimised shader source code.

## Getting Started

To build SLTL open the Visual Studio solution file located at the root of the repository. Visual Studio 2015 is currently the only supported development environment.

### Prerequisites

There are no external dependencies and therefore no prerequisites.

### Documentation

The [Tutorial](https://github.com/cheneryc/sltl/wiki/Tutorial) wiki is currently the only documentation available.

## Versioning

The library is currently at a proof of concept stage. Once it is possible to write a minimally useful vertex and fragment shader an initial version number will be assigned.

## Contributing

This project is primarily a demonstration of the author's coding abilities. There is currently no intention to accept code contributions from other developers although feedback, suggestions for improvements and bug reports are all welcome.

## Licence

SLTL is licensed under the MIT License.

## Copyright

Copyright 2014 - 2018, Christopher Chenery
