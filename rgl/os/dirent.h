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
#ifndef RGL_OS_DIRENT_H
#define RGL_OS_DIRENT_H

#include "rgl/core/types.h"
#include "rgl/os/unistd.h"

namespace rgl::sys{
	enum class file_type : unsigned char {
        unknown  = 0,
        fifo     = 1,
        chr      = 2,
        dir      = 4,
        blk      = 6,
        reg      = 8,
        lnk      = 10,
        sock     = 12,
        wht      = 14
    };

    struct dirent {
        uint64_t        d_ino;
        int64_t         d_off;
        unsigned short  d_reclen;
        file_type   	d_type;
        char            d_name[];
    };
    struct DIR {
        file_descriptor fd;
        char buffer[4096];
        long bpos;
        long bsize;
        DIR(file_descriptor fd, long bpos, long bsize)
        : fd(fd), bpos(bpos), bsize(bsize) {}
    };

    long getdents(file_descriptor fd, void* dirp, size_t count);

    int closedir(DIR* dir);
    DIR* opendir(const char* path);
    dirent* readdir(DIR* dir);


    constexpr uint32_t S_IFMT  = 0170000; // Bit mask for the file type bit field
    constexpr uint32_t S_IFREG = 0100000; // Regular file
    constexpr uint32_t S_IFDIR = 0040000; // Directory

    inline constexpr bool S_ISREG(uint32_t mode) { return (mode & S_IFMT) == S_IFREG; }
    inline constexpr bool S_ISDIR(uint32_t mode) { return (mode & S_IFMT) == S_IFDIR; }

}// namespace rgl::sys

#endif