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
#include "rgl/os/sys/mman.h"

void* rgl::sys::mmap(void* addr, size_t length, prot p, map flags, 
	int fd, off_t offset){
	int64_t ret;

	register void*    __addr   __asm__("rdi") = addr;
  register size_t   __len    __asm__("rsi") = length;
  register int      __prot   __asm__("rdx") = static_cast<int>(p);
  register int      __flags  __asm__("r10") = static_cast<int>(flags);
  register long     __fd     __asm__("r8")  = static_cast<long>(fd);
  register off_t    __off    __asm__("r9")  = offset;
    
	__asm__ volatile (
        "syscall"
        : "=a" (ret)
        : "a" (9), // __NR_mmap
          "r" (__addr),
          "r" (__len),
          "r" (__prot),
          "r" (__flags),
          "r" (__fd),
          "r" (__off)
        : "rcx", "r11", "memory"
    );

	if(ret < 0 && ret >= -4095)
		return MAP_FAILED;

	return reinterpret_cast<void*>(ret);
}

int rgl::sys::munmap(void* addr, size_t length){
	int64_t ret;
	__asm__ volatile (
		"syscall"
		: "=a" (ret)
		: "a" (11), //__NR_munmap
		  "D" (addr),
		  "S" (length)
		: "rcx", "r11", "memory"
	);
	return static_cast<int>(ret);
}