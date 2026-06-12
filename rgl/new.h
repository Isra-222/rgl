//new
#pragma once

#include "types.h"

extern "C" {
    void* malloc(rgl::size_t size);
    void free(void* ptr);
}

inline void* operator new(rgl::size_t size) {
    return malloc(size);
}
inline void* operator new(rgl::size_t, void* p) noexcept {
    return p;
}

inline void operator delete(void* p) noexcept {
    free(p);
}

inline void* operator new[](rgl::size_t size) {
    rgl::size_t* ptr = static_cast<rgl::size_t*>(malloc(size + sizeof(rgl::size_t)));

    *ptr = size;
    return static_cast<void*>(ptr + 1);
}
inline void* operator new[](rgl::size_t, void* p) noexcept {
    return p;
}

inline void operator delete[](void* p) noexcept {
    if(!p) return;

    rgl::size_t* ptr = static_cast<rgl::size_t*>(p) - 1;
    free(ptr);
}