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
#pragma once

#include "rgl/core/types.h"

namespace rgl {
    class string;

    class ifstream final {
        void* data_ptr = nullptr;
        size_t file_size = 0;
        size_t cursor = 0;
    public:
        explicit ifstream(const char* path);
        explicit ifstream(const string& path);
        ifstream();
        ~ifstream();

        ifstream(const ifstream&) = delete;
        ifstream& operator=(const ifstream&) = delete;
        
        ifstream(ifstream&& other) noexcept;
        ifstream& operator=(ifstream&& other) noexcept;

        void close();
        char peek() const;
        void advance();
        bool isAtEnd() const;
        void Open(const char* path);
        void Open(const string& path);
        bool is_open() const;
        const void* data() const { return data_ptr; }
        size_t size() const { return file_size; }
    };

    bool getline(ifstream& stream, string& line);
}