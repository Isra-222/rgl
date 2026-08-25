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
            unsigned char len;
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

        void reserve(size_t new_len);

        char* data_ptr() { return is_long ? long_str.buffer : sso.buffer; }
        const char* data_ptr() const { return is_long ? long_str.buffer : sso.buffer; }
        size_t length_val() const { return is_long ? long_str.len : sso.len; }

    public:
        static const size_t npos = size_t(-1);

        string();
        string(const char* str, size_t __size);
        string(const string_view& sv);
        string(const char* str);
        string(size_t count, char ch);
        string(string&& other) noexcept;
        string(const string& other);
        
        template <size_t N>
        string(const char (&str)[N]) {
            constexpr size_t actual_len = N - 1;
            if constexpr (actual_len <= SSO_CAP) {
                memcpy(sso.buffer, str, actual_len + 1);
                sso.len = static_cast<unsigned char>(actual_len);
                is_long = false;
            } else {
                long_str.buffer = new char[actual_len + 1];
                memcpy(long_str.buffer, str, actual_len + 1);
                long_str.len = actual_len;
                long_str.capacity = actual_len;
                is_long = true;
            }
        }

        ~string();

        string& operator=(string&& other) noexcept;
        string& operator=(const string& other);
        string& operator=(const char* str);

        bool operator==(const string& other) const;
        bool operator==(const char* str) const;

        string& operator+=(char c);
        string& operator+=(const char* str);
        string& operator+=(const string& other);

        char& operator[](size_t index);
        const char& operator[](size_t index) const;

        void clear();
        bool empty() const;

        const char* c_str() const;
        char* data() { return data_ptr(); }
        const char* data() const { return data_ptr(); }
        size_t size() const;
        size_t length() const;
        string_view view() const;

        operator bool() const;

        void push_back(char c);
        void append(const char* str);
        void append(const string& str);

        string substr(size_t pos = 0, size_t count = size_t(-1)) const;
        
        size_t find_first_of(const char* chars, size_t pos = 0) const;
        size_t find_last_of(const char* chars, size_t pos = size_t(-1)) const;
        size_t find_last_of(char ch, size_t pos = size_t(-1)) const;
        
        void erase(size_t index, size_t count = size_t(-1));
        
        size_t find_first_not_of(const char* chars, size_t pos = 0) const;
        size_t find_last_not_of(const char* chars, size_t pos = size_t(-1)) const;
        
        string& replace(size_t pos, size_t count, const char* str);
        string& replace(size_t pos, size_t count, const string& str);
        string& replace_all(const char* from, const char* to);
        string& replace(char from, char to);
        
        size_t find(char ch, size_t pos = 0) const;
        size_t find(const char* str, size_t pos = 0) const;
        size_t find(const string& str, size_t pos = 0) const;
        
        size_t rfind(char ch, size_t pos = size_t(-1)) const;
        size_t rfind(const char* str, size_t pos = size_t(-1)) const;
        size_t rfind(const string& str, size_t pos = size_t(-1)) const;
    };

    string operator+(const string& lhs, char rhs);
    string operator+(const string& lhs, const char* rhs);
    string operator+(const string& lhs, const string& rhs);
    string operator+(char lhs, const string& rhs);
    string operator+(const char* lhs, const string& rhs);

    template <>
    struct hasher<string> {
        inline uint64_t operator()(const rgl::string& key, uint64_t seed) const {
            return wyhash(reinterpret_cast<const uint8_t*>(key.data()), key.size(), seed);
        }
    };
}//namespace rgl