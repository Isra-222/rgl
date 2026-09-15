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

//stringOperators.cpp
#include "rgl/string/string.h"
#include "rgl/core/utility/memory.h"
#include "rgl/core/utility/move.h"

using namespace rgl;

string& string::operator=(string&& other) noexcept{
    if(this != &other){
        if(is_long)
            delete[] long_str.buffer;

        if (other.is_long){
            long_str = other.long_str;

            other.long_str.buffer = nullptr;
            other.long_str.len = 0;
            other.long_str.capacity = 0;
        }else{
            sso = other.sso;
            other.sso.len = 0;
            other.sso.buffer[0] = '\0';
        }

        is_long = other.is_long;
        other.is_long = false;
    }
    return *this;
}

string& string::operator=(const string& other){
    if(this != &other){
        if (is_long)
            delete[] long_str.buffer;

        char* str = allocate_for(other.length());
        if(other.is_long)
            memcpy(str, other.long_str.buffer, other.long_str.len);
        else
            memcpy(str, other.sso.buffer, sso.len);
    }
    return *this;
}

string& string::operator=(const char* str) {
    size_t len = str ? strlen(str) : 0;

    char* currentBuf = allocate_for(len);
    memcpy(currentBuf, str, len);

    return *this;
}
string& string::operator=(const string_view other){
    char* currentBuf = allocate_for(other.length());
    memcpy(currentBuf, other.data(), other.length());

    return *this;
}


bool string::operator==(const string& other) const {
    if(this == &other) return true;

    if (length() != other.length()) return false;
    return memcmp(data(), other.data(), length()) == 0;
}
bool string::operator==(const char* str) const {
    if(this->data() == str) return true;

    size_t len = str ? strlen(str) : 0;
    return (length() == len) && (memcmp(data(), str, len) == 0);
}



string operator+(const string& lhs, const string& rhs) {
    string result = lhs;
    result += rhs;
    return result;
}

string operator+(const string& lhs, const char* rhs) {
    string result = lhs;
    result += rhs;
    return result;
}

string operator+(const char* lhs, const string& rhs) {
    string result(lhs);
    result += rhs;
    return result;
}

string operator+(const string& lhs, char rhs) {
    string result = lhs;
    result += rhs;
    return result;
}

string operator+(char lhs, const string& rhs) {
    string result(1, lhs);
    result += rhs;
    return result;
}

string operator+(const string& lhs, rgl::string_view rhs) {
    string result = lhs;
    result += rhs;
    return result;
}

string operator+(rgl::string_view lhs, const string& rhs) {
    string result;
    result.reserve(lhs.size() + rhs.size());
    result.append(lhs.data(), lhs.size());
    result += rhs;
    return result;
}

string operator+(rgl::string_view lhs, rgl::string_view rhs) {
    string result;
    result.reserve(lhs.size() + rhs.size());
    result.append(lhs.data(), lhs.size());
    result.append(rhs.data(), rhs.size());
    return result;
}