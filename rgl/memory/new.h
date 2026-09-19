#ifndef RGL_NEW_H
#define RGL_NEW_H
#ifdef RGL_NOT_OVERRIDE_GLOBAL_NEW

#else

#pragma GCC system_header

#define _NEW
#define __NEW__
#define _GLIBCXX_NEW

#include "rgl/core/types.h"

#include "rmalloc.h"

inline void* operator new(rgl::size_t size) {
    return rgl::rmalloc(size);
}

inline void* operator new(rgl::size_t, void* p) noexcept {
    return p;
}

inline void operator delete(void* p) noexcept {
    rgl::rfree(p);
}

inline void* operator new[](rgl::size_t size) {
    rgl::size_t* ptr = static_cast<rgl::size_t*>(rgl::rmalloc(size + sizeof(rgl::size_t)));
    *ptr = size;
    return static_cast<void*>(ptr + 1);
}

inline void* operator new[](rgl::size_t, void* p) noexcept {
    return p;
}

inline void operator delete[](void* p) noexcept {
    if(!p) return;
    rgl::size_t* ptr = static_cast<rgl::size_t*>(p) - 1;
    rgl::rfree(ptr);
}
inline void operator delete(void* p, rgl::size_t size) noexcept {
    if(!p) return;
    rgl::size_t* ptr = static_cast<rgl::size_t*>(p) - 1;
    rgl::rfree(p);
}

inline void operator delete[](void* p, rgl::size_t size) noexcept {
    if(!p) return;
    rgl::size_t* ptr = static_cast<rgl::size_t*>(p) - 1;
    rgl::rfree(p);
}

#endif //RGL_NOT_OVERRIDE_GLOBAL_NEW
#endif // RGL_NEW_H
