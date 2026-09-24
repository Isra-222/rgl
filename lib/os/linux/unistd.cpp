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
#include "rgl/os/unistd.h"

using namespace rgl;

long sys::read(file_descriptor fd, void* buf, size_t count){
	long ret;
	__asm__ volatile(
		"syscall"
		: "=a" (ret)
		: "a"  (0), // __NR_read
		  "D"  (fd.get()),
		  "S"  (buf),
		  "d"  (count)
		: "memory", "rcx", "r11"
	);
	return ret;
}
long sys::write(file_descriptor fd, const void* buf, size_t count){
	long ret;
	__asm__ volatile(
		"syscall"
		: "=a" (ret)
		: "a"  (1), // __NR_write
		  "D"  (fd.get()),
		  "S"  (buf),
		  "d"  (count)
		: "memory", "rcx", "r11"
	);
	return ret;
}
long sys::close(file_descriptor fd){
	long ret;
	__asm__ volatile(
		"syscall"
		: "=a" (ret)
		: "a"  (3), // __NR_close
		  "D"  (fd.get())
		: "memory", "rcx", "r11"
	);
	return ret;
}
long sys::lseek(file_descriptor fd, long offset, int whence){
	long ret;
	__asm__ volatile(
		"syscall"
		: "=a" (ret)
		: "a"  (8), // __NR_lseek
		  "D"  (fd.get()),
		  "S"  (offset),
		  "d"  (whence)
		: "memory", "rcx", "r11"
	);
	return ret;
}