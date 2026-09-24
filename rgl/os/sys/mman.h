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
#ifndef RGL_SYS_MMAN_H
#define RGL_SYS_MMAN_H

#include "rgl/core/types.h"

namespace rgl::sys{

	#ifdef __OFF_T_TYPE__
        using off_t = __OFF_T_TYPE__;
    #else
        using off_t = int64_t;
    #endif


	namespace detail{
		inline void* map_failed() { return reinterpret_cast<void*>(static_cast<uintptr_t>(-1)); }
	}//namespace detail

	enum class prot : int {
		none = 0,
		read = 1,
		write = 2,
		exec = 4
	};
	inline constexpr prot operator|(prot a, prot b) { return static_cast<prot>(static_cast<int>(a) | static_cast<int>(b)); }
	
	enum class map : int {
        shared              = 0x01,
        _private            = 0x02,
        shared_validate     = 0x03, //linux extension
        fixed               = 0x10,
        anon                = 0x20,
        anonymous           = 0x20,
        growsdown           = 0x0100,
        denywrite           = 0x0800,
        executable          = 0x1000,
        locked              = 0x2000,
        noreserve           = 0x4000,
        populate            = 0x8000,
        nonblock            = 0x10000,
        stack               = 0x20000,
        huge_2mb            = 0x2400000,
        huge_1gb            = 0x3800000,
        hugetlb             = 0x400000,
        _32BIT              = 0x40,
        fixed_noreplace     = 0x100000,
        file                = 0x00,
        sync                = 0x80000,
        uninitialized       = 0x4000000
    };
	inline void* MAP_FAILED = detail::map_failed();


	inline constexpr map operator|(map a, map b) { return static_cast<map>(static_cast<int>(a) | static_cast<int>(b)); }
	
	/*enum class m_error : int {
		acces,
		again,
		exist,
		inval,
		nfile,
		nodev,
		nomen,
		overflow,
		perm,
		txtbsy,
	};*/


	void* mmap(void* addr, size_t length, prot p, map flags, 
		int fd, off_t offset);

	int munmap(void* addr, size_t length);
}//namespace rgl::sys

#endif //RGL_SYS_MMAN_H