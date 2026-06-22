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
//types
#pragma once

namespace rgl {
	using uint8_t = unsigned char;
	using int8_t = signed char;

	using uint16_t = unsigned short;
	using int16_t = signed short;

	using uint32_t = unsigned int;
	using int32_t = signed int;

	using uint64_t = unsigned long long;
    using int64_t  = signed long long;

	#ifdef __SIZE_TYPE__
        using size_t = __SIZE_TYPE__;
    #else
        using size_t = decltype(sizeof(0));
    #endif

    #ifdef __PTRDIFF_TYPE__
        using ssize_t = __PTRDIFF_TYPE__;
    #else
        using ssize_t = signed long long;
    #endif

    using nullptr_t = decltype(nullptr);

    #if defined(__UINTPTR_TYPE__)
        using uintptr_t = __UINTPTR_TYPE__;
    #else
        #if defined(_WIN64) || defined(__LP64__) || defined(_LP64)
            using uintptr_t = unsigned long long;
        #else
            using uintptr_t = unsigned int;
        #endif
    #endif

    #ifndef UINT64_C
        #define UINT64_C(val) val##ULL
    #endif
}