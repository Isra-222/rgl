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
#include "rgl/os/pthread.h"

#include "rgl/os/sys/thread.h"
#include "rgl/os/sys/futex.h"
#include "rgl/os/sys/process.h"

#include "rgl/async/atomic.h"

#include "rgl/memory/thread/stack_global.h"
#include "rgl/core/utility/memory.h"

#include "rgl/memory/new.h"

using namespace rgl;

void __rgl_run_thread_dtors();

namespace rgl::internal{
    void* allocate_thread_storage();
}


int sys::pthread_create(pthread_t* __restrict__ thread, const pthread_attr_t* __restrict__ attr, 
	thread_routine_t start_routine, void* __restrict__ arg) {
	static rgl::atomic<rgl::uint32_t> count = 1;

	size_t stack_size = RGL_DEFAULT_STACK_SIZE;
	int detach_state = attr ? attr->detach_state : 0;

	void* stack_mem = internal::GLOBAL_STACK_CACHE.getStack();
    if(!stack_mem) return -1; // EAGAIN o ENOMEM

    auto* tcb = reinterpret_cast<internal::thread_control_block*>(internal::allocate_thread_storage());
    if(!tcb) {
        internal::GLOBAL_STACK_CACHE.releaseStack(stack_mem);
        return -1;
    }

    tcb->self = tcb;
    new (&tcb->my_cache) internal::thread_cache(count.fetch_add(1, memory_order_relaxed));
    tcb->stack_base = stack_mem;
    tcb->stack_size = stack_size;
    tcb->routine = start_routine;
    tcb->arg = arg;
    tcb->retval = nullptr;
    tcb->detach_state = detach_state;
    tcb->state.store(0, memory_order_relaxed);
    tcb->jf.store(0, memory_order_relaxed);
    tcb->tgid = internal::get_control_block()->tgid;
    tcb->thread_dtor_count = 0;

    struct clone_args args;
    args.flags = clone_f::vm | clone_f::fs | clone_f::files | 
		clone_f::sighand | clone_f::thread | clone_f::settls | 
		clone_f::parent_settid | clone_f::child_cleartid;

    args.stack = reinterpret_cast<uint64_t>(stack_mem);
    args.stack_size = stack_size;
    args.parent_tid = reinterpret_cast<uint64_t>(&(tcb->tid));
    args.child_tid  = reinterpret_cast<uint64_t>(&(tcb->tid));
    args.tls = reinterpret_cast<uint64_t>(tcb);
    args.exit_signal = 0;

    long tid;
    __asm__ volatile (
        "syscall"
        : "=a" (tid)
        : "a" (435), // __NR_clone3
          "D" (&args),
          "S" (sizeof(clone_args))
        : "rcx", "r11", "memory"
    );

    if(tid < 0){
    	internal::GLOBAL_STACK_CACHE.releaseStack(stack_mem);
        return static_cast<int>(tid);
    }
    if(tid == 0){
    	internal::thread_trampoline();
    }

    tcb->tid = static_cast<int32_t>(tid);
    *thread = pthread_t(tcb);

    return 0;
}

int sys::pthread_join(pthread_t thread, void** retval) {
	auto* tcb = thread.handle;

	while(tcb->state.load(memory_order_acquire) == 0) {
		tcb->jf.store(1, memory_order_relaxed);
		while(tcb->state.load(memory_order_acquire) == 0)
			sys::futex_wait(&tcb->jf, 1);
	}
	if(retval)
		*retval = tcb->retval;

	if(tcb->detach_state == 0)
		internal::GLOBAL_STACK_CACHE.releaseStack(tcb->stack_base);

	return 0;
}

int sys::pthread_detach(pthread_t thread) {
	auto* tcb = thread.handle;

	if(tcb->state.load(memory_order_acquire) != 0)
		internal::GLOBAL_STACK_CACHE.releaseStack(tcb->stack_base);
	else
		tcb->detach_state = 1;
	
	return 0;
}

sys::pthread_t sys::pthread_self(void) {
	return pthread_t(internal::get_control_block());
}

void sys::pthread_kill(pthread_t thread, signal sig) {
	tgkill(internal::get_control_block()->tgid, thread.handle->tid, static_cast<int>(sig));
}
[[noreturn]] 
void sys::pthread_exit(void* retval){
	auto* tcb = internal::get_control_block();
    tcb->retval = retval;
    
    __rgl_run_thread_dtors();
    
    tcb->state.store(1, memory_order_relaxed);
    tcb->jf.store(2, memory_order_release);
    rgl::sys::futex_wake(&tcb->jf, 1);

    if(tcb->detach_state == 1)
    	internal::GLOBAL_STACK_CACHE.releaseStack(tcb->stack_base);

    sys::exit(0);
}