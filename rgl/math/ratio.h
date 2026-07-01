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

//ratio.h
#pragma once

#include "utility.h"

namespace rgl::math{
	template<ssize_t Num, ssize_t Den = 1>
    struct ratio{
        static_assert(Den != 0, "Error: Cannot divide by zero");

        static constexpr ssize_t divisor = gcd(Num, Den);

        static constexpr ssize_t num = (Num / divisor) * ((Den < 0)? -1: 1);
        static constexpr ssize_t den = (Den / divisor) * ((Den < 0)? -1: 1);
    };

}