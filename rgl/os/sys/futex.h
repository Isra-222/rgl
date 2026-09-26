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
#ifndef RGL_SYS_FUTEX_H
#define RGL_SYS_FUTEX_H

#include "rgl/core/types.h"

namespace rgl::sys{
	
	struct timespec;

	enum class futex_f : uint16_t {
        wait               = 0,
        wake               = 1,
        requeue            = 3,
        cmp_requeue        = 4,
        wake_op            = 5,
        lock_pi            = 6,
        unlock_pi          = 7,
        trylock_pi         = 8,
        wait_bitset        = 9,
        wake_bitset        = 10,
        wait_requeue_pi    = 11,
        cmp_requeue_pi     = 12,

        private_flag       = 128,
        clock_realtime     = 256
    };
	inline constexpr futex_f operator|(futex_f a, futex_f b) {
		return static_cast<futex_f>(static_cast<uint16_t>(a) | static_cast<uint16_t>(b));
    }

	long futex_call(volatile void* addr, uint32_t val, futex_f flag, const timespec* timeout = nullptr);
	
	inline long futex_wake(volatile void* addr, uint32_t count){ return futex_call(addr, count, futex_f::wake | futex_f::private_flag); }
	inline long futex_wait(volatile void* addr, uint32_t val){ return futex_call(addr, val, futex_f::wait | futex_f::private_flag); }
	inline long futex_timedwait(volatile void* addr, uint32_t val, const timespec* timeout){
		return futex_call(addr, val, futex_f::wait | futex_f::private_flag, timeout);
	}
}// namespace rgl::sys

#endif