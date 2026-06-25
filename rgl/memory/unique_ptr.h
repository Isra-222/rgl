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


//memory
#pragma once

#include "rgl/core/utility/move.h"
#include "new.h"

namespace rgl {
    template<typename T>
    class unique_ptr final {
        T* ptr;
    public:
        explicit unique_ptr(T* p = nullptr) : ptr(p) {}
        ~unique_ptr() { delete ptr; }

        unique_ptr(const unique_ptr&) = delete;
        unique_ptr& operator=(const unique_ptr&) = delete;
        unique_ptr(unique_ptr&& other) noexcept : ptr(other.ptr) { other.ptr = nullptr; }
        unique_ptr& operator=(unique_ptr&& other) noexcept {
            if (this != &other) { delete ptr; ptr = other.ptr; other.ptr = nullptr; }
            return *this;
        }

        T* operator->() const { return ptr; }
        T& operator*() const { return *ptr; }
        T* get() const { return ptr; }
        explicit operator bool() const noexcept {
            return ptr != nullptr;
        }
    };

    template<typename T>
    class unique_ptr<T[]> final {
        T* ptr;
    public:
        explicit unique_ptr(T* p = nullptr) : ptr(p) {}
        ~unique_ptr() { delete[] ptr; }

        unique_ptr(const unique_ptr&) = delete;
        unique_ptr& operator=(const unique_ptr&) = delete;
        unique_ptr(unique_ptr&& other) noexcept : ptr(other.ptr) { other.ptr = nullptr; }
        
        unique_ptr& operator=(unique_ptr&& other) noexcept {
            if (this != &other) { delete[] ptr; ptr = other.ptr; other.ptr = nullptr; }
            return *this;
        }

        T& operator[](size_t i) const { return ptr[i]; }
        T* get() const { return ptr; }
        explicit operator bool() const noexcept {
            return ptr != nullptr;
        }
    };

    template<typename T, typename... Args>
    unique_ptr<T> make_unique(Args&&... args) {
       return unique_ptr<T>(new T(rgl::forward<Args>(args)...));
    }
    template<typename T>
    unique_ptr<T[]> make_unique(size_t size){
        return unique_ptr<T[]>(new T[size]);
    }
};