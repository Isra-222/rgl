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

#include "string.h"
#include "rgl/core/types.h"
#include "rgl/core/traits.h"
#include "rgl/core/limits.h"

namespace rgl {

    namespace detail{
        template<typename uT>
        uT parse_unsigned(const char* str, size_t len, size_t& consumed, bool& overflow) {
            overflow = false;
            uT result = 0;
            size_t i = 0;
            const uT maxVal = static_cast<uT>(-1);

            for (; i < len; ++i) {
                unsigned char c = static_cast<unsigned char>(str[i]);
                unsigned d = c - '0';
                if (d > 9u) break;

                if (result > (maxVal - d) / 10u) {
                    overflow = true;
                    result = maxVal;
                    for (++i; i < len; ++i) {
                        unsigned char cc = static_cast<unsigned char>(str[i]);
                        if (static_cast<unsigned>(cc - '0') > 9u) break;
                    }
                    break;
                }
                result = static_cast<uT>(result * 10u + d);
            }
            consumed = i;
            return result;
        }
        template<typename T>
        T to_signed(const char* str, size_t len) {
            if (!str || len == 0) return 0;

            bool negative = false;
            size_t i = 0;
            if (str[0] == '-') { negative = true; i = 1; }
            else if (str[0] == '+') { i = 1; }

            size_t consumed = 0;
            bool overflow = false;
            using uT = make_unsigned_t<T>;

            uT uval = parse_unsigned<uT>(str + i, len - i, consumed, overflow);
            if (overflow)
                return negative ? numeric_limits<T>::min() : numeric_limits<T>::max();

            if (negative) {
                constexpr uT minAbs = static_cast<uT>(numeric_limits<T>::max()) + 1u;
                if (uval > minAbs) return numeric_limits<T>::min();
                if (uval == minAbs) return numeric_limits<T>::min();
                return static_cast<T>(-static_cast<T>(uval));
            }
            if (uval > static_cast<uT>(numeric_limits<T>::max()))
                return numeric_limits<T>::max();
            return static_cast<T>(uval);
        }
        template<typename T>
        T to_unsigned(const char* str, size_t len) {
            if (!str || len == 0) return 0;

            size_t i = 0;
            if (str[0] == '+') i = 1;
            else if (str[0] == '-') return 0;

            size_t consumed = 0;
            bool overflow = false;
            T r = parse_unsigned<T>(str + i, len - i, consumed, overflow);
            return overflow ? static_cast<T>(-1) : r;
        }
        template<typename T>
        T to_decimal(const char* str, size_t len) {
            if (!str || len == 0) return T(0);

            bool negative = false;
            size_t i = 0;
            if (str[0] == '-') { negative = true; i = 1; }
            else if (str[0] == '+') { i = 1; }

            uint64_t mantissa = 0;
            int exp10 = 0;
            bool afterPoint = false;
            bool any = false;

            for (; i < len; ++i) {
                char c = str[i];
                if (c == '.') {
                    if (afterPoint) break;
                    afterPoint = true;
                    continue;
                }
                unsigned d = static_cast<unsigned>(c - '0');
                if (d > 9u) break;
                any = true;

                if (mantissa > (numeric_limits<uint64_t>::max() - d) / 10u) {
                    if (!afterPoint) ++exp10;
                    continue;
                }
                mantissa = mantissa * 10u + d;
                if (afterPoint) ++exp10;
            }

            if (!any) return T(0);

            T result = static_cast<T>(mantissa);
            static const T pow10[] = { T(1), T(1e1), T(1e2), T(1e3), T(1e4),
                                       T(1e5), T(1e6), T(1e7), T(1e8), T(1e9) };
            while (exp10 > 9) { result /= T(1e9); exp10 -= 9; }
            if (exp10 > 0) result /= pow10[exp10];

            return negative ? -result : result;
        }
        template<typename T>
        string to_string_signed(T num) {
            using uT = make_unsigned_t<T>;

            char buffer[24];
            char* p = buffer + sizeof(buffer);
            *--p = '\0';

            bool neg = num < 0;

            uT uval = neg ? uT(0) - static_cast<uT>(num) : static_cast<uT>(num);

            do {
                *--p = char('0' + (uval % 10u));
                uval /= 10u;
            } while (uval);

            if (neg) *--p = '-';

            return string(p);
        }
        template<typename T>
        string to_string_unsigned(T num) {
            char buffer[24];
            char* p = buffer + sizeof(buffer);
            *--p = '\0';

            do {
                *--p = char('0' + (num % 10u));
                num /= 10u;
            } while (num);

            return string(p);
        }
        template<typename T>
        string to_string_decimal(T num) {
            if (num == T(0)) return "0.0";

            char buffer[64];
            char* p = buffer + sizeof(buffer);
            *--p = '\0';

            bool neg = num < 0;
            if (neg) num = -num;

            uint64_t ip = static_cast<uint64_t>(num);
            T frac = num - static_cast<T>(ip);

            constexpr int FRAC = 6;
            for (int k = 0; k < FRAC; ++k) frac *= T(10);
            uint64_t fp = static_cast<uint64_t>(frac + T(0.5));
            if (fp >= 1000000ULL) { fp -= 1000000ULL; ip += 1; }

            for (int k = 0; k < FRAC; ++k) {
                *--p = char('0' + (fp % 10));
                fp /= 10;
            }
            *--p = '.';

            do {
                *--p = char('0' + (ip % 10));
                ip /= 10;
            } while (ip);

            if (neg) *--p = '-';
            return string(p);
        }
    }//namespace

    // (16-bit)
    inline int16_t to_int16(const char* str, size_t len) { return detail::to_signed<int16_t>(str, len); }
    inline int16_t to_int16(const string_view str) { return to_int16(str.data(), str.length()); }
    inline int16_t to_int16(const string& str) { return to_int16(str.data(), str.length()); }
    inline int16_t to_int16(const char* str) { return to_int16(str, (str) ? strlen(str) : 0); }

    inline uint16_t to_uint16(const char* str, size_t len) { return detail::to_unsigned<uint16_t>(str, len); }
    inline uint16_t to_uint16(const string_view str) { return detail::to_unsigned<uint16_t>(str.data(), str.length()); }
    inline uint16_t to_uint16(const string& str) { return to_uint16(str.data(), str.length()); }
    inline uint16_t to_uint16(const char* str) { return to_uint16(str, (str) ? strlen(str) : 0); }


    // (32-bit)
    inline int32_t to_int32(const char* str, size_t len) { return detail::to_signed<int32_t>(str, len); }
    inline int32_t to_int32(const string_view str) { return detail::to_signed<int32_t>(str.data(), str.length()); }
    inline int32_t to_int32(const string& str) { return to_int32(str.data(), str.length()); }
    inline int32_t to_int32(const char* str) { return to_int32(str, (str) ? strlen(str) : 0); }

    inline uint32_t to_uint32(const char* str, size_t len)  { return detail::to_unsigned<uint32_t>(str, len); }
    inline uint32_t to_uint32(const string_view str) { return detail::to_unsigned<uint32_t>(str.data(), str.length()); }
    inline uint32_t to_uint32(const string& str) { return to_uint32(str.data(), str.length()); }
    inline uint32_t to_uint32(const char* str) { return to_uint32(str, (str) ? strlen(str) : 0); }


    // (64-bit)
    inline int64_t to_int64(const char* str, size_t len) { return detail::to_signed<int64_t>(str, len); }
    inline int64_t to_int64(const string_view str) { return detail::to_signed<int64_t>(str.data(), str.length()); }
    inline int64_t to_int64(const string& str) { return to_int64(str.data(), str.length()); }
    inline int64_t to_int64(const char* str) { return to_int64(str, (str) ? strlen(str) : 0); }

    inline uint64_t to_uint64(const char* str, size_t len)  { return detail::to_unsigned<uint64_t>(str, len); }
    inline uint64_t to_uint64(const string_view str) { return detail::to_unsigned<uint64_t>(str.data(), str.length()); }
    inline uint64_t to_uint64(const string& str) { return to_uint64(str.data(), str.length()); }
    inline uint64_t to_uint64(const char* str) { return to_uint64(str, (str) ? strlen(str) : 0); }


    // (Floats)
    inline float to_float(const char* str, size_t len) { return detail::to_decimal<float>(str, len); }
    inline float to_float(const string_view str) { return to_float(str.data(), str.length()); }
    inline float to_float(const string& str) { return to_float(str.data(), str.length()); }
    inline float to_float(const char* str) { return to_float(str, (str) ? strlen(str) : 0); }

    inline double to_double(const char* str, size_t len) { return detail::to_decimal<double>(str, len); }
    inline double to_double(const string_view str) { return to_double(str.data(), str.length()); }
    inline double to_double(const string& str) { return to_double(str.data(), str.length()); }
    inline double to_double(const char* str) { return to_double(str, (str) ? strlen(str) : 0); }

    inline long double to_long_double(const char* str, size_t len) { return detail::to_decimal<long double>(str, len); }
    inline long double to_long_double(const string_view str) { return to_long_double(str.data(), str.length()); }
    inline long double to_long_double(const string& str) { return to_long_double(str.data(), str.length()); }
    inline long double to_long_double(const char* str) { return to_long_double(str, (str) ? strlen(str) : 0); }


    inline string to_string(int16_t num) { return detail::to_string_signed<int16_t>(num); }
    inline string to_string(uint16_t num) { return detail::to_string_unsigned<uint16_t>(num); }
    inline string to_string(int32_t num) { return detail::to_string_signed<int32_t>(num); }
    inline string to_string(uint32_t num) { return detail::to_string_unsigned<uint32_t>(num); }
    inline string to_string(int64_t num) { return detail::to_string_signed<int64_t>(num); }
    inline string to_string(uint64_t num) { return detail::to_string_unsigned<uint64_t>(num); }
    inline string to_string(float num) { return detail::to_string_decimal<float>(num);}
    inline string to_string(double num) { return detail::to_string_decimal<double>(num);}
    inline string to_string(long double num) { return detail::to_string_decimal<long double>(num);}
    inline string to_string(bool b){ return b? string("true") : string("false"); }
    template<typename T>
    string to_string(T* ptr){ return ptr? to_string(*ptr) : string("nullptr"); }
}//namespace rgl