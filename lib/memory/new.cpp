#ifndef RGL_NOT_OVERRIDE_GLOBAL_NEW

#include "rgl/core/types.h"
#include "rgl/memory/rmalloc.h"

void* operator new(rgl::size_t size) {
    return rgl::rmalloc(size);
}

void operator delete(void* p) noexcept {
    rgl::rfree(p);
}

void* operator new[](rgl::size_t size) {
    rgl::size_t* ptr = static_cast<rgl::size_t*>(rgl::rmalloc(size + sizeof(rgl::size_t)));
    *ptr = size;
    return static_cast<void*>(ptr + 1);
}

void operator delete[](void* p) noexcept {
    if(!p) return;
    rgl::size_t* ptr = static_cast<rgl::size_t*>(p) - 1;
    rgl::rfree(ptr);
}

void operator delete(void* p, rgl::size_t) noexcept {
    rgl::rfree(p);
}

void operator delete[](void* p, rgl::size_t) noexcept {
    if(!p) return;
    rgl::size_t* ptr = static_cast<rgl::size_t*>(p) - 1;
    rgl::rfree(ptr);
}

#endif