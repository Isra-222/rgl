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

//minmax.h
#pragma once

namespace rgl::algo{
	template<typename T>
    const T& max(const T& a, const T& b){
        return (a < b)? b : a;
    }

    template<typename T>
    const T& min(const T& a, const T& b){
        return (a < b)? a : b;
    }
    template<typename T>
    inline const T& clamp(const T& value, const T& min_val, const T& max_val){
    	return (value < min_val) ? min_val : (value > max_val ? max_val : value);
    }
}