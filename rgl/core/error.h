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
    #if defined(__linux__)
    [[noreturn]] inline void abort() noexcept {
        asm volatile (
            "mov $60, %%rax\n\t" // sys_exit
            "mov $1, %%rdi\n\t"  // error code 1
            "syscall"
            : : : "rax", "rdi"
        );
        __builtin_unreachable();
    }
    #elif defined(_WIN32)
    extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int uExitCode);
    
    [[noreturn]] inline void abort() noexcept {
        ExitProcess(1);
        __builtin_unreachable();
    }
    #endif
}
