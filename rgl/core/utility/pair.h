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

//pair.h
#pragma once
#include "rgl/core/utility/move.h"
#include "rgl/core/utility/tuple.h"

namespace rgl {
    template<typename T1, typename T2>
    class pair {
    public:
        T1 first;
        T2 second;
        constexpr pair() : first(), second() {}
        constexpr pair(const T1& a, const T2& b) : first(a), second(b) {}
        template<typename U1, typename U2>
        constexpr pair(U1&& a, U2&& b) : first(rgl::move(a)), second(rgl::move(b)) {}
    };

    template<typename T1, typename T2>
    constexpr pair<decay_t<T1>, decay_t<T2>> make_pair(T1&& a, T2&& b) {
        return pair<decay_t<T1>, decay_t<T2>>(forward<T1>(a), forward<T2>(b));
    }
}