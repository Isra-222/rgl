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
#include "rgl/os/sys/futex.h"
#include "rgl/os/sys/time.h"

using namespace rgl;

long sys::futex_call(volatile void* addr, uint32_t val, futex_f flag, const timespec* timeout){
	long ret;
	register const timespec* __timeout __asm__("r10") = timeout;

	__asm__ volatile(
		"syscall"
		: "=a" (ret)
		: "a" (202), //__NR_futex
		  "D" (addr),
		  "S" (static_cast<int>(flag)),
		  "d" (static_cast<int>(val)),
		  "r" (__timeout)
		: "rcx", "r11", "memory"
	);

	return ret;
}