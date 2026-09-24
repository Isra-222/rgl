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
#ifndef RGL_OS_FCNTL_H
#define RGL_OS_FCNTL_H

#include "rgl/core/types.h"
#include "rgl/os/unistd.h"

namespace rgl::sys{
	enum class file_f : int {
		rdonly      = 00,
        wronly      = 01,
        rdwr        = 02,
        creat       = 0100,
        excl        = 0200,
        noctty      = 0400,
        trunc       = 01000,
        append      = 02000,
        nonblock    = 04000,
        sync        = 04010000,
        directory   = 0200000,
        nofollow    = 0400000,
        cloexec     = 02000000
	};
	inline constexpr file_f operator|(file_f a, file_f b) noexcept { return static_cast<file_f>(static_cast<int>(a) | static_cast<int>(b)); }
	inline constexpr file_f operator&(file_f a, file_f b) noexcept { return static_cast<file_f>(static_cast<int>(a) & static_cast<int>(b)); }

	enum class file_perm : int {
        none        = 0,   
        owner_read  = 0400,
        owner_write = 0200,
        owner_exec  = 0100,
        
        group_read  = 0040,
        group_write = 0020,
        group_exec  = 0010,
        
        others_read = 0004,
        others_write= 0002,
        others_exec = 0001,
    
        defaults    = 0666,
        standard    = 0644,
        private_file= 0600
    };
    inline constexpr file_perm operator|(file_perm a, file_perm b) noexcept 
    { return static_cast<file_perm>(static_cast<int>(a) | static_cast<int>(b)); }
    
    inline constexpr file_perm operator&(file_perm a, file_perm b) noexcept 
    { return static_cast<file_perm>(static_cast<int>(a) & static_cast<int>(b)); }

	file_descriptor open(const char* filename, file_f flags, file_perm mode = file_perm::defaults);

}// namespace rgl::sys

#endif