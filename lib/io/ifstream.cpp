/*
 * Regular Library (rgl)
 * Copyright (C) 2026 [Isra-222]
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see
 * <https://www.gnu.org/licenses/>.
 */

//ifstream.cpp
#include "rgl/io/ifstream.h"
#include "rgl/string/string.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace rgl {

    ifstream::ifstream(const string& path) {
        int fd = ::open(path.c_str(), O_RDONLY);
        if (fd == -1) return;

        struct stat st;
        if (::fstat(fd, &st) == -1) {
            ::close(fd);
            return;
        }
        
        file_size = static_cast<size_t>(st.st_size);
        if (file_size == 0) {
            ::close(fd);
            return;
        }

        data_ptr = ::mmap(nullptr, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
        
        if (data_ptr == MAP_FAILED) {
            data_ptr = nullptr;
            file_size = 0;
        }
        ::close(fd);
    }

    ifstream::~ifstream() {
        close();
    }

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
        if (data_ptr != nullptr) {
            ::munmap(data_ptr, file_size);
            data_ptr = nullptr;
            file_size = 0;
            cursor = 0;
        }
    }

    char ifstream::peek() const { 
        return (cursor < file_size) ? static_cast<char*>(data_ptr)[cursor] : '\0'; 
    }

    void ifstream::advance() { 
        if (cursor < file_size) cursor++; 
    }

    bool ifstream::isAtEnd() const { 
        return cursor >= file_size; 
    }

    bool ifstream::isOpen() const { 
        return data_ptr != nullptr; 
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
}