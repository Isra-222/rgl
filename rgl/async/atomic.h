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

//atomic
#pragma once

namespace rgl {

    enum class memory_order : int {
        relaxed,
        consume,
        acquire,
        release,
        acq_rel,
        seq_cst
    };

    inline constexpr memory_order memory_order_relaxed = memory_order::relaxed;
    inline constexpr memory_order memory_order_consume = memory_order::consume;
    inline constexpr memory_order memory_order_acquire = memory_order::acquire;
    inline constexpr memory_order memory_order_release = memory_order::release;
    inline constexpr memory_order memory_order_acq_rel = memory_order::acq_rel;
    inline constexpr memory_order memory_order_seq_cst = memory_order::seq_cst;

    template<typename T>
    class atomic {
    private:
        T value_;

    public:
        atomic() noexcept = default;
        constexpr atomic(T desired) noexcept : value_(desired) {}

        atomic(const atomic&) = delete;
        atomic& operator=(const atomic&) = delete;
        atomic& operator=(const atomic&) volatile = delete;

        T load(memory_order order = memory_order::seq_cst) const noexcept {
            return __atomic_load_n(&value_, static_cast<int>(order));
        }

        operator T() const noexcept {
            return load();
        }

        void store(T desired, memory_order order = memory_order::seq_cst) noexcept {
            __atomic_store_n(&value_, desired, static_cast<int>(order));
        }

        T operator=(T desired) noexcept {
            store(desired);
            return desired;
        }

        T exchange(T desired, memory_order order = memory_order::seq_cst) noexcept {
            return __atomic_exchange_n(&value_, desired, static_cast<int>(order));
        }

        bool compare_exchange_weak(T& expected, T desired, 
                                   memory_order success, memory_order failure) noexcept {
            return __atomic_compare_exchange_n(&value_, &expected, desired, true, 
                                               static_cast<int>(success), static_cast<int>(failure));
        }

        bool compare_exchange_weak(T& expected, T desired, 
                                   memory_order order = memory_order::seq_cst) noexcept {
            return compare_exchange_weak(expected, desired, order, order);
        }

        bool compare_exchange_strong(T& expected, T desired, 
                                     memory_order success, memory_order failure) noexcept {
            return __atomic_compare_exchange_n(&value_, &expected, desired, false, 
                                               static_cast<int>(success), static_cast<int>(failure));
        }

        bool compare_exchange_strong(T& expected, T desired, 
                                     memory_order order = memory_order::seq_cst) noexcept {
            return compare_exchange_strong(expected, desired, order, order);
        }
        T fetch_add(T operand, memory_order order = memory_order::seq_cst) noexcept {
            return __atomic_fetch_add(&value_, operand, static_cast<int>(order));
        }
    };

} // namespace rgl