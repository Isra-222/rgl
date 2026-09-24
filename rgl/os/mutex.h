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
#ifndef RGL_OS_MUTEX_H
#define RGL_OS_MUTEX_H

#include "rgl/core/types.h"
#include "rgl/async/atomic.h"

namespace rgl::sys{

	enum class mutex_kind : int32_t {
		normal = 0,
		recursive = 1,
		errorcheck = 2
	};

	struct alignas(64) pthread_mutex_t {
		atomic<int32_t> __lock;
		uint32_t __count;
		int32_t __owner;
		mutex_kind __kind;

		constexpr pthread_mutex_t(mutex_kind kind) noexcept
		: __lock(0), __count(0), __owner(-1), __kind(kind) {}
	};
	struct alignas(64) pthread_cond_t {
		atomic<int32_t> __sequence;
		atomic<int32_t> __waiters;

		constexpr pthread_cond_t() noexcept : __sequence(0), __waiters(0) {}
	};


	inline constexpr pthread_mutex_t fastmutex(mutex_kind::normal);
	inline constexpr pthread_mutex_t recmutex(mutex_kind::recursive);
	inline constexpr pthread_mutex_t errchkmutex(mutex_kind::errorcheck);
	inline constexpr pthread_cond_t  cond;

	
	inline int pthread_mutex_init(pthread_mutex_t* mutex, const void* /*attr*/ = nullptr){
		if(!mutex) return -1;
		mutex->__lock.store(0);
        mutex->__count = 0;
        mutex->__owner = -1;
        mutex->__kind = mutex_kind::normal;
        return 0;
	}

	int pthread_mutex_lock(pthread_mutex_t* mutex);


	int pthread_mutex_trylock(pthread_mutex_t* mutex);
	int pthread_mutex_unlock(pthread_mutex_t* mutex);
	int pthread_mutex_destroy(pthread_mutex_t* mutex);
	   
	//conditions
	inline int pthread_cond_init(pthread_cond_t* cond, void* /*cond_attr*/){
		if(!cond) return -1;
		cond->__sequence.store(0);
        cond->__waiters.store(0);
		return 0;
	}

	int pthread_cond_signal(pthread_cond_t* cond);
	int pthread_cond_broadcast(pthread_cond_t* cond);
	int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex);

	struct timespec;
	int pthread_cond_timedwait(pthread_cond_t* cond, pthread_mutex_t* mutex, const timespec* abstime);
	
	int pthread_cond_destroy(pthread_cond_t* cond);

}//namespace rgl::sys

#endif