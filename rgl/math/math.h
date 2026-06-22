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


//math
#pragma once

namespace rgl {
    inline float abs(float x) {
        return (x < 0) ? -x : x;
    }
    inline float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }

	inline float sin(float x) {
        const float PI = 3.14159265f;
        x = x - 2 * PI * (int)(x / (2 * PI));
        return x - (x*x*x)/6.0f + (x*x*x*x*x)/120.0f - (x*x*x*x*x*x*x)/5040.0f;
    }

    inline float cos(float x) {
        return sin(x + 1.570796f);
    }

    template<typename Func>
    inline float derivative(Func f, float x, float h = 0.001f) {
        return (f(x + h) - f(x - h)) / (2.0f * h);
    }

    template<typename Func>
    inline float integrate(Func f, float a, float b, int steps = 1000) {
        float h = (b - a) / steps;
        float sum = 0.5f * (f(a) + f(b));
        for (int i = 1; i < steps; ++i) sum += f(a + i * h);
        return sum * h;
    }


	template<typename T = float>
    class mat4 {
    public:
        T m[16];

        mat4(bool identity = true) {
            for (int i = 0; i < 16; ++i) m[i] = 0;
            if (identity) {
                m[0] = m[5] = m[10] = m[15] = (T)1;
            }
        }

        inline T& operator()(int row, int col) { return m[row * 4 + col]; }
        inline const T& operator()(int row, int col) const { return m[row * 4 + col]; }

        mat4 operator*(const mat4& other) const {
            mat4 result(false);
            for (int row = 0; row < 4; ++row) {
                for (int col = 0; col < 4; ++col) {
                    T sum = 0;
                    for (int k = 0; k < 4; ++k) {
                        sum += (*this)(row, k) * other(k, col);
                    }
                    result(row, col) = sum;
                }
            }
            return result;
        }
    };
    
    using mat4f = mat4<float>;
    using mat4d = mat4<double>;

    inline constexpr signed long long gcd(signed long long a, signed long long b){
        return b == 0? a : gcd(b, a % b);
    }

    template<ssize_t Num, ssize_t Den = 1>
    struct ratio{
        static_assert(Den != 0, "Error: Cannot divide by zero");

        static constexpr ssize_t divisor = gcd(Num, Den);

        static constexpr ssize_t num = (Num / divisor) * ((Den < 0)? -1: 1);
        static constexpr ssize_t den = (Den / divisor) * ((Den < 0)? -1: 1);
    };

}