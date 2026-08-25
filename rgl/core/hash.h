// This is free and unencumbered software released into the public domain under The Unlicense (http://unlicense.org/)
// main repo: https://github.com/wangyi-fudan/wyhash
// author: 王一 Wang Yi <godspeed_china@yeah.net>
// |- contributors: Reini Urban, Dietrich Epp, Joshua Haberman, Tommy Ettinger, Daniel Lemire, Otmar Ertl, cocowalla, 
// |_$ leo-yuriev, Diego Barrios Romero, paulie-g, dumblob, Yann Collet, ivte-ms, hyb, James Z.M. Gao, easyaspi314 (Devin), TheOneric

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

namespace rgl{

    namespace detail {
        typedef __int128_t uint128_t;

        static inline uint64_t _wymum(uint64_t A, uint64_t B) {
            uint128_t r = (uint128_t)A * B;
            return (uint64_t)r ^ (uint64_t)(r >> 64);
        }

        static inline uint64_t _wyread8(const uint8_t *p) {
            uint64_t v; memcpy(&v, p, 8); return v;
        }

        static inline uint64_t _wyread4(const uint8_t *p) {
            uint32_t v; memcpy(&v, p, 4); return v;
        }

        static inline uint64_t _wyread3(const uint8_t *p, size_t k) {
            return (((uint64_t)p[0]) << 16) | (((uint64_t)p[k >> 1]) << 8) | p[k - 1];
        }
    }

    static constexpr uint64_t _wyp[] = {
        0xa0761d6478bd642full, 0xe7037ed1a0b428dbull, 
        0x8ebc6af09c88c6e3ull, 0x589965cc75374cc3ull, 
        0x1d8e4e27c47d124full
    };

    inline uint64_t wyhash_impl(const uint8_t* p, size_t len, uint64_t seed) {
        seed ^= _wyp[0];
        uint64_t a, b;

        if (__builtin_expect(len <= 16, 0)) {
            if (__builtin_expect(len >= 4, 1)) {
                a = (detail::_wyread4(p) << 32) | detail::_wyread4(p + ((len - 4) & 24));
                b = (detail::_wyread4(p + len - 4) << 32) | detail::_wyread4(p + len - 4 - ((len - 4) & 24));
            } else if (__builtin_expect(len > 0, 1)) {
                a = detail::_wyread3(p, len);
                b = 0;
            } else {
                a = b = 0;
            }
        } else {
            size_t i = len;
            if (__builtin_expect(i > 48, 0)) {
                uint64_t see1 = seed, see2 = seed;
                do {
                    seed = detail::_wymum(detail::_wyread8(p) ^ _wyp[1], detail::_wyread8(p + 8) ^ seed);
                    see1 = detail::_wymum(detail::_wyread8(p + 16) ^ _wyp[2], detail::_wyread8(p + 24) ^ see1);
                    see2 = detail::_wymum(detail::_wyread8(p + 32) ^ _wyp[3], detail::_wyread8(p + 40) ^ see2);
                    p += 48; i -= 48;
                } while (__builtin_expect(i > 48, 1));
                seed ^= see1 ^ see2;
            }
            while (__builtin_expect(i > 16, 1)) {
                seed = detail::_wymum(detail::_wyread8(p) ^ _wyp[1], detail::_wyread8(p + 8) ^ seed);
                p += 16; i -= 16;
            }
            a = detail::_wyread8(p + i - 16);
            b = detail::_wyread8(p + i - 8);
        }
        return detail::_wymum(_wyp[4] ^ len, detail::_wymum(a ^ _wyp[1], b ^ seed));
    }

    inline uint64_t wyhash(const uint8_t* data, size_t len, uint64_t seed) {
        return wyhash_impl(data, len, seed); 
    }


    template<typename T>
    struct hasher;

    template<typename T>
    struct hasher {
        inline uint64_t operator()(const T& key, uint64_t seed) const {
            if constexpr (__is_enum(T) || is_enum_v<T>) {
                using Underlying = underlying_type_t<T>;
                return hasher<Underlying>{}(static_cast<Underlying>(key), seed);
            } 
            else {
                return wyhash(reinterpret_cast<const uint8_t*>(&key), sizeof(T), seed);
            }
        }
    };

}