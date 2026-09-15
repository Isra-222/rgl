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

//string.h
#pragma once

#include "rgl/core/hash.h"
#include "string_view.h"
#include "rgl/core/utility/memory.h"
#include "rgl/core/ctype.h"
#include "rgl/memory/new.h"
#include "rgl/core/utility/move.h"

namespace rgl {

    class string {
        static constexpr size_t SSO_CAP = 15;
    
        struct ShortString {
            char buffer[SSO_CAP + 1];
            uint8_t len;
        };

        struct LongString {
            char* buffer;
            size_t len;
            size_t capacity;
        };

        union {
            ShortString sso;
            LongString long_str;
        };
        bool is_long = false;

        char* allocate_for(size_t required_len);
    public:
        static constexpr size_t npos = ~static_cast<size_t>(0);

        string();
        string(const char* str, size_t __size);
        string(const string_view& sv);
        string(const char* str);
        string(size_t count, char ch);
        string(string&& other) noexcept;
        string(const string& other);
        
        template <size_t N>
        string(const char (&str)[N]) {
            char* dest = allocate_for(N - 1);
            if (str && N > 0)
            memcpy(dest, str, N);
        }

        ~string();

        string& operator=(string&& other) noexcept;
        string& operator=(const string& other);
        string& operator=(const char* str);
        string& operator=(const string_view other);
        template<size_t N>
        string& operator=(const char (&str)[N]){
            char* currentBuf = allocate_for(N - 1);
            memcpy(currentBuf, str, N);
            return *this;
        }

        bool operator==(const string& other) const;
        bool operator==(const char* str) const;
        bool operator==(const string_view& other) const { return *this == other.data(); }

        void clear() {
            if (is_long) {
                long_str.len = 0;
                long_str.buffer[0] = '\0';
            } else {
                sso.len = 0;
                sso.buffer[0] = '\0';
            }
        }
        void shrink_to_fit();

        bool empty() const{ return length() == 0; }
        void reserve(size_t new_len);
        void resize(size_t count, char c);
        void resize(size_t count) { return resize(count, '\0'); }


        char* data() { return is_long ? long_str.buffer : sso.buffer; }
        const char* data() const { return is_long ? long_str.buffer : sso.buffer; }
        const char* c_str() const { return data(); }

        size_t length() const { return is_long ? long_str.len : sso.len; }
        size_t capacity() const { return is_long ? long_str.capacity : SSO_CAP; }
        size_t size() const { return length(); }
        
        string_view view() const { return string_view(data(), length()); }

        char& operator[](size_t index) { return data()[index]; }
        const char& operator[](size_t index) const { return data()[index]; }
        operator bool() const { return length() > 0; }

        string& push_back(const char c);

        string& append(const char* str, size_t size);
        string& append(const char* str) { return append(str, (str)? strlen(str) : 0); }
        string& append(const string& str) { return append(str.data(), str.length()); }
        string& append(const string_view str) { return append(str.data(), str.length()); }

        string& operator+=(const char c) { return push_back(c); }
        string& operator+=(const char* str) { return append(str); }
        string& operator+=(const string& other) { return append(other); }
        string& operator+=(const string_view other) { return append(other); }

        string substr(size_t pos = 0, size_t count = npos) const;
        void erase(size_t index, size_t count = npos);

        size_t find_first_of(const char* chars, size_t pos = 0) const;
        size_t find_last_of(const char* chars, size_t pos = npos) const;

        size_t find_first_of(const string_view& chars, size_t pos = 0) const { return find_first_of(chars.data(), pos); }
        size_t find_last_of(const string_view& chars, size_t pos = npos) const { return find_last_of(chars.data(), pos); }

        size_t find_first_of(char ch, size_t pos = 0) const { return find(ch, pos); }
        size_t find_last_of(char ch, size_t pos = npos) const { return rfind(ch, pos); }
        
        size_t find_first_not_of(const char* chars, size_t pos = 0) const;
        size_t find_last_not_of(const char* chars, size_t pos = npos) const;

        size_t find_first_not_of(const string_view& chars, size_t pos = 0) const { return find_first_not_of(chars.data(), pos); }
        size_t find_last_not_of(const string_view& chars, size_t pos = npos) const { return find_last_not_of(chars.data(), pos); }

        size_t find_first_of(const string& str, size_t pos = 0) const { return find_first_of(str.data(), pos); }
        size_t find_last_of(const string& str, size_t pos = npos) const { return find_last_of(str.data(), pos); }

        size_t find_first_not_of(const string& str, size_t pos = 0) const { return find_first_not_of(str.data(), pos); }
        size_t find_last_not_of(const string& str, size_t pos = npos) const { return find_last_not_of(str.data(), pos); }


        size_t find(char ch, size_t pos = 0) const{
            for (size_t i = pos; i < length(); ++i)
                if (data()[i] == ch) return i;
            return size_t(-1);
        }

        size_t find(const char* str, size_t pos, size_t str_len) const;
        size_t find(const char* str, size_t pos) const { return find(str, pos, str ? strlen(str) : 0); }
        size_t find(const string& str, size_t pos) const { return find(str.c_str(), pos, str.length()); }
        size_t find(const string_view str, size_t pos) const { return find(str.data(), pos, str.size()); }
        

        size_t rfind(char ch, size_t pos = npos) const{
            if (pos == size_t(-1) || pos >= length()) pos = length() - 1;
            for (size_t i = pos; i != size_t(-1); --i) 
                if (data()[i] == ch) return i;
            return size_t(-1);
        }
        size_t rfind(const char* str, size_t pos, size_t len) const;
        size_t rfind(const char* str, size_t pos) const { return rfind(str, pos, str ? strlen(str) : npos); }
        size_t rfind(const string& str, size_t pos) const { return rfind(str.data(), pos, str.size()); }
        size_t rfind(const string_view str, size_t pos) const { return rfind(str.data(), pos, str.size()); }


        string& replace(char from, char to){
            for (size_t i = 0; i < length(); ++i)
                if (data()[i] == from) data()[i] = to;
            return *this;
        }
        string& replace(size_t pos, size_t count, const char* str, size_t len);
        string& replace(size_t pos, size_t count, const char* str) { return replace(pos, count, str, str ? strlen(str) : 0); }
        string& replace(size_t pos, size_t count, const string& str){ return replace(pos, count, str.c_str(), str.length()); }
        string& replace(size_t pos, size_t count, const string_view str) { return replace(pos, count, str.data(), str.size()); }
        
        string& replace_all(const char* from, size_t from_len, const char* to, size_t to_len);
        string& replace_all(const char* from, const char* to) { return replace_all(from, from ? strlen(from) : 0, to, to ? strlen(to) : 0); }
        string& replace_all(const string_view& from, const string_view& to) { return replace_all(from.data(), from.size(), to.data(), to.size()); }
        string& replace_all(const char* from, const string_view& to) { return replace_all(from, from ? strlen(from) : 0, to.data(), to.size()); }
        string& replace_all(const string_view& from, const char* to) { return replace_all(from.data(), from.size(), to, to ? strlen(to) : 0); }
    
        string& insert(size_t index, const char* str, size_t len) { return replace(index, 0, str, len); }
        string& insert(size_t index, const char* str) { return replace(index, 0, str); }
        string& insert(size_t index, const string& str) { return replace(index, 0, str); }
        string& insert(size_t index, string_view str) { return replace(index, 0, str); }
        string& insert(size_t index, size_t count, char ch) {
            string temp(count, ch);
            return replace(index, 0, temp.data(), count);
        }

    };
    string operator+(const string& lhs, const string& rhs);
    string operator+(const string& lhs, const char* rhs);
    string operator+(const char* lhs, const string& rhs);
    string operator+(const string& lhs, char rhs);
    string operator+(char lhs, const string& rhs);
    string operator+(const string& lhs, rgl::string_view rhs);
    string operator+(rgl::string_view lhs, const string& rhs);
    string operator+(rgl::string_view lhs, rgl::string_view rhs);
}//namespace rgl