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

//string.cpp
#include "rgl/string/string.h"
#include "rgl/core/utility/memory.h"
#include "rgl/core/utility/move.h"

using namespace rgl;

char* string::allocate_for(size_t required_len){
    if (required_len <= SSO_CAP) {
        if (is_long) {
            delete[] long_str.buffer;
            is_long = false;
        }
        sso.len = static_cast<uint8_t>(required_len);
        sso.buffer[required_len] = '\0';
        return sso.buffer;
    }

    if (is_long && long_str.capacity >= required_len) {
        long_str.len = required_len;
        long_str.buffer[required_len] = '\0';
        return long_str.buffer;
    }

    size_t new_capacity = required_len * 2;
    char* new_buf = new char[new_capacity + 1];
    new_buf[new_capacity] = '\0';

    if (is_long) {
        delete[] long_str.buffer;
    }

    is_long = true;
    long_str.buffer = new_buf;
    long_str.len = required_len;
    long_str.capacity = new_capacity;

    return new_buf;
}

string::string() : is_long(false) {
    sso.len = 0;
    sso.buffer[0] = '\0';
}
string::string(const string_view& sv) : is_long(false) {
    size_t len = sv.size();

    char* dest = allocate_for(len);
    if(len > 0)
        memcpy(dest, sv.data(), len);

}
string::string(const string& other) : is_long(other.is_long) {
    size_t len = other.size();
    char* dest = allocate_for(len);
    memcpy(dest, other.data(), len + 1);
}
string::string(const char* str) {
    size_t len = str ? strlen(str) : 0;
    char* dest = allocate_for(len);
    if (str && len > 0)
        memcpy(dest, str, len);
}

string::string(const char* str, size_t __size) {
    char* dest = allocate_for(__size);
    if (str && __size > 0)
        memcpy(dest, str, __size);
}

string::string(size_t count, char ch) {
    char* dest = allocate_for(count);
    for (size_t i = 0; i < count; ++i)
        dest[i] = ch;
}

string::string(string&& other) noexcept : is_long(other.is_long) {
    if (is_long) {
        long_str = other.long_str;
        other.long_str.buffer = nullptr;
        other.long_str.len = 0;
        other.long_str.capacity = 0;
    } else {
        sso = other.sso;
        other.sso.len = 0;
        other.sso.buffer[0] = '\0';
    }
    other.is_long = false;
}

string::~string() {
    if (is_long) {
        delete[] long_str.buffer;
    }
}