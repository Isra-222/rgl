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

#pragma once

#include "string.h"

namespace rgl {
    double strtod(const char* str);
    long double strtold(const char* str, char** endptr);
    long long atoll_custom(const char* str);
    int double_to_buffer(char* buf, double d);
    
    inline long double stold(const string& str){ return strtold(str.c_str(), nullptr); }
    long long stoll(const string& str);
    int stoi(const string& str);
    double stod(const string& str);
    
    string to_string(int i);
    string to_string(unsigned int i);
    string to_string(long i);
    string to_string(unsigned long i);
    string to_string(long long i);
    string to_string(unsigned long long i);
    string to_string(double d);
    string to_string(float f);
    string to_string(bool b);
    string to_string(const char* str);
    string to_string(const string& str);
    string to_string(char c);
    string to_string(void*);
    
    template<typename T>
    string to_string(T* ptr) {
        if (ptr == nullptr) return string("nullptr");
        char buf[32];
        buf[0] = '0';
        buf[1] = 'x';
        uintptr_t val = reinterpret_cast<uintptr_t>(ptr);
        int pos = 2;
        for (int i = sizeof(uintptr_t) * 2 - 1; i >= 0; --i) {
            int nibble = (val >> (i * 4)) & 0xF;
            buf[pos++] = (nibble < 10) ? (nibble + '0') : (nibble - 10 + 'a');
        }
        buf[pos] = '\0';
        return string(buf);
    }
}