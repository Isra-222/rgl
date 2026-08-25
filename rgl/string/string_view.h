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

#include "rgl/core/hash.h"
#include "rgl/core/types.h"
#include "rgl/core/utility/memory.h"

namespace rgl {

    class string_view{
        const char* ptr;
        size_t len;
    public:
        static constexpr size_t npos = ~static_cast<size_t>(0);

        string_view() : ptr(nullptr), len(0) {}
        string_view(const char* str) : ptr(str), len(str ? strlen(str) : 0) {}
        string_view(const char* str, size_t l) : ptr(str), len(l) {}
        template <size_t N>
        constexpr string_view(const char (&str)[N]) noexcept : ptr(str), len(N - 1) {}
    
        const char* data() const { return ptr; }
        size_t size() const { return len; }

        constexpr string_view substr(size_t pos = 0, size_t count = npos) const{
            if(pos >= len) return string_view("", 0);
            size_t rlen = (count == npos || pos + count > len) ? (len - pos) : count;
            return string_view(ptr + pos, rlen);
        }

        char operator[](size_t i) const { return ptr[i]; }
        operator bool() const { return ptr != nullptr && len > 0; }
    };

    template <>
    struct hasher<rgl::string_view> {
        inline uint64_t operator()(const rgl::string_view& key, uint64_t seed) const {
            return wyhash(reinterpret_cast<const uint8_t*>(key.data()), key.size(), seed);
        }
    };
}//namespace rgl
