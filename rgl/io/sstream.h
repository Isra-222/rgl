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

//sstream
#pragma once

#include "rgl/string/string.h"

namespace rgl {

    class sstream {
        rgl::string data;
        size_t cursor = 0;

    public:
        sstream() = default;

        sstream(const rgl::string& str) : data(str) {}

        char peek() const { 
            return (cursor < data.length()) ? data[cursor] : '\0'; 
        }
        
        void advance() { 
            if (cursor < data.length()) cursor++; 
        }
        
        bool isAtEnd() const { 
            return cursor >= data.length(); 
        }
        
        bool isOpen() const { 
            return true;
        }
        void str(const rgl::string& s) {
            data = s;
            cursor = 0;
        }

        rgl::string str() const {
            return data;
        }
        void clear() {
            cursor = 0;
        }
    };
    inline bool getline(rgl::sstream& stream, rgl::string& line) {
        line.clear();
        if (stream.isAtEnd()) return false;
        
        while (!stream.isAtEnd()) {
            char c = stream.peek();
            stream.advance();
            if (c == '\n') break;
            if (c != '\r') {
                line += c;
            }
        }
        return true;
    }

}