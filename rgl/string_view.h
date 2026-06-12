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

//string_view

#pragma once

#include "types.h"

namespace rgl {
    inline size_t strlen(const char* str){
        const char* s = str;
        while(*s){
            s++;
        }
        return static_cast<size_t>(s - str);
    }

    inline bool is_space(char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
    }

    inline bool is_alpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    inline bool is_digit(char c) {
        return c >= '0' && c <= '9';
    }

    inline bool is_alnum(char c) {
        return is_alpha(c) || is_digit(c);
    }

    class string_view{
        const char* ptr;
        size_t len;
    public:
        string_view() : ptr(nullptr), len(0) {}
        string_view(const char* str) : ptr(str), len(str ? strlen(str) : 0) {}
        string_view(const char* str, size_t l) : ptr(str), len(l) {}
    
        const char* data() const { return ptr; }
        size_t size() const { return len; }

        char operator[](size_t i) const { return ptr[i]; }
        operator bool() const { return ptr != nullptr && len > 0; }
    };


}//namespace rgl
