#ifndef RGL_NEW_H
#define RGL_NEW_H

#pragma GCC system_header

#define _NEW
#define __NEW__
#define _GLIBCXX_NEW

#include "../core/types.h"

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
inline void operator delete(void* p, rgl::size_t size) noexcept {
    ::free(p);
}

inline void operator delete[](void* p, rgl::size_t size) noexcept {
    ::free(p);
}

#endif // RGL_NEW_H
