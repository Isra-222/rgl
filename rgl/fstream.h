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

//istream
#pragma once

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "string.h"

namespace rgl {

    class fstream {
        char* data_ptr = nullptr;
        size_t file_size = 0;
        size_t cursor = 0;

    public:
        fstream(const rgl::string& path) {

			int fd = open(path.c_str(), O_RDONLY);
			if (fd == -1) return;

			struct stat st;
			fstat(fd, &st);
			file_size = st.st_size;

			data_ptr = (char*)mmap(nullptr, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
			close(fd);
        }

        ~fstream() {
            if (data_ptr) {
                munmap(data_ptr, file_size);
            }
        }

        char peek() const { return (cursor < file_size) ? data_ptr[cursor] : '\0'; }
        void advance() { if (cursor < file_size) cursor++; }
        bool isAtEnd() const { return cursor >= file_size; }
        bool isOpen() const { return data_ptr != nullptr; }
    };
}