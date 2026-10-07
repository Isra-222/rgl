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
#include "rgl/os/fcntl.h"
#include "rgl/os/unistd.h"

using namespace rgl;

ofstream::ofstream(rgl::string_view path) 
    : ostream(sys::open(path.data(), sys::file_f::wronly | 
        sys::file_f::creat | sys::file_f::trunc, sys::file_perm::owner_read | sys::file_perm::owner_write)) {}

void ofstream::open(rgl::string_view path) {
    if (file_is_open) {
        sys::close(fd);
    }
    fd = sys::open(path.data(), sys::file_f::wronly | sys::file_f::creat | sys::file_f::trunc, sys::file_perm::owner_read | sys::file_perm::owner_write);
    file_is_open = fd;
}

void ofstream::close(){
    if(file_is_open){
        flush_buffer();
        sys::close(fd);
        fd = -1;
        file_is_open = false;
    }
}

ofstream::~ofstream() {
    if (fd) {
        sys::close(fd);
        fd = -1;
    }
}
