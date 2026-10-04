#ifndef RGL_NEW_H
#define RGL_NEW_H
#ifdef RGL_NOT_OVERRIDE_GLOBAL_NEW

#else

#pragma GCC system_header

#define _NEW
#define __NEW__
#define _GLIBCXX_NEW

#include "rgl/core/types.h"

inline void* operator new(rgl::size_t, void* p) noexcept {
    return p;
}

inline void* operator new[](rgl::size_t, void* p) noexcept {
    return p;
}

#endif //RGL_NOT_OVERRIDE_GLOBAL_NEW
#endif // RGL_NEW_H
