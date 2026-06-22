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
//istream
#pragma once

#include <unistd.h>

#include "../string/format.h"

#ifndef EOF
    #define EOF (-1)
#endif

namespace rgl{
    class istream {
        char buffer[1024];
        size_t buffer_pos = 0;
        size_t buffer_size = 0;
        bool is_good = true;
        int fd;
        
        char getch() {
            if(buffer_pos < buffer_size)
                return buffer[buffer_pos++];
            
            ssize_t bytes = ::read(fd, buffer, sizeof(buffer));
            if(bytes <= 0) {
                is_good = false;
                return '\0';
            }
            
            buffer_size = static_cast<size_t>(bytes);
            buffer_pos = 0;
            return buffer[buffer_pos++];
        }
        
        void ungetch(char c) {
            if (buffer_pos > 0) {
                buffer[--buffer_pos] = c;
            }
        }
        
        void skip_whitespace() {
            char c;
            while ((c = getch()) != '\0') {
                if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                    ungetch(c);
                    break;
                }
            }
        }
    public:
        explicit istream(int file_descriptor = 0) : fd(file_descriptor) {}
        
        istream& operator>>(char& c) {
            skip_whitespace();
            c = getch();
            return *this;
        }
        
        istream& operator>>(signed char& c) {
            char tmp;
            *this >> tmp;
            c = static_cast<signed char>(tmp);
            return *this;
        }
        
        istream& operator>>(unsigned char& c) {
            char tmp;
            *this >> tmp;
            c = static_cast<unsigned char>(tmp);
            return *this;
        }
        
        istream& operator>>(char* buffer) {
            skip_whitespace();
            size_t pos = 0;
            char c;
            while ((c = getch()) != '\0' && c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                if (pos < 255) {
                    buffer[pos++] = c;
                }
            }
            buffer[pos] = '\0';
            return *this;
        }
        
        istream& operator>>(short& value) {
            int tmp;
            *this >> tmp;
            value = static_cast<short>(tmp);
            return *this;
        }
        
        istream& operator>>(int& value) {
            skip_whitespace();
            bool negative = false;
            value = 0;
            char c = getch();
            
            if (c == '-') {
                negative = true;
                c = getch();
            } else if (c == '+') {
                c = getch();
            }
            
            while (c >= '0' && c <= '9') {
                value = value * 10 + (c - '0');
                c = getch();
            }
            
            if (negative) value = -value;
            ungetch(c);
            return *this;
        }
        
        istream& operator>>(long& value) {
            int tmp;
            *this >> tmp;
            value = static_cast<long>(tmp);
            return *this;
        }
        
        istream& operator>>(long long& value) {
            int tmp;
            *this >> tmp;
            value = static_cast<long long>(tmp);
            return *this;
        }
        
        istream& operator>>(unsigned short& value) {
            int tmp;
            *this >> tmp;
            value = static_cast<unsigned short>(tmp);
            return *this;
        }
        
        istream& operator>>(unsigned int& value) {
            skip_whitespace();
            value = 0;
            char c = getch();
            
            while (c >= '0' && c <= '9') {
                value = value * 10 + static_cast<unsigned int>(c - '0');
                c = getch();
            }
            
            ungetch(c);
            return *this;
        }
        
        istream& operator>>(unsigned long& value) {
            unsigned int tmp;
            *this >> tmp;
            value = static_cast<unsigned long>(tmp);
            return *this;
        }
        
        istream& operator>>(unsigned long long& value) {
            unsigned int tmp;
            *this >> tmp;
            value = static_cast<unsigned long long>(tmp);
            return *this;
        }

        istream& operator>>(float& value) {
            double tmp;
            *this >> tmp;
            value = static_cast<float>(tmp);
            return *this;
        }
        
        istream& operator>>(double& value) {
            skip_whitespace();
            char temp[64];
            size_t i = 0;
            char c = getch();

            while ((c >= '0' && c <= '9') || c == '.' || c == 'e' || c == '-' || c == '+') {
                if (i < 63) temp[i++] = c;
                c = getch();
            }
            temp[i] = '\0';
            ungetch(c);

            value = strtod(temp); 
            return *this;
        }
        
        istream& operator>>(long double& value) {
            skip_whitespace();
            char temp[64];
            size_t i = 0;
            char c = getch();

            while ((c >= '0' && c <= '9') || c == '.' || c == 'e' || c == '-' || c == '+') {
                if (i < 63) temp[i++] = c;
                c = getch();
            }
            temp[i] = '\0';
            ungetch(c);

            value = strtod(temp); 
            return *this;
        }
        
        istream& operator>>(bool& value) {
            char buffer[10];
            *this >> buffer;
            if (buffer[0] == '1' || buffer[0] == 't' || buffer[0] == 'T' || 
                buffer[0] == 'y' || buffer[0] == 'Y') {
                value = true;
            } else if (buffer[0] == '0' || buffer[0] == 'f' || buffer[0] == 'F' || 
                       buffer[0] == 'n' || buffer[0] == 'N') {
                value = false;
            }
            return *this;
        }
        
        istream& operator>>(rgl::string& str) {
            char buffer[1024];
            *this >> buffer;
            str = buffer;
            return *this;
        }
        
        istream& getline(char* buffer, size_t size) {
            size_t pos = 0;
            char c;
            while (pos < size - 1 && (c = getch()) != '\n' && c != '\r' && c != '\0') {
                buffer[pos++] = c;
            }
            buffer[pos] = '\0';
            
            if (c == '\r' && (c = getch()) != '\n') {
                ungetch(c);
            }
            return *this;
        }
        
        istream& getline(rgl::string& str, char delim = '\n') {
            str.clear();
            char c;
            while ((c = getch()) != delim && c != '\0') {
                str += c;
            }
            return *this;
        }

        operator bool() const { return is_good; }
        bool eof() const { return !is_good; }
    };
}