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

//error
#pragma once

namespace rgl {
    
    struct source_location {
        const char* file;
        int line;
        const char* function;
    };

    #define rgl_here rgl::source_location{__FILE__, __LINE__, __builtin_FUNCTION()}

    [[noreturn]] void abort() noexcept;
    [[noreturn]] void panic(const char* message, source_location loc = rgl_here);

    [[noreturn]] void unreachable() noexcept;

    #define rgl_assert(condition) \
        do { \
            if (!(condition)) { \
                rgl::panic("Assert failed: " #condition); \
            } \
        } while(0)
}