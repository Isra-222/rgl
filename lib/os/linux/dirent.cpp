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
#include "rgl/os/dirent.h"
#include "rgl/os/fcntl.h"
#include "rgl/os/unistd.h"

#include "rgl/memory/new.h"

using namespace rgl;

long sys::getdents(file_descriptor fd, void* dirp, size_t count){
	long ret;

	__asm__ volatile(
		"syscall"
		: "=a" (ret)
		: "a"  (217), // __NR_getdents64
		  "D"  (fd.get()),
		  "S"  (dirp),
		  "d"  (count)
		: "memory", "rcx", "r11"
	);

	return ret;
}
int sys::closedir(DIR* dir){
	if(!dir) return -1;

	long result = sys::close(dir->fd);

	delete dir;

	return (result < 0)? -1 : 0;
}
sys::DIR* sys::opendir(const char* path){
	file_descriptor fd = sys::open(path, file_f::rdonly);
	if(!fd) return nullptr;

	return new DIR(fd, 0, 0);
}
rgl::sys::dirent* sys::readdir(DIR* dir){
	if(!dir) return nullptr;

	if (dir->bpos >= dir->bsize) {
        long nread = getdents(dir->fd, dir->buffer, sizeof(dir->buffer));
        if (nread <= 0) {
            return nullptr;
        }
        dir->bsize = nread;
        dir->bpos = 0;
    }
    dirent* entry = reinterpret_cast<dirent*>(dir->buffer + dir->bpos);
    dir->bpos += entry->d_reclen;

    return entry;
}