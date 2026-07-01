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

//vector.h
#pragma once

namespace rgl::math {

	template <typename T = float>
	class alignas(16) vec4 {
	public:
	    union {
	        struct { T x, y, z, w; };
	        T v[4];
	    };

	    vec4() : x(0), y(0), z(0), w(0) {}
	    vec4(T scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
	    vec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

	    inline T& operator[](int i) { return v[i]; }
	    inline const T& operator[](int i) const { return v[i]; }

	    vec4 operator+(const vec4& other) const { return {x + other.x, y + other.y, z + other.z, w + other.w}; }
	    vec4 operator-(const vec4& other) const { return {x - other.x, y - other.y, z - other.z, w - other.w}; }

	    static T dot(const vec4& a, const vec4& b) {
	        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	    }

	    inline T* data() { return v; }
	    inline const T* data() const { return v; }
	};

	using vec4f = vec4<float>;
	using vec4d = vec4<double>;

} // namespace rgl::math