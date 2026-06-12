# RGL - Regular Library

RGL (Regular Library) is a custom, minimalist C++ library designed to operate independently of the standard library (std). It is built from the ground up to provide essential containers and utilities while maintaining a low footprint and high performance.

## Core Philosophy

The primary goal of RGL is to offer a std-free alternative for C++ development, making it ideal for:

* __Game Engines__: Where fine-grained memory control is critical.
* __Embedded Systems__: Where minimal dependency and binary size are required.
* __Low-Level Systems__: Learning and understanding the mechanics behind core C++ features like memory management and container implementation.

## Key Features

* __Custom Containers__: Includes high-performance rgl::vector and rgl::map implementations with move semantics and perfect forwarding.
* __Minimalist Memory Management__: Full control over memory allocation through custom new and delete operators, avoiding standard library overhead.
* __Zero-Dependency__: Does not rely on std, allowing for greater portability across platforms and environments.
* __Modern C++ Semantics__: Fully leverages move semantics, emplace methods, and custom traits for efficient object handling.

## Components

Some components are:

* __rgl/vector__: Dynamic array implementation with emplace_back and move-aware resizing.
* __rgl/map__: Open-addressing hash map with efficient linear probing.
* __rgl/string__: Custom string class with custom memory ownership and deep-copy capabilities.
* __rgl/iostream__: Custom I/O stream interface wrapping printf and scanf.
* __rgl/memory__: Smart pointers (unique_ptr) and RAII-based memory management tools.

## Why use RGL?

If you are building an engine or a system where every byte and every instruction cycle counts, RGL provides you with the building blocks to control exactly how your objects are created, moved, and destroyed—without the hidden complexities of the standard template library.

---

## LICENSE

__*LGPLv3*__

---