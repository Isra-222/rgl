# RGL - Regular Library


<img src="assets/logo.png" alt="Regular Library Logo" width="1000" height="600"/>


__A Minimalist, High-Performance C++ Library__

RGL (Regular Library) is a lightweight C++ library designed to operate independently of the C++ Standard Library. Built from the ground up, it provides essential containers, algorithms, and utilities while maintaining a minimal footprint, predictable performance, and complete transparency.

---

## Philosophy

RGL was created with a clear philosophy: to deliver a library that is not only free and open-source but also __approachable, customizable, and user-friendly__.

We believe developers should have full control over their tools. Unlike the opaque implementations of the Standard Library—where internal mechanics often remain hidden behind layers of abstraction—RGL prioritizes simplicity, readability, and understandability. Every component is designed to be easily inspectable, modifiable, and adaptable to specific project requirements.

---

## Why Choose RGL?

* The C++ Standard Library is powerful, but it comes with trade-offs:

* Over-Abstraction: Many implementations, such as std::vector<bool>, introduce surprising behavior and non-intuitive memory layouts.

* Complex Templates: Codebases often become cluttered with deeply nested, unreadable template metaprogramming.

* Black Box Design: Internal implementation details are frequently hidden, making debugging and customization difficult.

__RGL addresses these issues by offering:__

* Equal Abstraction Level: RGL provides functionality comparable to the Standard Library but without unnecessary complexity.

* Performance Without Sacrifice: Achieves high performance while maintaining full control over memory allocation and algorithm implementation.

* Transparency: All code is written to be readable, debuggable, and easily modifiable.

* Minimal Footprint: Lightweight and dependency-free, ideal for resource-constrained environments and embedded systems.

---

## Getting Started

__Building the Library__

Clone the repository and build RGL using CMake:

```bash

git clone https://github.com/Isra-222/rgl.git
cd rgl

cmake -B build
cmake --build build
```

The build process generates a static library (librgl.a) and the necessary header files.

---

## Usage Example

Here's a minimal example to get you started with RGL:

```cpp

#include "rgl/io/iostream.h"

int main(void) {
    rgl::out << "What's up, World!" << rgl::endl;
    return 0;
}
```

### Compiling Your Application

Link against the RGL library and include the appropriate header paths:
bash

```bash
clang++ yourCode.cpp build/librgl.a -I. -o hello_world
./hello_world
```

## Feature Overview

RGL provides a comprehensive set of modules designed for modern C++ development:

__Containers__

* rgl::vector: Dynamic array with custom allocator support

* rgl::array: Fixed-size array

* rgl::smallVector: Optimized small-size vector with stack allocation

* rgl::map: Associative container

__Memory Management__

* Linear Allocator: Sequential allocation with fast reset

* Pool Allocator: Fixed-size object pooling

* Stack Allocator: LIFO allocation strategy

__Algorithms__

* Search: find, binary_search

* Sorting: sort with customizable comparison

* Sequence Operations: rotate, reverse, shuffle

* Utility: min, max, swap

__Concurrency__

* rgl::thread: Platform-agnostic thread management

* rgl::atomic: Atomic operations

* rgl::mutex: Mutex synchronization

* rgl::condition_variable: Thread coordination

__Mathematics__

* Trigonometric Functions: sin, cos, tan, etc.

* Random Generation: Integer and floating-point random numbers

* Statistical Utilities: Min, max, and average operations

__Input/Output__

* rgl::out: Standard output stream

* rgl::ofstream: File output stream

__Time Management__

* High-Resolution Timing: rgl::anemo chrono functionality

* Duration Conversions: Microsecond, millisecond, and second precision

---

### Version

Current Version: 0.4-alpha

_Note: This is an alpha release. Features are subject to change, and the API may evolve based on community feedback and testing._

---

## Roadmap

* Full algorithm coverage (sorting, searching, partitioning)

* Smart pointers (unique_ptr, shared_ptr)

* String manipulation utilities

* File system operations

* Networking support

* Benchmarking suite

---

## Contributing

Contributions are welcome! Whether you're reporting bugs, suggesting improvements, or submitting pull requests, please ensure:

* Code adheres to the existing style and conventions.

* New features include appropriate tests.

* Documentation is updated accordingly.

For major changes, please open an issue first to discuss what you would like to change.

---

## License

__LGPLv3 (GNU Lesser General Public License, Version 3)__

RGL is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the LGPLv3 for more details.

__Key Terms:__

* You may use RGL in proprietary projects.

* Modifications to RGL must be released under LGPLv3.

* You must provide attribution and include the license notice.

---

## Contact & Community

* GitHub: https://github.com/Isra-222/rgl

* Issues: https://github.com/Isra-222/rgl/issues

* Discussions: https://github.com/Isra-222/rgl/discussions

---

## Acknowledgments

RGL is developed with a focus on simplicity, performance, and transparency—inspired by the belief that library code should be both powerful and understandable.

##  Attribution

__Random module by__: Ryo Suzuki <reputeless@gmail.com> for _Xoshiro PRNG_  
repo: https://github.com/Reputeless/Xoshiro-cpp.git

__Distribution module by__: Daniel Lemire <https://github.com/lemire>
repo: https://github.com/lemire/fastrange

__Hash module by__: Wang Yi <godspeed_china@yeah.net>  
|- contributors: Reini Urban, Dietrich Epp, Joshua Haberman, Tommy Ettinger, Daniel Lemire, Otmar Ertl, cocowalla,  
|_$ leo-yuriev, Diego Barrios Romero, paulie-g, dumblob, Yann Collet, ivte-ms, hyb, James Z.M. Gao, easyaspi314 (Devin), TheOneric for _wyhash_  
repo: <https://github.com/wangyi-fudan/wyhash>

---

## Next steps

Keep in mind:

* Improve the memory driver in `memory/new.h`.

---