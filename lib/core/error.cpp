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

//error.cpp
#include "rgl/core/error.h"
#include "rgl/io/iostream.h"

#if defined(__linux__)
    #include <unistd.h> 
#elif defined(_WIN32)
    extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int uExitCode);
#endif

namespace rgl {
    [[noreturn]] void abort() noexcept {
        #if defined(__linux__)
            asm volatile (
                "mov $60, %%rax\n\t"
                "mov $1, %%rdi\n\t"
                "syscall"
                : : : "rax", "rdi"
            );
        #elif defined(_WIN32)
            ExitProcess(1);
        #endif
        __builtin_unreachable();
    }
    [[noreturn]] inline void unreachable() noexcept {
	    #if defined(__GNUC__) || defined(__clang__)
	        __builtin_unreachable();
	    #elif defined(_MSC_VER)
	        __assume(false);
	    #endif
	}

	[[noreturn]] void panic(const char* message, source_location loc){
		err << "Panic at " << loc.file << ":" << loc.line 
        << " in function " << loc.function << "\n"
        << "Message: " << message << endl;
		abort();
	}
}