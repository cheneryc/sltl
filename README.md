# Shading Language Template Library (SLTL)

SLTL is an embedded domain specific language (eDSL) for writing shaders in C++14

## Motivation

__Two main uses:__

1. A means of developing cross platform shaders that takes advantage of the strict type safety provided by C++. Shader code can easily be shared, adapted and reused through standard C++ language features. Here SLTL is used as the core of a shader compiler that is integrated into the build process of a 3D engine. Only the output of the SLTL library (typically the source code for a shader program written in GLSL) is included with an application.

For some platforms the output of the SLTL library must be compiled with another external tool (e.g. HLSL and D3DCompileFromFile) before it is ready for distribution.

2. As part of a dynamic shader creation system. With this approach shader source code is generated and compiled at runtime.

__Advantages:__

* Easily maximise code reuse when generating many shader permutations that are all related and only differ in small details without resorting to preprocessor macros.
* Take advantage of C++ strict type system to discover shader incompatibilities and errors at compile time.
* Avoid having to learn and use different shader languages during cross platform development. 

## Introduction

Any shader written with SLTL is probably best considered as a meta-shader. The structure of the shader isn't determined until the C++ code is executed at which point an abstract syntax tree (AST) of the actual shader(s) is produced. Any C++ flow control structures (if-statements, for-loops etc.) in the meta-shader represent potentially many different actual shaders. Finally, to convert the AST into source code an output action corresponding to a specific shader language (currently only GLSL is supported) must be applied.

## Getting Started

To build SLTL open the Visual Studio solution file located at the root of the repository. Visual Studio 2015 is currently the only supported develpment environment.

### Prerequisites

There are no external dependencies and therefore no prerequisites.

### Documentation

TODO: link to wiki

## Versioning

The library is currently at a proof of concept stage. Once it is possible to write a minimally useful vertex and fragment shader an initial version number will be assigned.

## Contributing

This project is primarily a demonstration of the author's coding abilities. There is currently no intention to accept code contributions from other developers although feedback, suggestions for improvements and bug reports are all welcome.

## Licence

TBD. Expected to be open sourced using either boost or MIT licences.

## Copyright

Copyright 2014 - 2018, Christopher Chenery
