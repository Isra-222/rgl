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

//move.h
#pragma once
#include "rgl/core/traits.h"

namespace rgl {
    template<typename T>
    constexpr typename remove_reference<T>::type&& move(T&& arg) noexcept {
        return static_cast<typename remove_reference<T>::type&&>(arg);
    }

    template<typename T>
    constexpr T&& forward(typename remove_reference<T>::type& arg) noexcept {
        return static_cast<T&&>(arg);
    }

    template<typename T>
    void swap(T& a, T& b) {
        T temp = move(a);
        a = move(b);
        b = move(temp);
    }
}