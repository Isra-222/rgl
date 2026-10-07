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
#ifndef RGL_OS_UNISTD_H
#define RGL_OS_UNISTD_H

#include "rgl/core/types.h"

namespace rgl::sys{

	class file_descriptor final {
		int fd;
	public:
		constexpr file_descriptor(int raw) : fd(raw) {}
		constexpr file_descriptor() : fd(-1) {}

		constexpr int get() const { return fd; }
		operator bool() const { return fd >= 0; }
		
		constexpr bool operator==(int other) const noexcept { return fd == other; }
		constexpr bool operator!=(int other) const noexcept { return fd != other; }

		constexpr bool operator==(const file_descriptor& other) const noexcept { return fd == other.fd; }
        constexpr bool operator!=(const file_descriptor& other) const noexcept { return fd != other.fd; }

	};
	inline constexpr file_descriptor stdin(0);	
	inline constexpr file_descriptor stdout(1);
	inline constexpr file_descriptor stderr(2);	

	long read(file_descriptor fd, void* buf, size_t count);
	long write(file_descriptor fd, const void* buf, size_t count);
	long close(file_descriptor fd);
	long lseek(file_descriptor fd, long offset, int whence);
}// namespace rgl::sys

#endif