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
#include "rgl/os/sys/thread.h"
#include "rgl/os/sys/time.h"

using namespace rgl;

long sys::clock_nanosleep(const timespec* req, const timespec* rem){
	long ret;

	register const timespec* __req __asm__("rdi") = req;
	register const timespec* __rem __asm__("rsi") = rem;

	__asm__ volatile (
		"syscall"
		: "=a" (ret)
		: "a" (35), //__NR_nanosleep
		  "r" (__req),
		  "r" (__rem)
		: "rcx", "r11", "memory"
	);

	return ret;
}
long sys::clone(clone_f flags, void* stack, pid_t* parent_tid, pid_t* child_tid, unsigned long tls){
	long ret;

	register clone_f     __flags      __asm__("rdi") = flags;
    register void*       __stack      __asm__("rsi") = stack;
    register pid_t*      __parent_tid __asm__("rdx") = parent_tid;
    register pid_t*      __child_tid  __asm__("r10") = child_tid;
    register unsigned long __tls      __asm__("r8")  = tls;

	__asm__ volatile (
        "syscall"
        : "=a" (ret)
        : "a" (56), // __NR_clone
          "r" (__flags),
          "r" (__stack),
          "r" (__parent_tid),
          "r" (__child_tid),
          "r" (__tls)
        : "rcx", "r11", "memory"
    );

	return ret;
}

long sys::clone3(const clone_args* cl_args, size_t size){
    long ret;

    __asm__ volatile (
        "syscall"
        : "=a" (ret)
        : "a" (435), // __NR_clone3
          "D" (cl_args),
          "S" (size)
        : "rcx", "r11", "memory"
    );

    return ret;
}

long sys::arch_prctl(int code, unsigned long addr){
    long ret;
    __asm__ volatile (
        "syscall"
        : "=a" (ret)
        : "a" (158), //__NR_prctl
          "D" (code), 
          "S" (addr)
        : "rcx", "r11", "memory"
    );
    return ret;
}
long sys::gettid(void) {
    long ret;
    __asm__ volatile (
        "syscall"
        : "=a" (ret) //__NR_gettid
        : "a" (186)
        : "rcx", "r11", "memory"
    );
    return ret;
}