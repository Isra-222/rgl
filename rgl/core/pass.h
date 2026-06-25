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

//pass.h
#pragma once

namespace rgl{
	inline void pass(){
		#if defined(__x86_64__) || defined(_M_X64)
        __asm__ volatile ("nop");
		#elif defined(__aarch64__)
		        __asm__ volatile ("nop");
		#else
		    ((void)0)
		#endif
	}
	inline void relax(){
		#if defined(__x86_64__) || defined(_M_X64)
        __asm__ volatile ("pause");
		#elif defined(__aarch64__)
		        __asm__ volatile ("yield");
		#else
			((void)0)
		#endif
	}
}