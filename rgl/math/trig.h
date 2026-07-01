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

//trig.h
#pragma once

#include "constants.h"

namespace rgl::math{
	inline float normalize_angle(float radians) {
        radians = radians - consts::PI_2X * (float)((int)(radians * consts::INV_PI * 0.5f));
        if (radians > consts::PI) radians -= consts::PI_2X;
        if (radians < -consts::PI) radians += consts::PI_2X;
        return radians;
    }
    inline float sin(float radians) {
        radians = normalize_angle(radians);
        float x2 = radians * radians;
        return radians * (1.0f - x2 * (0.16666667f - x2 * 0.00833333f));
    }
    inline float cos(float radians) {
        return sin(radians + consts::PI_2);
    }
}