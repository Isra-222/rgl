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

#include "rgl/os/internal/thread_control_block.h"
#include "rgl/os/sys/process.h"
#include "rgl/os/sys/futex.h"
#include "rgl/memory/thread/stack_global.h"
#include "rgl/memory/new.h"
#include "rgl/memory/rmalloc.h"

using namespace rgl;

internal::thread_control_block* internal::get_control_block() {
	thread_control_block* tcb;
	__asm__ volatile(
		"movq %%fs:0, %0"
		: "=r" (tcb)
	);
	return tcb;
}

//-------------------------------------

void __rgl_run_thread_dtors();

namespace rgl::internal{
	void* allocate_thread_storage();
}

void internal::thread_trampoline(){
	auto* tcb = get_control_block();
	if(!tcb || !tcb->routine){
		tcb->~thread_control_block();
		sys::exit(-1);
	}

	void* retval = tcb->routine(tcb->arg);
	tcb->retval = retval;

    __rgl_run_thread_dtors();

	tcb->state.store(1, memory_order_release);
	tcb->jf.store(2, memory_order_release);
    rgl::sys::futex_wake(&tcb->jf, 1);


	if(tcb->detach_state == 1)
		internal::GLOBAL_STACK_CACHE.releaseStack(tcb->stack_base);

	
    rgl::rfree(tcb->raw_tls_allocation);
	tcb->~thread_control_block();
	rgl::sys::exit(0);
}