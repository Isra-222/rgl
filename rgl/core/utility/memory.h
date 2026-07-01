/*
 * Regular Library (rgl)
 * Copyright (C) 2026 [Isra-222]
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see
 * <https://www.gnu.org/licenses/>.
*/

//memory.h
#pragma once
#include "rgl/core/types.h"

namespace rgl {

    inline void* memcpy(void* dest, const void* src, size_t count) {
        #if __has_builtin(__builtin_memcpy)
            return __builtin_memcpy(dest, src, count);
        #else
            char* d = static_cast<char*>(dest);
            const char* s = static_cast<const char*>(src);
            while (count--) *d++ = *s++;
            return dest;
        #endif
    }

    inline size_t strlen(const char* str) {
        #if __has_builtin(__builtin_strlen)
            return __builtin_strlen(str);
        #else
            const char* s = str;
            while (*s) s++;
            return static_cast<size_t>(s - str);
        #endif
    }

    inline int memcmp(const void* ptr1, const void* ptr2, size_t count) {
        #if __has_builtin(__builtin_memcmp)
            return __builtin_memcmp(ptr1, ptr2, count);
        #else
            const unsigned char* p1 = static_cast<const unsigned char*>(ptr1);
            const unsigned char* p2 = static_cast<const unsigned char*>(ptr2);
            while (count-- > 0) {
                if (*p1 != *p2) return (*p1 > *p2) ? 1 : -1;
                p1++; p2++;
            }
            return 0;
        #endif
    }
}