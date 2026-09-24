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
#ifndef RGL_SYS_THREAD_H
#define RGL_SYS_THREAD_H

#include "rgl/core/types.h"

namespace rgl::sys{
	struct timespec;

	using pid_t = int32_t;

	long clock_nanosleep(const timespec* req, const timespec* rem = nullptr);

	enum class clone_f : uint64_t {
    // Cloning flags
	    csignal              = 0x000000ffULL,
	    vm                   = 0x00000100ULL,
	    fs                   = 0x00000200ULL,
	    files                = 0x00000400ULL,
	    sighand              = 0x00000800ULL,
	    pidfd                = 0x00001000ULL,
	    ptrace               = 0x00002000ULL,
	    vfork                = 0x00004000ULL,
	    parent               = 0x00008000ULL,
	    thread               = 0x00010000ULL,
	    newns                = 0x00020000ULL,
	    sysvsem              = 0x00040000ULL,
	    settls               = 0x00080000ULL,
	    parent_settid        = 0x00100000ULL,
	    child_cleartid       = 0x00200000ULL,
	    detached             = 0x00400000ULL,
	    untraced             = 0x00800000ULL,
	    child_settid         = 0x01000000ULL,
	    newcgroup            = 0x02000000ULL,
	    newuts               = 0x04000000ULL,
	    newipc               = 0x08000000ULL,
	    newuser              = 0x10000000ULL,
	    newpid               = 0x20000000ULL,
	    newnet               = 0x40000000ULL,
	    io                   = 0x80000000ULL,

	    // Flags for the clone3() syscall
	    clear_sighand        = (1ULL << 32),
	    into_cgroup          = (1ULL << 33),
	    autoreap             = (1ULL << 34),
	    nnp                  = (1ULL << 35),
	    pidfd_autokill       = (1ULL << 36),
	    empty_mntns          = (1ULL << 37),

	    // Intersect with CSIGNAL
	    newtime              = 0x00000080ULL,

	    // Unshare flags
	    unshare_empty_mntns  = 0x00100000ULL
	};
	inline clone_f constexpr operator|(clone_f a, clone_f b) { 
		return static_cast<clone_f>(static_cast<uint64_t>(a) | static_cast<uint64_t>(b)); 
	}
	inline constexpr clone_f operator&(clone_f a, clone_f b) {
    	return static_cast<clone_f>(static_cast<uint64_t>(a) & static_cast<uint64_t>(b));
	}
	struct clone_args {
		clone_f  flags;
		uint64_t pidfd;
		uint64_t child_tid;
		uint64_t parent_tid;
		uint64_t exit_signal;
		uint64_t stack;
		uint64_t stack_size;
		uint64_t tls;
		uint64_t set_tid;
		uint64_t set_tid_size;
		uint64_t cgroup;

		clone_args()
		: flags(clone_f::unshare_empty_mntns),pidfd(0),child_tid(0),parent_tid(0),
			exit_signal(0),stack(0),stack_size(0),tls(0),set_tid(0),set_tid_size(0),cgroup(0) {}
	};

	
	long clone(clone_f flags, void* stack, pid_t* parent_tid, pid_t* child_tid, unsigned long tls);
	long clone3(const clone_args* cl_args, size_t size);
	long arch_prctl(int code, unsigned long addr);
	long gettid(void);

}//namespace rgl

#endif