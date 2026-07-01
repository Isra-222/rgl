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

//ostream.h
#pragma once

#include "rgl/core/types.h"

namespace rgl {
    class string;
    class string_view;

    class ostream {
    protected:
        int fd;
    private:    
        static constexpr size_t BUFFER_SIZE = 4096;
        char buffer[BUFFER_SIZE];
        size_t pos = 0;

        void buffer_char(char c);
        void _write(const char* str, size_t len);
        void print_unsigned(unsigned long long value, int base = 10, bool uppercase = false);
        void print_signed(long long value);

    public:
        enum class FormatFlag {
            DEC = 10,
            HEX = 16,
            OCT = 8,
            BIN = 2
        };
        
    private:
        FormatFlag current_base = FormatFlag::DEC;
        bool uppercase_hex = false;
        bool show_base_prefix = false;
        bool show_plus = false;
        
    public:
        explicit ostream(int file_descriptor = 1);
        virtual ~ostream();

        void flush_buffer();

        ostream& set_base(FormatFlag base);
        ostream& set_uppercase(bool uppercase);
        ostream& set_show_base(bool show);
        ostream& set_show_plus(bool show);

        ostream& operator<<(char c);
        ostream& operator<<(signed char c);
        ostream& operator<<(unsigned char c);
        ostream& operator<<(const char* str);
        ostream& operator<<(char* str);
        ostream& operator<<(const rgl::string& str);
        ostream& operator<<(const rgl::string_view& str);
        ostream& operator<<(short value);
        ostream& operator<<(int value);
        ostream& operator<<(long value);
        ostream& operator<<(long long value);
        ostream& operator<<(unsigned short value);
        ostream& operator<<(unsigned int value);
        ostream& operator<<(unsigned long value);
        ostream& operator<<(unsigned long long value);
        ostream& operator<<(float value);
        ostream& operator<<(double value);
        ostream& operator<<(bool value);
        ostream& operator<<(const void* ptr);
        
        template<typename T>
        ostream& operator<<(T* ptr) {
            return operator<<(static_cast<const void*>(ptr));
        }
        
        ostream& operator<<(nullptr_t);
        ostream& operator<<(ostream& (*manipulator)(ostream&));

        ostream& put(char c);
        ostream& write(const char* str, size_t count);
        
        template<typename Func>
        ostream& operator<<(Func func) {
            return *this << reinterpret_cast<const void*>(func);
        }
    };

    ostream& endl(ostream& os);
    ostream& ends(ostream& os);
    ostream& dec(ostream& os);
    ostream& hex(ostream& os);
    ostream& oct(ostream& os);
    ostream& bin(ostream& os);
    ostream& uppercase(ostream& os);
    ostream& nouppercase(ostream& os);
    ostream& showbase(ostream& os);
    ostream& noshowbase(ostream& os);
    ostream& showpos(ostream& os);
    ostream& noshowpos(ostream& os);
}