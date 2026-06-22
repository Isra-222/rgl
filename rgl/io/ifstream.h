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

//ifstream
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "string.h"

namespace rgl{

    class ifstream final{
        char* data_ptr = nullptr;
        size_t file_size = 0;
        size_t cursor = 0;

    public:
        ifstream(const rgl::string& path) {
            int fd = ::open(path.c_str(), O_RDONLY);
            if (fd == -1) return;

            struct stat st;
            if (::fstat(fd, &st) == -1) {
                ::close(fd);
                return;
            }
            
            file_size = static_cast<size_t>(st.st_size);
            data_ptr = static_cast<char*>(::mmap(nullptr, file_size, PROT_READ, MAP_PRIVATE, fd, 0));
            
            if (data_ptr == MAP_FAILED) {
                data_ptr = nullptr;
                file_size = 0;
            }
            ::close(fd);
        }

        ~ifstream() {
            if (data_ptr) {
                munmap(data_ptr, file_size);
            }
        }

        void close() {
            if (data_ptr != nullptr) {
                munmap(data_ptr, file_size);
                data_ptr = nullptr;
                file_size = 0;
                cursor = 0;
            }
        }

        char peek() const { return (cursor < file_size) ? data_ptr[cursor] : '\0'; }
        void advance() { if (cursor < file_size) cursor++; }
        bool isAtEnd() const { return cursor >= file_size; }
        bool isOpen() const { return data_ptr != nullptr; }
    };
    inline bool getline(rgl::ifstream& stream, rgl::string& line) {
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