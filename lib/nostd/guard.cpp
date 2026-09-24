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

#include "rgl/core/error.h"
#include "rgl/os/internal/thread_control_block.h"


#ifndef MAX_DESTRUCTORS
	#define MAX_DESTRUCTORS 256
#endif

extern "C" {

	void* __dso_handle = nullptr;

	int __cxa_guard_acquire(long* guard) { return !(*guard); }
	void __cxa_guard_release(long* guard) { *guard = 1; }
	void __cxa_guard_abort(long*) {}

    static rgl::internal::__rgl_DestructorEntry dtor_table[MAX_DESTRUCTORS];
    static rgl::size_t dtor_count = 0;

    int __cxa_atexit(void (*destructor)(void*), void* arg, void* dso_handle) {
    	if (dtor_count >= MAX_DESTRUCTORS) {
    		panic("several global objects redifine the macro MAX_DESTRUCTORS");
    		return -1;
    	}
    	dtor_table[dtor_count++] = rgl::internal::__rgl_DestructorEntry{destructor, arg, dso_handle};
        return 0;
    }

    void __cxa_finalize(void* dso_handle) {
        for (long i = static_cast<long>(dtor_count) - 1; i >= 0; --i) {
            if (dtor_table[i].destructor) {
                if (dso_handle == nullptr || dtor_table[i].dso_handle == dso_handle) {
                    void (*func)(void*) = dtor_table[i].destructor;
                    void* arg = dtor_table[i].arg;
                    dtor_table[i].destructor = nullptr;
                    
                    func(arg);
                }
            }
        }
    }

    int __cxa_thread_atexit(void (*destructor)(void*), void* arg, void* dso_handle) {
        auto* tcb = rgl::internal::get_control_block();
        if (!tcb) return -1;

        for (rgl::size_t i = 0; i < tcb->thread_dtor_count; ++i) {
            if (tcb->thread_dtor_table[i].destructor == destructor && 
                tcb->thread_dtor_table[i].arg == arg) {
                return 0;
            }
        }

        if (tcb->thread_dtor_count >= MAX_THREAD_DESTRUCTORS) {
            panic("Too many thread_local destructors (exceeded MAX_THREAD_DESTRUCTORS)\nredifine the macro \" MAX_THREAD_DESTRUCTORS\"");
            return -1;
        }

        tcb->thread_dtor_table[tcb->thread_dtor_count++] = rgl::internal::__rgl_DestructorEntry{
            destructor, arg, dso_handle
        };
        return 0;
    }
}
void __rgl_run_thread_dtors() {
    auto* tcb = rgl::internal::get_control_block();
    if (!tcb) return;
    for (long i = static_cast<long>(tcb->thread_dtor_count) - 1; i >= 0; --i) {
        if (tcb->thread_dtor_table[i].destructor) {
            auto func = tcb->thread_dtor_table[i].destructor;
            void* arg = tcb->thread_dtor_table[i].arg;
            
            tcb->thread_dtor_table[i].destructor = nullptr;
            func(arg);
        }
    }
}
