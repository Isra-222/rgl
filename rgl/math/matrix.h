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

//matrix.h
#pragma once

namespace rgl::math {

	template <typename T = float>
	class alignas(16) mat4 {
	public:
	    T m[16];
	    explicit mat4(bool identity = true) {
	        if (identity) {
	            for (int i = 0; i < 16; ++i) m[i] = 0;
	            m[0] = m[5] = m[10] = m[15] = static_cast<T>(1);
	        } else {
	            for (int i = 0; i < 16; ++i) m[i] = static_cast<T>(0);
	        }
	    }

	    inline T& operator()(int row, int col) { return m[row * 4 + col]; }
	    inline const T& operator()(int row, int col) const { return m[row * 4 + col]; }

	    inline T* data() { return m; }
	    inline const T* data() const { return m; }

	    mat4 operator*(const mat4& other) const {
	        mat4 res(false);
	        for (int i = 0; i < 4; ++i) {
	            for (int j = 0; j < 4; ++j) {
	                res.m[i * 4 + j] = m[i * 4 + 0] * other.m[0 * 4 + j] +
	                                   m[i * 4 + 1] * other.m[1 * 4 + j] +
	                                   m[i * 4 + 2] * other.m[2 * 4 + j] +
	                                   m[i * 4 + 3] * other.m[3 * 4 + j];
	            }
	        }
	        return res;
	    }

	    mat4 transposed() const {
	        mat4 res(false);
	        for (int i = 0; i < 4; ++i) {
	            for (int j = 0; j < 4; ++j) {
	                res.m[j * 4 + i] = m[i * 4 + j];
	            }
	        }
	        return res;
	    }
	};

	using mat4f = mat4<float>;
	using mat4d = mat4<double>;

} // namespace rgl::math