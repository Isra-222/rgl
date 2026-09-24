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
#include "rgl/core/types.h"
#include "rgl/os/sys/process.h"
#include "rgl/os/internal/thread_control_block.h"
#include "rgl/os/sys/thread.h"

#ifndef ARCH_SET_FS
	#define ARCH_SET_FS 0x1002
#endif

extern int main(void);
extern int main(int argc, char** argv);

using init_fn_t = void (*)(void);

extern "C"{
	extern init_fn_t __init_array_start[];
    extern init_fn_t __init_array_end[];
}

namespace {
    void run_global_constructors() {
        for (auto* func = __init_array_start; func < __init_array_end; ++func) {
            if (*func) {
                (*func)();
            }
        }
    }
}

extern "C" {
	[[noreturn]] void _start(){
		char** stack_pointer;

		__asm__ volatile ("movq %%rsp, %0" : "=r"(stack_pointer));
        __asm__ volatile (
            "xor %%rbp, %%rbp\n\t"
            "andq $-16, %%rsp\n\t"
            : : : "memory"
        );
		int argc = *reinterpret_cast<int*>(stack_pointer);
        char** argv = reinterpret_cast<char**>(stack_pointer + 1);

        static rgl::internal::thread_control_block main_tcb(0);

        main_tcb.self = &main_tcb;
        main_tcb.tid = rgl::sys::gettid();
        main_tcb.tgid = main_tcb.tid;
        main_tcb.stack_base = stack_pointer;
        main_tcb.stack_size = 0;
        main_tcb.routine = nullptr;
        main_tcb.arg = nullptr;
        main_tcb.retval = nullptr;
        main_tcb.detach_state = 0;
        main_tcb.state.store(0, rgl::memory_order_relaxed);
        main_tcb.jf.store(0, rgl::memory_order_relaxed);
        main_tcb.thread_dtor_count = 0;

        rgl::sys::arch_prctl(ARCH_SET_FS, reinterpret_cast<unsigned long>(&main_tcb));
		
        run_global_constructors();

		int exit = main(argc, argv);
		exit += main();
		rgl::sys::exit_group(exit);

		while(1)
			__asm__ volatile ("hlt");
	}
}