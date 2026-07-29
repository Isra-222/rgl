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

//limits
#pragma once
#include "types.h"

namespace rgl {

    template<typename T>
    struct numeric_limits;

    #define RGL_DEFINE_LIMITS_UNSIGNED(T) \
    template<> struct numeric_limits<T> { \
        static constexpr T min() { return 0; } \
        static constexpr T max() { return (T)~0ULL; } \
        static constexpr T lowest() { return 0; }\
    };

    #define RGL_DEFINE_LIMITS_SIGNED(T) \
    template<> struct numeric_limits<T> { \
        static constexpr T min() { return (T)(1ULL << (sizeof(T) * 8 - 1)); } \
        static constexpr T max() { return (T)(~(1ULL << (sizeof(T) * 8 - 1))); } \
        static constexpr T lowest() { return min(); }\
    };

    #define RGL_DEFINE_LIMITS_FLOAT(T, min_val, max_val, lowest_val) \
    template<> struct numeric_limits<T> { \
        static constexpr T min() { return min_val; } \
        static constexpr T max() { return max_val; } \
        static constexpr T lowest() { return lowest_val; } \
    };

    RGL_DEFINE_LIMITS_UNSIGNED(uint8_t)
    RGL_DEFINE_LIMITS_UNSIGNED(uint16_t)
    RGL_DEFINE_LIMITS_UNSIGNED(uint32_t)
    RGL_DEFINE_LIMITS_UNSIGNED(uint64_t)

    RGL_DEFINE_LIMITS_SIGNED(int8_t)
    RGL_DEFINE_LIMITS_SIGNED(int16_t)
    RGL_DEFINE_LIMITS_SIGNED(int32_t)
    RGL_DEFINE_LIMITS_SIGNED(int64_t)

    RGL_DEFINE_LIMITS_FLOAT(float, __FLT_MIN__, __FLT_MAX__, -__FLT_MAX__)
    RGL_DEFINE_LIMITS_FLOAT(double, __DBL_MIN__, __DBL_MAX__, -__DBL_MAX__)

    #undef RGL_DEFINE_LIMITS_UNSIGNED
    #undef RGL_DEFINE_LIMITS_SIGNED
}