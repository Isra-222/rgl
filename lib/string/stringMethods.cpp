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

//stringMethonds.cpp
#include "rgl/string/string.h"
#include "rgl/core/utility/memory.h"
#include "rgl/core/utility/move.h"

using namespace rgl;

string& string::append(const char* str, size_t len) {
    if (len == 0 || !str) return *this;

    size_t old_len = length();
    size_t new_len = old_len + len;
    if ((!is_long && new_len <= SSO_CAP) || (is_long && new_len <= long_str.capacity)) {
        char* dest = data() + old_len;
        memcpy(dest, str, len);
        
        if (is_long) {
            long_str.len = new_len;
            long_str.buffer[new_len] = '\0';
        } else {
            sso.len = static_cast<uint8_t>(new_len);
            sso.buffer[new_len] = '\0';
        }
        return *this;
    }
    
    size_t target_capacity = new_len * 2; 
    
    string temp(move(*this));
    char* buf = allocate_for(target_capacity);

    memcpy(buf, temp.data(), old_len);
    memcpy(buf + old_len, str, len);

    if (is_long) {
        long_str.len = new_len;
        long_str.buffer[new_len] = '\0';
    }
    return *this;
}
string& string::push_back(const char c){
    size_t old_len = length();
    size_t new_len = old_len + 1;

    if ((!is_long && new_len <= SSO_CAP) || (is_long && new_len <= long_str.capacity)) {
        char* dest = data() + old_len;
        dest[0] = c;
        dest[1] = '\0';

        if (is_long) {
            long_str.len = new_len;
        } else {
            sso.len = static_cast<uint8_t>(new_len);
        }
        return *this;
    }

    size_t target_capacity = new_len * 2;

    string temp(move(*this));
    char* buf = allocate_for(target_capacity);

    memcpy(buf, temp.data(), old_len);
    buf[old_len] = c;
    buf[new_len] = '\0';

    if (is_long) {
        long_str.len = new_len;
    }
    
    return *this;
}

void string::reserve(size_t new_len) {
    if (new_len > length()) {
        char* new_buf = new char[new_len + 1];
        if (data()) {
            memcpy(new_buf, data(), length());
            delete[] data();
        }
        if (is_long) {
            long_str.buffer = new_buf;
            long_str.capacity = new_len;
        } else {
            long_str.buffer = new_buf;
            long_str.len = sso.len;
            long_str.capacity = new_len;
            is_long = true;
        }
        data()[length()] = '\0';
    }
}
void string::resize(size_t count, char c){
    size_t old_len = length();

    if (count == old_len) return;

    if (count < old_len) {
        if (is_long) {
            long_str.len = count;
            long_str.buffer[count] = '\0';
        } else {
            sso.len = static_cast<uint8_t>(count);
            sso.buffer[count] = '\0';
        }
        return;
    }

    bool fits = (!is_long && count <= SSO_CAP) || (is_long && count <= long_str.capacity);

    if (fits) {
        char* dest = data() + old_len;

        for (size_t i = 0; i < (count - old_len); ++i)
            dest[i] = c;

        if (is_long) {
            long_str.len = count;
            long_str.buffer[count] = '\0';
        } else {
            sso.len = static_cast<uint8_t>(count);
            sso.buffer[count] = '\0';
        }
    } else {
        size_t target_capacity = (count > old_len * 2) ? count : old_len * 2;

        string temp(move(*this));

        char* new_buf = new char[target_capacity + 1];

        if (old_len > 0)
            memcpy(new_buf, temp.data(), old_len);

        for (size_t i = 0; i < (count - old_len); ++i)
            new_buf[old_len + i] = c;

        new_buf[count] = '\0';

        is_long = true;
        long_str.buffer = new_buf;
        long_str.len = count;
        long_str.capacity = target_capacity;
    }
}

void string::shrink_to_fit() {
    if (!is_long) return;

    size_t len = long_str.len;

    if (len <= SSO_CAP) {
        char temp_buf[SSO_CAP + 1];
        memcpy(temp_buf, long_str.buffer, len);
        temp_buf[len] = '\0';

        delete[] long_str.buffer;

        is_long = false;
        sso.len = static_cast<uint8_t>(len);
        memcpy(sso.buffer, temp_buf, len + 1);
    }
    else if (long_str.capacity > len) {
        char* new_buf = new char[len + 1];
        memcpy(new_buf, long_str.buffer, len);
        new_buf[len] = '\0';

        delete[] long_str.buffer;

        long_str.buffer = new_buf;
        long_str.capacity = len;
    }
}

string string::substr(size_t pos, size_t count) const {
    if (pos >= length()) return string();
    if (count == size_t(-1) || pos + count > length()) {
        count = length() - pos;
    }
    string result;
    char* dest = result.allocate_for(count);
    memcpy(dest, data() + pos, count);
    return result;
}


size_t string::find_first_of(const char* chars, size_t pos) const {
    if (!chars) return npos;
    for (size_t i = pos; i < length(); ++i) {
        for (const char* c = chars; *c; ++c) {
            if (data()[i] == *c) return i;
        }
    }
    return npos;
}

size_t string::find_last_of(const char* chars, size_t pos) const {
    if (!chars || length() == 0) return npos;
    size_t start = (pos >= length()) ? length() - 1 : pos;
    for (size_t i = start; i != size_t(-1); --i) {
        for (const char* c = chars; *c; ++c) {
            if (data()[i] == *c) return i;
        }
    }
    return npos;
}

size_t string::find_first_not_of(const char* chars, size_t pos) const {
    if (!chars) return pos < length() ? pos : npos;
    for (size_t i = pos; i < length(); ++i) {
        bool found = false;
        for (const char* c = chars; *c; ++c) {
            if (data()[i] == *c) {
                found = true;
                break;
            }
        }
        if (!found) return i;
    }
    return npos;
}

size_t string::find_last_not_of(const char* chars, size_t pos) const {
    if (length() == 0) return npos;
    size_t start = (pos == size_t(-1) || pos >= length()) ? length() - 1 : pos;
    for (size_t i = start; i != size_t(-1); --i) {
        bool found = false;
        for (const char* c = chars; *c; ++c) {
            if (data()[i] == *c) {
                found = true;
                break;
            }
        }
        if (!found) return i;
    }
    return npos;
}

void string::erase(size_t index, size_t count) {
    if (index >= length()) return;
    if (count == size_t(-1) || index + count > length()) {
        count = length() - index;
    }
    if (count == 0) return;

    size_t old_len = length();
    size_t new_len = old_len - count;
    const char* src = data();

    char* dest = allocate_for(new_len);

    if(index > 0)
        memcpy(dest, src, index);

    if(index + count < old_len)
        memcpy(dest + index, src + index + count, old_len - (index + count));

}
string& string::replace(size_t pos, size_t count, const char* str, size_t len){
    if (pos > length()) return *this;
    if (count > length() - pos) count = length() - pos;
    
    size_t old_len = length();
    size_t str_len = len;
    size_t new_len = old_len - count + str_len;
    const char* src = data();

    char* dest = allocate_for(new_len);

    if(pos > 0)
        memcpy(dest, src, pos);

    if(str_len > 0)
        memcpy(dest + pos, str, str_len);

    if(pos + count < old_len)
        memcpy(dest + pos + str_len, src + pos + count, old_len - (pos + count));
    

    return *this;
}

string& string::replace_all(const char* from, size_t from_len, const char* to, size_t to_len) {
    if (!from || from_len == 0) return *this;
    
    size_t count = 0;
    size_t pos = 0;
    while ((pos = find(from, from_len, pos)) != size_t(-1)) {
        ++count;
        pos += from_len;
    }
    if (count == 0) return *this;

    size_t old_len = length();
    size_t new_len = old_len + count * (to_len - from_len);

    string temp;
    char* dest = temp.allocate_for(new_len);

    size_t src_pos = 0;
    size_t dst_pos = 0;
    pos = 0;
    
    while ((pos = find(from, from_len, src_pos)) != size_t(-1)) {
        size_t chunk_len = pos - src_pos;
        if (chunk_len > 0) {
            memcpy(dest + dst_pos, data() + src_pos, chunk_len);
            dst_pos += chunk_len;
        }
        if (to_len > 0) {
            memcpy(dest + dst_pos, to, to_len);
            dst_pos += to_len;
        }
        src_pos = pos + from_len;
    }
    if (src_pos < old_len) {
        memcpy(dest + dst_pos, data() + src_pos, old_len - src_pos);
        dst_pos += old_len - src_pos;
    }
    dest[new_len] = '\0';
    
    *this = move(temp);
    return *this;
}

size_t string::find(const char* str, size_t pos, size_t str_len) const {
    if (!str || str_len == 0) return pos;
    if (str_len > length() || pos >= length()) return size_t(-1);
    for (size_t i = pos; i <= length() - str_len; ++i) {
        if (memcmp(data() + i, str, str_len) == 0) return i;
    }
    return size_t(-1);
}


size_t string::rfind(const char* str, size_t pos, size_t len) const {
    if (len == 0) return (pos < length()) ? pos : length();
    if (!str || len > length()) return npos;
    
    if (pos == size_t(-1) || pos > length() - len) {
        pos = length() - len;
    }
    
    for (size_t i = pos; i != size_t(-1); --i) {
        if (memcmp(data() + i, str, len) == 0) return i;
    }
    
    return npos;
}