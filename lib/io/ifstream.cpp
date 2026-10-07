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

#include "rgl/io/ifstream.h"
#include "rgl/string/string.h"

#include "rgl/os/fcntl.h"
#include "rgl/os/sys/mman.h"
#include "rgl/os/sys/stat.h"
#include "rgl/os/unistd.h"

using namespace rgl;

ifstream::ifstream(ifstream&& other) noexcept 
    : data_ptr(other.data_ptr), file_size(other.file_size), cursor(other.cursor) {
    other.data_ptr = nullptr;
    other.file_size = 0;
    other.cursor = 0;
}

ifstream& ifstream::operator=(ifstream&& other) noexcept {
    if (this != &other) {
        close();
        data_ptr = other.data_ptr;
        file_size = other.file_size;
        cursor = other.cursor;
        
        other.data_ptr = nullptr;
        other.file_size = 0;
        other.cursor = 0;
    }
    return *this;
}

void ifstream::close() {
    if (data_ptr != nullptr && data_ptr != sys::MAP_FAILED) {
        sys::munmap(data_ptr, file_size);
        data_ptr = nullptr;
        file_size = 0;
        cursor = 0;
    }
}


void ifstream::open(string_view path) {
    close();

    if (!path.data()) return;

    sys::file_descriptor file_desc = sys::open(path.data(), sys::file_f::rdonly);
    int fd = file_desc.get();
    if (fd == -1) return;

    sys::stat_ st;
    if (sys::fstat(fd, &st) == -1) {
        sys::close(fd);
        return;
    }
    
    file_size = static_cast<size_t>(st.st_size);
    if (file_size == 0) {
        sys::close(fd);
        return;
    }

    data_ptr = sys::mmap(nullptr, file_size, sys::prot::read, sys::map::_private, fd, 0);
    
    if (data_ptr == sys::MAP_FAILED) {
        data_ptr = nullptr;
        file_size = 0;
    }
    sys::close(fd);
    cursor = 0;
}

char ifstream::peek() const { 
    return (cursor < file_size && data_ptr) ? static_cast<const char*>(data_ptr)[cursor] : '\0'; 
}

void ifstream::advance() { 
    if (cursor < file_size) cursor++; 
}

bool ifstream::isAtEnd() const { 
    return cursor >= file_size; 
}

bool ifstream::is_open() const { 
    return data_ptr != nullptr && data_ptr != sys::MAP_FAILED; 
}

bool getline(rgl::ifstream& stream, rgl::string& line) {
    if (stream.isAtEnd()) return false;
    
    line.clear();
    bool found = false;
    while (!stream.isAtEnd()) {
        char c = stream.peek();
        stream.advance();
        if (c == '\n') {
            found = true;
            break;
        }
        if (c != '\r') line += c;
    }
    return found || !line.empty(); 
}