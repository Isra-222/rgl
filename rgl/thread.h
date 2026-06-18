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

//thread
#pragma once

#include <pthread.h>
#include <sched.h>

#include "abort.h"
#include "utility.h"
#include "types.h"
#include "anemo.h"

namespace rgl {
    class thread {
    public:
        using native_handle_type = pthread_t;
    private:
        native_handle_type handle_;
        bool joined_;

        struct thread_state_base {
            virtual ~thread_state_base() = default;
            virtual void run() = 0;
        };

        template <typename F, typename... Args>
        struct thread_state : public thread_state_base {
            typename decay<F>::type func;
            tuple<typename decay<Args>::type...> args;

        template<typename F_, typename... Args_>
        thread_state(F_&& f, Args_&&... a) 
        : func(rgl::move(f)), args(rgl::move(a)...) {}

            void run() override {
                apply(move(func), move(args));
            }
        };

        static void* thread_routine(void* arg) {
            auto* state = static_cast<thread_state_base*>(arg);
            state->run();
            delete state;
            return nullptr;
        }

    public:
        thread(const thread&) = delete;
        thread& operator=(const thread&) = delete;

        thread(thread&& other) noexcept
            : handle_(other.handle_), joined_(other.joined_) {
            other.handle_ = 0;
            other.joined_ = false;
        }

        thread& operator=(thread&& other) noexcept {
            if (this != &other) {
                if (joinable()) abort();

                handle_ = other.handle_;
                joined_ = other.joined_;
                other.handle_ = 0;
                other.joined_ = false;
            }
            return *this;
        }

        ~thread() {
            if (joinable()) abort();
        }

        thread() noexcept : handle_(0), joined_(false) {}

        template<typename F, typename... Args>
        explicit thread(F&& f, Args&&... args) : joined_(false) {
            using state_type = thread_state<decay_t<F>, decay_t<Args>...>;
            auto* state = new state_type(
                forward<F>(f), 
                forward<Args>(args)...
            );

            if (pthread_create(&handle_, nullptr, thread_routine, state) != 0) {
                delete state;
                abort();
            }
        }

        [[nodiscard]] bool joinable() const noexcept {
            return handle_ != 0;
        }

        void join() {
            pthread_join(handle_, nullptr);
            joined_ = true;
            handle_ = 0;
        }

        void detach() {
            pthread_detach(handle_);
            handle_ = 0;
        }

        native_handle_type native_handle() noexcept {
            return handle_;
        }
    };

	namespace this_thread {
        
        template<typename Rep, typename Period>
        inline void sleep_for(const anemo::Duration<Rep, Period>& d) noexcept {
            auto ns = anemo::duration_cast<anemo::nanoseconds>(d);
            
            long long total_ns = ns.count();
            time_t sec = total_ns / 1000000000LL;
            long nsec = total_ns % 1000000000LL;
            
            struct timespec ts;
            ts.tv_sec = sec;
            ts.tv_nsec = nsec;
            
            nanosleep(&ts, nullptr);
        }

        template<typename Clock, typename Duration>
        inline void sleep_until(const anemo::time_point<Clock, Duration>& t) noexcept {
            auto now = Clock::now();

            auto delta = t - now;

            if (delta.count() > 0) {
                sleep_for(delta);
            }
        }

        inline void yield() noexcept {
            sched_yield();
        }

        inline uint64_t get_id() noexcept {
            return static_cast<uint64_t>(pthread_self());
        }
    }
}