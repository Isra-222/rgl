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
#include "rgl/os/sys/time.h"

int rgl::sys::clock_gettime(int clk_id, timespec* tp){
	int64_t ret;
	__asm__ volatile(
		"syscall"
		: "=a" (ret)
		: "a"  (228), // __NR_clock_gettime
		  "D"  (clk_id),
		  "S"  (tp)
		: "rcx", "r11", "memory"
	);
	return static_cast<int>(ret);
}