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

//mutex
#pragma once

#include "rgl/os/mutex.h"
#include "rgl/time/anemo.h"

namespace rgl {
    class mutex final{
        sys::pthread_mutex_t mutex_;
    public:
        //pthread_mutex_init(&mutex_, nullptr);
        mutex() : mutex_(sys::mutex_kind::normal) {}
        mutex(sys::mutex_kind kind) : mutex_(kind) {}

        ~mutex() { pthread_mutex_destroy(&mutex_); }

        mutex(const mutex&) = delete;
        mutex& operator=(const mutex&) = delete;

        void trylock() noexcept { pthread_mutex_trylock(&mutex_); }
        void lock() noexcept { pthread_mutex_lock(&mutex_); }
        void unlock() noexcept { pthread_mutex_unlock(&mutex_); }

        sys::pthread_mutex_t* native_handle() noexcept { return &mutex_; }
    };

    class condition_variable final {
        sys::pthread_cond_t cond_;
    public:
        enum class CvStatus {
            no_timeout,
            timeout
        };

        //pthread_cond_init(&cond_, nullptr);
        condition_variable() = default;

        ~condition_variable() { pthread_cond_destroy(&cond_); }

        condition_variable(const condition_variable&) = delete;
        condition_variable& operator=(const condition_variable&) = delete;

        void notify_one() noexcept { pthread_cond_signal(&cond_); }
        void notify_all() noexcept { pthread_cond_broadcast(&cond_); }

        template<typename Predicate>
        void wait(mutex& m, Predicate pred) noexcept { while(!pred()) pthread_cond_wait(&cond_, m.native_handle()); }
        
        template<typename Rep, typename Period>
        CvStatus wait_for(mutex& m, const anemo::Duration<Rep, Period>& rel_time) {
            auto abs_time = anemo::to_abstime(rel_time);
            
            int res = sys::pthread_cond_timedwait(&cond_, m.native_handle(), &abs_time);
            return (res == anemo::ETIMEDOUT) ? CvStatus::timeout : CvStatus::no_timeout;
        }

        template<typename Rep, typename Period>
        int wait_for(mutex& m, const anemo::Duration<Rep, Period>& rel_time) {
            sys::timespec abstime = anemo::to_abstime_from_now(rel_time);

            return pthread_cond_timedwait(&cond_, m.native_handle(), &abstime);
        }

        template<typename Clock, typename Duration>
        int wait_until(mutex& m, const anemo::time_point<Clock, Duration>& timeout_time) {
            auto sys_tp = anemo::duration_cast<anemo::system_clock::duration>(timeout_time.time_since_epoch());
            anemo::system_clock::time_point tp_sys(sys_tp);
            
            sys::timespec abstime = anemo::to_abstime(tp_sys);
            return pthread_cond_timedwait(&cond_, m.native_handle(), &abstime);
        }

        
        sys::pthread_cond_t* native_handle() noexcept { return &cond_; }
    };

} // namespace rgl