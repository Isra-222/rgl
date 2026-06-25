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
//array
#pragma once
#include "rgl/core/types.h"

namespace rgl {
    template<typename T, size_t N>
    class array {
        T data[N];
    public:
        constexpr size_t size() const { return N; }

        constexpr T& operator[](size_t index) { return data[index]; }
        constexpr const T& operator[](size_t index) const { return data[index]; }

        using iterator = T*;
        using const_iterator = const T*;

        constexpr iterator begin() { return data; }
        constexpr iterator end() { return data + N; }

        constexpr const_iterator begin() const { return data; }
        constexpr const_iterator end() const { return data + N; }

        constexpr bool operator==(const array& other) const {
            for (size_t i = 0; i < N; ++i) {
                if (data[i] != other.data[i]) return false;
            }
            return true;
        }

        constexpr bool operator!=(const array& other) const {
            return !(*this == other);
        }

        constexpr T* data_ptr() { return data; }
        constexpr const T* data_ptr() const { return data; }
    };
}