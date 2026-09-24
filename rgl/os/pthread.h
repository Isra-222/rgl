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
#ifndef RGL_OS_PTHREAD_H
#define RGL_OS_PTHREAD_H

#include "rgl/core/types.h"
#include "rgl/os/internal/thread_control_block.h"

namespace rgl::sys{
	struct pthread_attr_t {
		size_t stack_size;
		size_t guard_size;
		int detach_state;

		constexpr pthread_attr_t() noexcept
		: stack_size(2 * 1024 * 1024), guard_size(4096), detach_state(0) {}
		constexpr pthread_attr_t(size_t sstack) noexcept
		: stack_size(sstack), guard_size(4096), detach_state(0) {}
	};

	struct pthread_t { 
		internal::thread_control_block* handle; 
		pthread_t(internal::thread_control_block* h) : handle(h) {}

		bool operator==(nullptr_t) const noexcept { return handle == nullptr; }
		bool operator!=(nullptr_t) const noexcept { return handle != nullptr; }
	};

	int pthread_create(pthread_t* __restrict__ thread, const pthread_attr_t* __restrict__ attr,
		thread_routine_t start_routine, void* __restrict__ arg);

	int pthread_join(pthread_t thread, void** retval);
	int pthread_detach(pthread_t thread);
	pthread_t pthread_self(void);
	
	//NOTE ADD ENUM CLASS SIGNALS
	void pthread_kill(pthread_t thread, int sig);

	[[noreturn]] 
    [[deprecated("rgl::sys::pthread_exit bypasses C++ stack unwinding and skips RAII destructors. Use standard return instead.")]]
    void pthread_exit(void* retval);

	int nanosleep(const struct timespec* duration, timespec* rem);
} //namespace rgl::sys

#endif