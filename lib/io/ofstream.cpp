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

//ostream.cpp
#include "rgl/io/ofstream.h"
#include <fcntl.h>
#include <unistd.h>

namespace rgl {
    ofstream::ofstream(const char* path) 
        : ostream(::open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644)) {
    }

    ofstream::~ofstream() {
        if (fd != -1) {
            ::close(fd);
            fd = -1;
        }
    }
}