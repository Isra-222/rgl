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
#ifndef RGL_OS_STAT_H
#define RGL_OS_STAT_H

#include "rgl/core/types.h"
#include "rgl/os/unistd.h"

namespace rgl::sys {
	struct stat_ {
        uint64_t        st_dev;
        uint64_t        st_ino;
        uint64_t        st_nlink;
        uint32_t        st_mode;
        uint32_t        st_uid;
        uint32_t        st_gid;
        uint32_t        __pad0;
        uint64_t        st_rdev;
        int64_t         st_size;
        int64_t         st_blksize;
        int64_t         st_blocks;
        uint64_t        st_atime;
        uint64_t        st_atime_nsec;
        uint64_t        st_mtime;
        uint64_t        st_mtime_nsec;
        uint64_t        st_ctime;
        uint64_t        st_ctime_nsec;
        int64_t         __unused[3];
    };

    long stat(const char* pathname, stat_* statbuf);
    long fstat(file_descriptor fd, stat_* statbuf);
    long lstat(const char* pathname, stat_* statbuf);
}//namespace rgl::sys

#endif