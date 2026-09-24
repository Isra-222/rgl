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
#include "rgl/os/fcntl.h"

using namespace rgl;

sys::file_descriptor sys::open(const char* filename, file_f flags, file_perm mode){
	long ret;
	__asm__ volatile(
		"syscall"
		: "=a" (ret)
		: "a"  (2),  //__NR_open
		  "D"  (filename),
		  "S"  (static_cast<int>(flags)),
		  "d"  (static_cast<int>(mode))
		: "memory", "rcx", "r11"
	);
	return sys::file_descriptor(ret);
}