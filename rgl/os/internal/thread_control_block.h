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
#pragma once

#include "rgl/core/types.h"
#include "rgl/async/atomic.h"
#include "rgl/memory/regular/thread_local.h"

namespace rgl {
	using thread_routine_t = void* (*)(void*);
	
	namespace internal{
		struct __rgl_DestructorEntry {
        	void (*destructor)(void*);
        	void* arg;
        	void* dso_handle;
    	};

		struct thread_control_block{
			thread_control_block* self;
			thread_cache my_cache;
			int32_t tid;
			int32_t tgid;
			void* stack_base;
			size_t stack_size;

			thread_routine_t routine;
			void* arg;
			void* retval;

			int detach_state;
			atomic<int32_t> state;
			atomic<int32_t> jf;

			__rgl_DestructorEntry thread_dtor_table[RGL_MAX_THREAD_DESTRUCTORS];
			uint32_t thread_dtor_count = 0;
			void* raw_tls_allocation;

			thread_control_block(rgl::uint32_t id) : my_cache(id) {}
			~thread_control_block() = default;
		};
		thread_control_block* get_control_block();

		void thread_trampoline();
	}
}//namesapace rgl