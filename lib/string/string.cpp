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

namespace rgl {

    void string::reserve(size_t new_len) {
        if (new_len > length_val()) {
            char* new_buf = new char[new_len + 1];
            if (data_ptr()) {
                memcpy(new_buf, data_ptr(), length_val());
                delete[] data_ptr();
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
            data_ptr()[length_val()] = '\0';
        }
    }

    string::string() : is_long(false) {
        sso.len = 0;
        sso.buffer[0] = '\0';
    }

    string::string(const string_view& sv) : is_long(false) {
        size_t len = sv.size();
        if (len <= SSO_CAP) {
            memcpy(sso.buffer, sv.data(), len);
            sso.buffer[len] = '\0';
            sso.len = static_cast<unsigned char>(len);
        } else {
            long_str.buffer = new char[len + 1];
            memcpy(long_str.buffer, sv.data(), len);
            long_str.buffer[len] = '\0';
            long_str.len = len;
            long_str.capacity = len;
            is_long = true;
        }
    }

    string::string(const string& other) : is_long(other.is_long) {
        if (is_long) {
            long_str.len = other.long_str.len;
            long_str.capacity = other.long_str.capacity;
            long_str.buffer = new char[long_str.capacity + 1];
            memcpy(long_str.buffer, other.long_str.buffer, long_str.len + 1);
        } else {
            sso.len = other.sso.len;
            memcpy(sso.buffer, other.sso.buffer, sso.len + 1);
        }
    }

    string::string(const char* str) : is_long(false) {
        size_t len = str ? strlen(str) : 0;
        if (len <= SSO_CAP) {
            if (str) {
                memcpy(sso.buffer, str, len);
            }
            sso.buffer[len] = '\0';
            sso.len = static_cast<unsigned char>(len);
        } else {
            long_str.buffer = new char[len + 1];
            if (str) {
                memcpy(long_str.buffer, str, len);
            }
            long_str.buffer[len] = '\0';
            long_str.len = len;
            long_str.capacity = len;
            is_long = true;
        }
    }

    string::string(size_t count, char ch) : is_long(false) {
        if (count <= SSO_CAP) {
            for (size_t i = 0; i < count; ++i) {
                sso.buffer[i] = ch;
            }
            sso.buffer[count] = '\0';
            sso.len = static_cast<unsigned char>(count);
        } else {
            long_str.buffer = new char[count + 1];
            for (size_t i = 0; i < count; ++i) {
                long_str.buffer[i] = ch;
            }
            long_str.buffer[count] = '\0';
            long_str.len = count;
            long_str.capacity = count;
            is_long = true;
        }
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

    string& string::operator=(string&& other) noexcept {
        if (this != &other) {
            if (is_long) {
                delete[] long_str.buffer;
            }
            if (other.is_long) {
                long_str = other.long_str;
                other.long_str.buffer = nullptr;
                other.long_str.len = 0;
                other.long_str.capacity = 0;
            } else {
                sso = other.sso;
                other.sso.len = 0;
                other.sso.buffer[0] = '\0';
            }
            is_long = other.is_long;
            other.is_long = false;
        }
        return *this;
    }

    string& string::operator=(const string& other) {
        if (this != &other) {
            if (is_long) {
                delete[] long_str.buffer;
            }
            if (other.is_long) {
                long_str.len = other.long_str.len;
                long_str.capacity = other.long_str.capacity;
                long_str.buffer = new char[long_str.capacity + 1];
                memcpy(long_str.buffer, other.long_str.buffer, long_str.len + 1);
                is_long = true;
            } else {
                sso.len = other.sso.len;
                memcpy(sso.buffer, other.sso.buffer, sso.len + 1);
                is_long = false;
            }
        }
        return *this;
    }

    string& string::operator=(const char* str) {
        size_t len = str ? strlen(str) : 0;
        if (is_long) {
            delete[] long_str.buffer;
            is_long = false;
        }
        if (len <= SSO_CAP) {
            if (str) {
                memcpy(sso.buffer, str, len);
            }
            sso.buffer[len] = '\0';
            sso.len = static_cast<unsigned char>(len);
        } else {
            long_str.buffer = new char[len + 1];
            if (str) {
                memcpy(long_str.buffer, str, len);
            }
            long_str.buffer[len] = '\0';
            long_str.len = len;
            long_str.capacity = len;
            is_long = true;
        }
        return *this;
    }

    bool string::operator==(const string& other) const {
        if (length_val() != other.length_val()) return false;
        return memcmp(data_ptr(), other.data_ptr(), length_val()) == 0;
    }

    bool string::operator==(const char* str) const {
        size_t len = str ? strlen(str) : 0;
        return (length_val() == len) && (memcmp(data_ptr(), str, len) == 0);
    }

    string& string::operator+=(char c) {
        size_t new_len = length_val() + 1;
        if (is_long) {
            if (new_len > long_str.capacity) {
                reserve(new_len);
            }
            long_str.buffer[long_str.len] = c;
            long_str.buffer[new_len] = '\0';
            long_str.len = new_len;
        } else {
            if (new_len <= SSO_CAP) {
                sso.buffer[sso.len] = c;
                sso.buffer[new_len] = '\0';
                sso.len = static_cast<unsigned char>(new_len);
            } else {
                char* new_buf = new char[new_len + 1];
                memcpy(new_buf, sso.buffer, sso.len);
                new_buf[sso.len] = c;
                new_buf[new_len] = '\0';
                long_str.buffer = new_buf;
                long_str.len = new_len;
                long_str.capacity = new_len;
                is_long = true;
            }
        }
        return *this;
    }

    string& string::operator+=(const char* str) {
        if (!str || !*str) return *this;
        size_t str_len = strlen(str);
        size_t new_len = length_val() + str_len;
        
        if (is_long) {
            if (new_len > long_str.capacity) {
                reserve(new_len);
            }
            memcpy(long_str.buffer + long_str.len, str, str_len);
            long_str.buffer[new_len] = '\0';
            long_str.len = new_len;
        } else {
            if (new_len <= SSO_CAP) {
                memcpy(sso.buffer + sso.len, str, str_len);
                sso.buffer[new_len] = '\0';
                sso.len = static_cast<unsigned char>(new_len);
            } else {
                char* new_buf = new char[new_len + 1];
                memcpy(new_buf, sso.buffer, sso.len);
                memcpy(new_buf + sso.len, str, str_len);
                new_buf[new_len] = '\0';
                long_str.buffer = new_buf;
                long_str.len = new_len;
                long_str.capacity = new_len;
                is_long = true;
            }
        }
        return *this;
    }

    string& string::operator+=(const string& other) {
        return *this += other.c_str();
    }

    char& string::operator[](size_t index) {
        return data_ptr()[index];
    }

    const char& string::operator[](size_t index) const {
        return data_ptr()[index];
    }

    void string::clear() {
        if (is_long) {
            delete[] long_str.buffer;
            is_long = false;
        }
        sso.len = 0;
        sso.buffer[0] = '\0';
    }

    bool string::empty() const {
        return length_val() == 0;
    }

    const char* string::c_str() const {
        return data_ptr() ? data_ptr() : "";
    }

    size_t string::size() const {
        return length_val();
    }

    size_t string::length() const {
        return length_val();
    }

    string_view string::view() const {
        return string_view(this->c_str(), this->size());
    }

    string::operator bool() const {
        return length_val() > 0;
    }

    void string::push_back(char c) {
        *this += c;
    }

    void string::append(const char* str) {
        *this += str;
    }

    void string::append(const string& str) {
        *this += str;
    }

    string string::substr(size_t pos, size_t count) const {
        if (pos >= length_val()) return string();
        if (count == size_t(-1) || pos + count > length_val()) {
            count = length_val() - pos;
        }
        if (count <= SSO_CAP) {
            string result;
            memcpy(result.sso.buffer, data_ptr() + pos, count);
            result.sso.buffer[count] = '\0';
            result.sso.len = static_cast<unsigned char>(count);
            return result;
        } else {
            char* new_buf = new char[count + 1];
            memcpy(new_buf, data_ptr() + pos, count);
            new_buf[count] = '\0';
            string result;
            result.long_str.buffer = new_buf;
            result.long_str.len = count;
            result.long_str.capacity = count;
            result.is_long = true;
            return result;
        }
    }

    size_t string::find_first_of(const char* chars, size_t pos) const {
        if (!chars) return npos;
        for (size_t i = pos; i < length_val(); ++i) {
            for (const char* c = chars; *c; ++c) {
                if (data_ptr()[i] == *c) return i;
            }
        }
        return npos;
    }

    size_t string::find_last_of(const char* chars, size_t pos) const {
        if (!chars || length_val() == 0) return npos;
        size_t start = (pos >= length_val()) ? length_val() - 1 : pos;
        for (size_t i = start; i != size_t(-1); --i) {
            for (const char* c = chars; *c; ++c) {
                if (data_ptr()[i] == *c) return i;
            }
        }
        return npos;
    }

    size_t string::find_last_of(char ch, size_t pos) const {
        if (length_val() == 0) return npos;
        size_t start = (pos >= length_val()) ? length_val() - 1 : pos;
        for (size_t i = start; i != size_t(-1); --i) {
            if (data_ptr()[i] == ch) return i;
        }
        return npos;
    }

    void string::erase(size_t index, size_t count) {
        if (index >= length_val()) return;
        if (count == size_t(-1) || index + count > length_val()) {
            count = length_val() - index;
        }
        if (count == 0) return;

        size_t new_len = length_val() - count;
        if (is_long) {
            char* new_buf = nullptr;
            if (new_len > SSO_CAP) {
                new_buf = new char[new_len + 1];
                if (index > 0) {
                    memcpy(new_buf, long_str.buffer, index);
                }
                if (index + count < long_str.len) {
                    memcpy(new_buf + index, long_str.buffer + index + count, long_str.len - (index + count));
                }
                new_buf[new_len] = '\0';
                delete[] long_str.buffer;
                long_str.buffer = new_buf;
                long_str.len = new_len;
                long_str.capacity = new_len;
            } else {
                char temp[SSO_CAP + 1];
                if (index > 0) {
                    memcpy(temp, long_str.buffer, index);
                }
                if (index + count < long_str.len) {
                    memcpy(temp + index, long_str.buffer + index + count, long_str.len - (index + count));
                }
                temp[new_len] = '\0';
                delete[] long_str.buffer;
                memcpy(sso.buffer, temp, new_len + 1);
                sso.len = static_cast<unsigned char>(new_len);
                is_long = false;
            }
        } else {
            if (new_len <= SSO_CAP) {
                char temp[SSO_CAP + 1];
                if (index > 0) {
                    memcpy(temp, sso.buffer, index);
                }
                if (index + count < sso.len) {
                    memcpy(temp + index, sso.buffer + index + count, sso.len - (index + count));
                }
                temp[new_len] = '\0';
                memcpy(sso.buffer, temp, new_len + 1);
                sso.len = static_cast<unsigned char>(new_len);
            } else {
                char* new_buf = new char[new_len + 1];
                if (index > 0) {
                    memcpy(new_buf, sso.buffer, index);
                }
                if (index + count < sso.len) {
                    memcpy(new_buf + index, sso.buffer + index + count, sso.len - (index + count));
                }
                new_buf[new_len] = '\0';
                long_str.buffer = new_buf;
                long_str.len = new_len;
                long_str.capacity = new_len;
                is_long = true;
            }
        }
    }

    size_t string::find_first_not_of(const char* chars, size_t pos) const {
        if (!chars) return pos < length_val() ? pos : npos;
        for (size_t i = pos; i < length_val(); ++i) {
            bool found = false;
            for (const char* c = chars; *c; ++c) {
                if (data_ptr()[i] == *c) {
                    found = true;
                    break;
                }
            }
            if (!found) return i;
        }
        return npos;
    }

    size_t string::find_last_not_of(const char* chars, size_t pos) const {
        if (length_val() == 0) return npos;
        size_t start = (pos == size_t(-1) || pos >= length_val()) ? length_val() - 1 : pos;
        for (size_t i = start; i != size_t(-1); --i) {
            bool found = false;
            for (const char* c = chars; *c; ++c) {
                if (data_ptr()[i] == *c) {
                    found = true;
                    break;
                }
            }
            if (!found) return i;
        }
        return npos;
    }

    string& string::replace(size_t pos, size_t count, const char* str) {
        if (pos > length_val()) return *this;
        if (count > length_val() - pos) count = length_val() - pos;
        size_t str_len = str ? strlen(str) : 0;
        size_t new_len = length_val() - count + str_len;
        char* new_buf = new char[new_len + 1];
        if (pos > 0) {
            memcpy(new_buf, data_ptr(), pos);
        }
        if (str_len > 0) {
            memcpy(new_buf + pos, str, str_len);
        }
        if (pos + count < length_val()) {
            memcpy(new_buf + pos + str_len, data_ptr() + pos + count, length_val() - (pos + count));
        }
        new_buf[new_len] = '\0';
        
        if (is_long) {
            delete[] long_str.buffer;
        }
        
        if (new_len <= SSO_CAP) {
            memcpy(sso.buffer, new_buf, new_len + 1);
            sso.len = static_cast<unsigned char>(new_len);
            is_long = false;
            delete[] new_buf;
        } else {
            long_str.buffer = new_buf;
            long_str.len = new_len;
            long_str.capacity = new_len;
            is_long = true;
        }
        return *this;
    }

    string& string::replace(size_t pos, size_t count, const string& str) {
        return replace(pos, count, str.c_str());
    }

    string& string::replace_all(const char* from, const char* to) {
        if (!from || !*from) return *this;
        size_t from_len = strlen(from);
        size_t to_len = to ? strlen(to) : 0;
        size_t count = 0;
        size_t pos = 0;
        while ((pos = find(from, pos)) != size_t(-1)) {
            ++count;
            pos += from_len;
        }
        if (count == 0) return *this;
        size_t new_len = length_val() + count * (to_len - from_len);
        char* new_buf = new char[new_len + 1];
        size_t src_pos = 0;
        size_t dst_pos = 0;
        pos = 0;
        while ((pos = find(from, src_pos)) != size_t(-1)) {
            size_t chunk_len = pos - src_pos;
            if (chunk_len > 0) {
                memcpy(new_buf + dst_pos, data_ptr() + src_pos, chunk_len);
                dst_pos += chunk_len;
            }
            if (to_len > 0) {
                memcpy(new_buf + dst_pos, to, to_len);
                dst_pos += to_len;
            }
            src_pos = pos + from_len;
        }
        if (src_pos < length_val()) {
            memcpy(new_buf + dst_pos, data_ptr() + src_pos, length_val() - src_pos);
            dst_pos += length_val() - src_pos;
        }
        new_buf[new_len] = '\0';
        
        if (is_long) {
            delete[] long_str.buffer;
        }
        
        if (new_len <= SSO_CAP) {
            memcpy(sso.buffer, new_buf, new_len + 1);
            sso.len = static_cast<unsigned char>(new_len);
            is_long = false;
            delete[] new_buf;
        } else {
            long_str.buffer = new_buf;
            long_str.len = new_len;
            long_str.capacity = new_len;
            is_long = true;
        }
        return *this;
    }

    string& string::replace(char from, char to) {
        for (size_t i = 0; i < length_val(); ++i) {
            if (data_ptr()[i] == from) data_ptr()[i] = to;
        }
        return *this;
    }

    size_t string::find(char ch, size_t pos) const {
        for (size_t i = pos; i < length_val(); ++i) {
            if (data_ptr()[i] == ch) return i;
        }
        return size_t(-1);
    }

    size_t string::find(const char* str, size_t pos) const {
        if (!str || !*str) return pos;
        size_t str_len = strlen(str);
        if (str_len > length_val()) return size_t(-1);
        for (size_t i = pos; i <= length_val() - str_len; ++i) {
            if (memcmp(data_ptr() + i, str, str_len) == 0) return i;
        }
        return size_t(-1);
    }

    size_t string::find(const string& str, size_t pos) const {
        return find(str.c_str(), pos);
    }

    size_t string::rfind(char ch, size_t pos) const {
        if (pos == size_t(-1) || pos >= length_val()) pos = length_val() - 1;
        for (size_t i = pos; i != size_t(-1); --i) {
            if (data_ptr()[i] == ch) return i;
        }
        return size_t(-1);
    }

    size_t string::rfind(const char* str, size_t pos) const {
        if (!str || !*str) return pos == size_t(-1) ? length_val() : pos;
        size_t str_len = strlen(str);
        if (str_len > length_val()) return size_t(-1);
        if (pos == size_t(-1) || pos > length_val() - str_len) {
            pos = length_val() - str_len;
        }
        for (size_t i = pos; i != size_t(-1); --i) {
            if (memcmp(data_ptr() + i, str, str_len) == 0) return i;
        }
        return size_t(-1);
    }

    size_t string::rfind(const string& str, size_t pos) const {
        return rfind(str.c_str(), pos);
    }

    string operator+(const string& lhs, char rhs) {
        string result = lhs;
        result += rhs;
        return result;
    }

    string operator+(const string& lhs, const char* rhs) {
        string result = lhs;
        result += rhs;
        return result;
    }

    string operator+(const string& lhs, const string& rhs) {
        string result = lhs;
        result += rhs;
        return result;
    }

    string operator+(char lhs, const string& rhs) {
        string result(1, lhs);
        result += rhs;
        return result;
    }

    string operator+(const char* lhs, const string& rhs) {
        string result(lhs);
        result += rhs;
        return result;
    }
}//namespace rgl