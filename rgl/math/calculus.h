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

//calculus.h
#pragma once

namespace rgl::math {
	inline float int_gaussian(float (*f)(float), float a, float b) {
	    float mid = (a + b) / 2.0f;
	    float diff = (b - a) / 2.0f;
	    float x1 = mid - diff * 0.5773502692f;
	    float x2 = mid + diff * 0.5773502692f;
	    
	    return diff * (f(x1) + f(x2));
	}
	template<typename Func>
    inline float int_trap(Func f, float a, float b, int steps = 1000) {
        float h = (b - a) / steps;
        float sum = 0.5f * (f(a) + f(b));
        for (int i = 1; i < steps; ++i) sum += f(a + i * h);
        return sum * h;
    }
    template<typename Func>
	inline float int_simpson(Func f, float a, float b, int steps = 1000) {
	    if (steps % 2 != 0) steps++;
	    float h = (b - a) / steps;
	    float sum = f(a) + f(b);

	    for (int i = 1; i < steps; ++i) {
	        float x = a + i * h;
	        sum += (i % 2 == 0) ? 2 * f(x) : 4 * f(x);
	    }
	    return (h / 3.0f) * sum;
	}
    template<typename Func>
    inline float diff_central(Func f, float x, float h = 0.001f) {
        return (f(x + h) - f(x - h)) / (2.0f * h);
    }

    template<typename Func>
    inline float diff_5pt(Func f, float x, float h = 0.001f) {
        return (-f(x + 2 * h) + 8 * f(x + h) - 8 * f(x - h) + f(x - 2 * h)) / (12.0f * h);
    }

    template<typename Func>
    inline float diff_7pt(Func f, float x, float h = 0.001f) {
        return (f(x + 3 * h) - 9 * f(x + 2 * h) + 45 * f(x + h) 
                - 45 * f(x - h) + 9 * f(x - 2 * h) - f(x - 3 * h)) / (60.0f * h);
    }
    inline float clamp(float v, float min, float max) {
        return (v < min) ? min : (v > max) ? max : v;
    }
    
    inline float map(float v, float in_min, float in_max, float out_min, float out_max) {
        return (v - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

}//namespace rgl::math
