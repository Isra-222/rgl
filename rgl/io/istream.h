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

// istream.h
#pragma once

#include "rgl/string/format.h"

#ifndef EOF
    #define EOF (-1)
#endif

namespace rgl {
    class istream {
        char buffer[1024];
        size_t buffer_pos;
        size_t buffer_size;
        bool is_good;
        int fd;
        
        char getch();
        void ungetch(char c);
        void skip_whitespace();
        
    public:
        explicit istream(int file_descriptor = 0);
        
        istream& operator>>(char& c);
        istream& operator>>(signed char& c);
        istream& operator>>(unsigned char& c);
        istream& operator>>(char* buffer);
        istream& operator>>(short& value);
        istream& operator>>(int& value);
        istream& operator>>(long& value);
        istream& operator>>(long long& value);
        istream& operator>>(unsigned short& value);
        istream& operator>>(unsigned int& value);
        istream& operator>>(unsigned long& value);
        istream& operator>>(unsigned long long& value);
        istream& operator>>(float& value);
        istream& operator>>(double& value);
        istream& operator>>(long double& value);
        istream& operator>>(bool& value);
        istream& operator>>(rgl::string& str);
        
        istream& getline(char* buffer, size_t size);
        istream& getline(rgl::string& str, char delim = '\n');
        
        operator bool() const;
        bool eof() const;
    };
}