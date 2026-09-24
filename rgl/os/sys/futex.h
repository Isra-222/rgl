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

	namespace detail{
		long futex_call(volatile void* addr, uint32_t val, uint8_t flag, const timespec* timeout = nullptr);
	}//namespace detail

	inline long futex_wake(volatile void* addr, uint32_t count){ return detail::futex_call(addr, count, 1); }
	inline long futex_wait(volatile void* addr, uint32_t val){ return detail::futex_call(addr, val, 0); }
	inline long futex_timedwait(volatile void* addr, uint32_t val, const timespec* timeout){
		return detail::futex_call(addr, val, 0, timeout);
	}
}// namespace rgl::sys

#endif