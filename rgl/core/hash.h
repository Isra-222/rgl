/*
 * rapidhash V3 - Very fast, high quality, platform-independent hashing algorithm.
 *
 * Based on 'wyhash', by Wang Yi <godspeed_china@yeah.net>
 * 
 * Copyright (C) 2025 Nicolas De Carli
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * You can contact the author at:
 *   - rapidhash source repository: https://github.com/Nicoshev/rapidhash
 */

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

//hash.h
#pragma once

#include "types.h"
#include "utility/memory.h"
#include "traits.h"

namespace rgl::hash {
    namespace detail {
        #ifndef RGL_SECRET
            inline constexpr uint64_t rapid_secret[8] = {
                0x2d358dccaa6c78a5ull,
                0x8bb84b93962eacc9ull,
                0x4b33a62ed433d4a3ull,
                0x4d5a2da51de1aa47ull,
                0xa0761d6478bd642full,
                0xe7037ed1a0b428dbull,
                0x90ed1765281c388cull,
                0xaaaaaaaaaaaaaaaaull
            };
            inline constexpr const uint64_t* rgl_secret = rapid_secret;
        #else
            extern const uint64_t rgl_secret[8];
        #endif

        inline constexpr void rapid_mum(uint64_t *A, uint64_t *B) noexcept {
            #if defined(__SIZEOF_INT128__)
                __uint128_t r = *A; r *= *B;
                *A = (uint64_t)r; *B = (uint64_t)(r >> 64);
            #else
                uint64_t ha = *A >> 32, hb = *B >> 32, la = (uint32_t)*A, lb = (uint32_t)*B;
                uint64_t rh = ha * hb, rm0 = ha * lb, rm1 = hb * la, rl = la * lb, t = rl + (rm0 << 32), c = t < rl;
                uint64_t lo = t + (rm1 << 32); 
                c += lo < t; 
                uint64_t hi = rh + (rm0 >> 32) + (rm1 >> 32) + c;
                *A = lo; *B = hi;
            #endif
        }
        inline constexpr uint64_t rapid_mix(uint64_t A, uint64_t B) noexcept{ rapid_mum(&A,&B); return A^B; }
        inline constexpr uint64_t rapid_read64(const uint8_t *p) noexcept { uint64_t v = 0; memcpy(&v, p, sizeof(uint64_t)); return v; }
        inline constexpr uint64_t rapid_read32(const uint8_t *p) noexcept { uint32_t v = 0; memcpy(&v, p, sizeof(uint32_t)); return v; }

        inline constexpr uint64_t rapidhash_internal(const uint8_t* p, size_t len, uint64_t seed, const uint64_t* secret) noexcept {
            seed ^= rapid_mix(seed ^ secret[2], secret[1]);
            uint64_t a=0, b=0;
            size_t i = len;
            if (len <= 16) [[likely]] {
              if (len >= 4) {
                seed ^= len;
                if (len >= 8) {
                  const uint8_t* plast = p + len - 8;
                  a = rapid_read64(p);
                  b = rapid_read64(plast);
                } else {
                  const uint8_t* plast = p + len - 4;
                  a = rapid_read32(p);
                  b = rapid_read32(plast);
                }
              } else if (len > 0) {
                a = (((uint64_t)p[0])<<45)|p[len-1];
                b = p[len>>1];
              } else
                a = b = 0;
            } 
            else {
                if (len > 112) {
                    uint64_t see1 = seed, see2 = seed;
                    uint64_t see3 = seed, see4 = seed;
                    uint64_t see5 = seed, see6 = seed;
                    do {
                        seed = rapid_mix(rapid_read64(p) ^ secret[0], rapid_read64(p + 8) ^ seed);
                        see1 = rapid_mix(rapid_read64(p + 16) ^ secret[1], rapid_read64(p + 24) ^ see1);
                        see2 = rapid_mix(rapid_read64(p + 32) ^ secret[2], rapid_read64(p + 40) ^ see2);
                        see3 = rapid_mix(rapid_read64(p + 48) ^ secret[3], rapid_read64(p + 56) ^ see3);
                        see4 = rapid_mix(rapid_read64(p + 64) ^ secret[4], rapid_read64(p + 72) ^ see4);
                        see5 = rapid_mix(rapid_read64(p + 80) ^ secret[5], rapid_read64(p + 88) ^ see5);
                        see6 = rapid_mix(rapid_read64(p + 96) ^ secret[6], rapid_read64(p + 104) ^ see6);
                        p += 112;
                        i -= 112;
                    } while(i > 112);
                    seed ^= see1;
                    see2 ^= see3;
                    see4 ^= see5;
                    seed ^= see6;
                    see2 ^= see4;
                    seed ^= see2;
                }
                if (i > 16) {
                    seed = rapid_mix(rapid_read64(p) ^ secret[2], rapid_read64(p + 8) ^ seed);
                    if (i > 32) {
                        seed = rapid_mix(rapid_read64(p + 16) ^ secret[2], rapid_read64(p + 24) ^ seed);
                        if (i > 48) {
                            seed = rapid_mix(rapid_read64(p + 32) ^ secret[1], rapid_read64(p + 40) ^ seed);
                            if (i > 64) {
                                seed = rapid_mix(rapid_read64(p + 48) ^ secret[1], rapid_read64(p + 56) ^ seed);
                                if (i > 80) {
                                    seed = rapid_mix(rapid_read64(p + 64) ^ secret[2], rapid_read64(p + 72) ^ seed);
                                    if (i > 96) {
                                        seed = rapid_mix(rapid_read64(p + 80) ^ secret[1], rapid_read64(p + 88) ^ seed);
                                    }
                                }
                            }
                        }
                    }
                }
                a=rapid_read64(p+i-16) ^ i;  b=rapid_read64(p+i-8);
            }
            a ^= secret[1];
            b ^= seed;
            rapid_mum(&a, &b);
            return rapid_mix(a ^ secret[7], b ^ secret[1] ^ i);
        }
    } // namespace detail
    inline constexpr uint64_t hash_impl(const uint8_t* key, size_t len, uint64_t seed){
        return detail::rapidhash_internal(key, len, seed, detail::rgl_secret);
    }
}// namespace rgl::hash

namespace rgl {
    template<typename T>
    struct hasher {
        static inline uint64_t hash(const T& key, uint64_t seed) {
            if constexpr (__is_enum(T) || is_enum_v<T>) {
                using Underlying = underlying_type_t<T>;
                return hasher<Underlying>::hash(static_cast<Underlying>(key), seed);
            } 
            else {
                return hash::hash_impl(reinterpret_cast<const uint8_t*>(&key), sizeof(T), seed);
            }
        }

        inline uint64_t operator()(const T& key, uint64_t seed) const {
            return hash(key, seed);
        }
    };
}