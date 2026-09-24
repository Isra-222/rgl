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
#ifndef RGL_SYS_TIME
#define RGL_SYS_TIME

#include "rgl/core/types.h"

namespace rgl {
	using time_t = size_t;

	namespace sys {
		struct timespec{
			int64_t tv_sec;
			int64_t tv_nsec;
		};

		constexpr int CLOCK_REALTIME = 0;
		constexpr int CLOCK_MONOTONIC = 1;

		int clock_gettime(int clk_id, timespec* tp);
	} // namespace sys
}//namespace rgl


#endif