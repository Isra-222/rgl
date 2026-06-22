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

#include <pthread.h>

namespace rgl {

    class mutex final{
    public:
        using native_handle_type = pthread_mutex_t*;

        mutex() {
            pthread_mutex_init(&mutex_, nullptr);
        }

        ~mutex() {
            pthread_mutex_destroy(&mutex_);
        }

        mutex(const mutex&) = delete;
        mutex& operator=(const mutex&) = delete;

        void lock() noexcept {
            pthread_mutex_lock(&mutex_);
        }

        void unlock() noexcept {
            pthread_mutex_unlock(&mutex_);
        }

        native_handle_type native_handle() noexcept {
            return &mutex_;
        }

    private:
        pthread_mutex_t mutex_;
    };

    class condition_variable final {
    public:
        using native_handle_type = pthread_cond_t*;

        condition_variable() {
            pthread_cond_init(&cond_, nullptr);
        }

        ~condition_variable() {
            pthread_cond_destroy(&cond_);
        }

        condition_variable(const condition_variable&) = delete;
        condition_variable& operator=(const condition_variable&) = delete;

        void wait(mutex& m) noexcept {
            pthread_cond_wait(&cond_, m.native_handle());
        }

        void notify_one() noexcept {
            pthread_cond_signal(&cond_);
        }

        void notify_all() noexcept {
            pthread_cond_broadcast(&cond_);
        }

        native_handle_type native_handle() noexcept {
            return &cond_;
        }

    private:
        pthread_cond_t cond_;
    };

} // namespace rgl