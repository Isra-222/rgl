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

#include "rgl/os/sys/time.h"
#include "rgl/math/ratio.h"

namespace rgl::anemo {
    inline constexpr int ETIMEDOUT = 110;
    
    //durations
    template<typename Rep, typename Period = rgl::math::ratio<1>>
    class Duration final {
        Rep ticks_;
    public:
        constexpr Duration() : ticks_(0) {}
        constexpr explicit Duration(const Rep& ticks) : ticks_(ticks) {}
        
        [[nodiscard]] constexpr Rep count() const { return ticks_; }

        using rep = Rep;
        using period = Period;

        constexpr Duration operator+(const Duration& rhs) const {
            return Duration(ticks_ + rhs.ticks_);
        }

        constexpr Duration operator-(const Duration& rhs) const {
            return Duration(ticks_ - rhs.ticks_);
        }

        Duration& operator+=(const Duration& rhs) {
            ticks_ += rhs.ticks_;
            return *this;
        }

        Duration& operator-=(const Duration& rhs) {
            ticks_ -= rhs.ticks_;
            return *this;
        }
    };
    template<typename ToDuration, typename Rep, typename Period>
    constexpr ToDuration duration_cast(const Duration<Rep, Period>& d) {
        using ToRep = typename ToDuration::rep;
        constexpr ssize_t num = Period::num * ToDuration::period::den;
        constexpr ssize_t den = Period::den * ToDuration::period::num;
        
        if constexpr (num >= den && (num % den == 0)) {
            return ToDuration(static_cast<ToRep>(d.count() * (num / den)));
        } else if constexpr (num < den && (den % num == 0)) {
            return ToDuration(static_cast<ToRep>(d.count() / (den / num)));
        } else {
            return ToDuration(static_cast<ToRep>(d.count() * num / den));
        }
    }

    using nanoseconds = Duration<int64_t, rgl::math::ratio<1, 1000000000>>;
    using microseconds = Duration<int64_t, rgl::math::ratio<1, 1000000>>;
    using milliseconds = Duration<int64_t, rgl::math::ratio<1, 1000>>;

    using seconds = Duration<int64_t, rgl::math::ratio<1, 1>>;

    using minutes = Duration<int64_t, rgl::math::ratio<60, 1>>;
    using hours = Duration<int64_t, rgl::math::ratio<3600, 1>>;

    //clocks
    template<typename Clock, typename Duration>
    class time_point final {
        Duration duration_;
    public:
        constexpr time_point() : duration_(0) {}
        constexpr explicit time_point(const Duration& d) : duration_(d) {}

        constexpr Duration time_since_epoch() const { return duration_; }
        
        constexpr time_point operator+(const Duration& d) const {
            return time_point(duration_ + d);
        }

        constexpr time_point operator-(const Duration& d) const {
            return time_point(duration_ - d);
        }

        time_point& operator+=(const Duration& d) {
            duration_ += d;
            return *this;
        }

        time_point& operator-=(const Duration& d) {
            duration_ -= d;
            return *this;
        }

        constexpr Duration operator-(const time_point& rhs) const {
            return duration_ - rhs.duration_;
        }
    };

    class system_clock final{
    public:
        using rep = int64_t;
        using period = rgl::math::ratio<1, 1000000000>; 
        using duration = Duration<rep, period>;
        using time_point = rgl::anemo::time_point<system_clock, duration>;

        static time_point now() noexcept {
            sys::timespec ts;
            clock_gettime(sys::CLOCK_REALTIME, &ts);
            int64_t total_ns = (ts.tv_sec * 1000000000LL) + ts.tv_nsec;
            return time_point(duration(total_ns));
        }
    };

    class steady_clock final{
    public:
        using rep = int64_t;
        using period = rgl::math::ratio<1, 1000000000>;
        using duration = Duration<rep, period>;
        using time_point = rgl::anemo::time_point<steady_clock, duration>;

        static time_point now() noexcept {
            sys::timespec ts;
            clock_gettime(sys::CLOCK_MONOTONIC, &ts);
            
            int64_t total_ns = (ts.tv_sec * 1000000000LL) + ts.tv_nsec;
            
            return time_point(duration(total_ns));
        }
    };

    inline sys::timespec to_abstime(const typename system_clock::time_point& tp) noexcept{
        auto ns = duration_cast<nanoseconds>(tp.time_since_epoch());
        int64_t total_ns = ns.count();

        sys::timespec ts;
        ts.tv_sec = static_cast<int64_t>(total_ns / 1000000000LL);
        ts.tv_nsec = static_cast<int64_t>(total_ns % 1000000000LL);
        return ts;
    }

    template<typename Rep, typename Period>
    inline sys::timespec to_abstime_from_now(const Duration<Rep, Period>& rel_time) noexcept {
        auto now_tp = system_clock::now();
        auto sys_rel_duration = duration_cast<typename system_clock::duration>(rel_time);
        auto future_tp = now_tp + sys_rel_duration;
        
        return to_abstime(future_tp);
    }
}//namespace rgl::anemo