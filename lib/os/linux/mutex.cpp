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
#include "rgl/os/mutex.h"
#include "rgl/os/sys/futex.h"

#include "rgl/os/sys/time.h"

#include "rgl/os/pthread.h"
#include "rgl/extra/pass.h"

#include "rgl/core/limits.h"

using namespace rgl;

//mutex

int sys::pthread_mutex_lock(pthread_mutex_t* mutex){
	if(!mutex) return -1;

	if(mutex->__kind == mutex_kind::recursive && mutex->__owner == pthread_self().handle->tid){
		mutex->__count++;
		return 0;
	}

	int32_t expected = 0;
	do{
		expected = 0;
		relax();
	}while(!mutex->__lock.compare_exchange_weak(
		expected, 1,
		memory_order_acquire, memory_order_relaxed
	));

	mutex->__owner = pthread_self().handle->tid;
	mutex->__count = 1;

	return 0;
}
int sys::pthread_mutex_trylock(pthread_mutex_t* mutex){
	if(!mutex) return -1;

	if (mutex->__kind == mutex_kind::recursive && mutex->__owner == pthread_self().handle->tid) {
        mutex->__count++;
        return 0;
    }
    int32_t expected = 0;

	if(!mutex->__lock.
		compare_exchange_weak(expected, 1, memory_order_acquire, memory_order_relaxed)){
		return -1;
	}


	mutex->__owner = pthread_self().handle->tid;
	mutex->__count = 1;

	return 0;
}
int sys::pthread_mutex_unlock(pthread_mutex_t* mutex){
	if(!mutex) return -1;

	if (mutex->__kind == mutex_kind::recursive && mutex->__owner == pthread_self().handle->tid) {
        mutex->__count--;
        if(mutex->__count > 0) return 0;
    }
    mutex->__owner = -1;
    mutex->__count = 0;

    mutex->__lock.store(0, memory_order_release);

    return 0;
}
int sys::pthread_mutex_destroy(pthread_mutex_t* mutex){
	if(!mutex) return -1;

	if(mutex->__lock.load(memory_order_relaxed) > 0)
		return -1;

	/*
	 * Let the ceremony consecrate the marriage
	 * Let me be the protégé of five magics
	 * Give me alchemy, give me wizardry
	 * Give me sorcery, thermatology
	 * Electricity, magic if you please
	 * Master all of these, bring him to his knees
	 * -666
	*/
	mutex->__lock.store(-666, memory_order_release);

	mutex->__owner = -1;
	mutex->__count = 0;

	return 0;
}

//COND-----

int sys::pthread_cond_signal(pthread_cond_t* cond){
	if(!cond) return -1;

	cond->__sequence.fetch_add(1, memory_order_release);

	if (cond->__waiters.load(memory_order_relaxed) > 0)
		sys::futex_wake(&cond->__sequence, 1);

	return 0;
}
int sys::pthread_cond_broadcast(pthread_cond_t* cond){
	if(!cond) return -1;

	cond->__sequence.fetch_add(1, memory_order_release);

	if (cond->__waiters.load(memory_order_relaxed) > 0)
		sys::futex_wake(&cond->__sequence, numeric_limits<int>::max());

	return 0;
}
int sys::pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex){
	if(!cond || !mutex) return -1;

	uint32_t seq = cond->__sequence.load(memory_order_relaxed);
	cond->__waiters.fetch_add(1, memory_order_relaxed);

	pthread_mutex_unlock(mutex);
	sys::futex_wait(&cond->__sequence, seq);

	cond->__waiters.fetch_sub(1, memory_order_relaxed);
	pthread_mutex_lock(mutex);

	return 0;
}
int sys::pthread_cond_timedwait(pthread_cond_t* cond, pthread_mutex_t* mutex, const timespec* abstime){
	if(!cond || !mutex) return -1;

	if(!abstime) return pthread_cond_wait(cond, mutex);

	uint32_t seq = cond->__sequence.load(memory_order_relaxed);
    cond->__waiters.fetch_add(1, memory_order_relaxed);

    pthread_mutex_unlock(mutex);
	long res = sys::futex_timedwait(&cond->__sequence, seq, abstime);

	cond->__waiters.fetch_sub(1, memory_order_relaxed);
	pthread_mutex_lock(mutex);

	if(res < 0) return -1;

	return 0;
}
int sys::pthread_cond_destroy(pthread_cond_t* cond){
	if(!cond) return -1;

	if (cond->__waiters.load(memory_order_relaxed) > 0) return -1;

	cond->__sequence.store(0, memory_order_relaxed);
    cond->__waiters.store(0, memory_order_relaxed);

	return 0;
}

