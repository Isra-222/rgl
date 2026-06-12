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

//iostream
#pragma once

#include "string.h"

#ifndef EOF
    #define EOF (-1)
#endif

extern "C" {
    int printf(const char* format, ...);
    int putchar(int c);
    int scanf(const char* format, ...);
    int getchar(void);
    int puts(const char* s);
}

namespace rgl {

    class ostream {
    private:
        // Método auxiliar para imprimir enteros sin signo en diferentes bases
        void print_unsigned(unsigned long long value, int base = 10, bool uppercase = false) {
            if (value == 0) {
                putchar('0');
                return;
            }
            
            char buffer[65];
            int index = 0;
            
            while (value > 0) {
                int digit = value % base;
                buffer[index++] = (digit < 10) ? ('0' + digit) : (uppercase ? ('A' + digit - 10) : ('a' + digit - 10));
                value /= base;
            }
            
            while (index > 0) {
                putchar(buffer[--index]);
            }
        }
        
        void print_signed(long long value) {
            if (value < 0) {
                putchar('-');
                value = -value;
            }
            print_unsigned(static_cast<unsigned long long>(value));
        }
        
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
        ostream& set_base(FormatFlag base) {
            current_base = base;
            return *this;
        }
        
        ostream& set_uppercase(bool uppercase) {
            uppercase_hex = uppercase;
            return *this;
        }
        
        ostream& set_show_base(bool show) {
            show_base_prefix = show;
            return *this;
        }
        
        ostream& set_show_plus(bool show) {
            show_plus = show;
            return *this;
        }

        ostream& operator<<(char c) {
            putchar(c);
            return *this;
        }
        
        ostream& operator<<(signed char c) {
            putchar(static_cast<char>(c));
            return *this;
        }
        
        ostream& operator<<(unsigned char c) {
            putchar(static_cast<char>(c));
            return *this;
        }

        ostream& operator<<(const char* str) {
            printf("%s", str);
            return *this;
        }
        
        ostream& operator<<(char* str) {
            printf("%s", str);
            return *this;
        }
        
        ostream& operator<<(const rgl::string& str) {
            printf("%s", str.c_str());
            return *this;
        }
        
        ostream& operator<<(short value) {
            return operator<<(static_cast<long long>(value));
        }
        
        ostream& operator<<(int value) {
            return operator<<(static_cast<long long>(value));
        }
        
        ostream& operator<<(long value) {
            return operator<<(static_cast<long long>(value));
        }
        
        ostream& operator<<(long long value) {
            if (current_base != FormatFlag::DEC) {
                if (show_base_prefix) {
                    switch (current_base) {
                        case FormatFlag::HEX: *this << "0x"; break;
                        case FormatFlag::OCT: *this << "0"; break;
                        case FormatFlag::BIN: *this << "0b"; break;
                        default: break;
                    }
                }
                if (value < 0) {
                    putchar('-');
                    value = -value;
                } else if (show_plus && current_base == FormatFlag::DEC) {
                    putchar('+');
                }
                print_unsigned(static_cast<unsigned long long>(value), 
                              static_cast<int>(current_base), uppercase_hex);
            } else {
                if (show_plus && value >= 0) putchar('+');
                print_signed(value);
            }
            return *this;
        }
        
        ostream& operator<<(unsigned short value) {
            return operator<<(static_cast<unsigned long long>(value));
        }
        
        ostream& operator<<(unsigned int value) {
            return operator<<(static_cast<unsigned long long>(value));
        }
        
        ostream& operator<<(unsigned long value) {
            return operator<<(static_cast<unsigned long long>(value));
        }
        
        ostream& operator<<(unsigned long long value) {
            if (current_base != FormatFlag::DEC) {
                if (show_base_prefix) {
                    switch (current_base) {
                        case FormatFlag::HEX: *this << "0x"; break;
                        case FormatFlag::OCT: *this << "0"; break;
                        case FormatFlag::BIN: *this << "0b"; break;
                        default: break;
                    }
                }
                print_unsigned(value, static_cast<int>(current_base), uppercase_hex);
            } else {
                print_unsigned(value);
            }
            return *this;
        }
        
        ostream& operator<<(float value) {
            return operator<<(static_cast<double>(value));
        }
        
        ostream& operator<<(double value) {
            if (value != value) {
                *this << "nan";
                return *this;
            }
            if (value * 2 == value && value != 0) {
                *this << ((value < 0) ? "-inf" : "inf");
                return *this;
            }
            printf("%g", value);
            return *this;
        }
        
        ostream& operator<<(long double value) {
            printf("%Lg", value);
            return *this;
        }

        ostream& operator<<(bool value) {
            *this << (value ? "true" : "false");
            return *this;
        }

        ostream& operator<<(const void* ptr) {
            FormatFlag old_base = current_base;
            bool old_prefix = show_base_prefix;
            bool old_uppercase = uppercase_hex;
            
            set_base(FormatFlag::HEX);
            set_show_base(true);
            set_uppercase(false);
            
            *this << reinterpret_cast<unsigned long long>(ptr);
            
            set_base(old_base);
            set_show_base(old_prefix);
            set_uppercase(old_uppercase);
            return *this;
        }
        
        template<typename T>
        ostream& operator<<(T* ptr) {
            return operator<<(static_cast<const void*>(ptr));
        }
        
        ostream& operator<<(nullptr_t) {
            *this << "nullptr";
            return *this;
        }
        
        ostream& operator<<(ostream& (*manipulator)(ostream&)) {
            return manipulator(*this);
        }        

        ostream& put(char c) {
            putchar(c);
            return *this;
        }
        
        ostream& write(const char* str, size_t count) {
            for (size_t i = 0; i < count; ++i) {
                putchar(str[i]);
            }
            return *this;
        }
        

        template<typename Func>
        ostream& operator<<(Func func) {
            return *this << reinterpret_cast<const void*>(func);
        }
    };
    
    inline ostream& endl(ostream& os) {
        os << '\n';
        return os;
    }
    
    inline ostream& ends(ostream& os) {
        os << '\0';
        return os;
    }
    
    
    inline ostream& dec(ostream& os) {
        os.set_base(ostream::FormatFlag::DEC);
        return os;
    }
    
    inline ostream& hex(ostream& os) {
        os.set_base(ostream::FormatFlag::HEX);
        return os;
    }
    
    inline ostream& oct(ostream& os) {
        os.set_base(ostream::FormatFlag::OCT);
        return os;
    }
    
    inline ostream& bin(ostream& os) {
        os.set_base(ostream::FormatFlag::BIN);
        return os;
    }

    inline ostream& uppercase(ostream& os) {
        os.set_uppercase(true);
        return os;
    }
    
    inline ostream& nouppercase(ostream& os) {
        os.set_uppercase(false);
        return os;
    }
    
    inline ostream& showbase(ostream& os) {
        os.set_show_base(true);
        return os;
    }
    
    inline ostream& noshowbase(ostream& os) {
        os.set_show_base(false);
        return os;
    }
    
    inline ostream& showpos(ostream& os) {
        os.set_show_plus(true);
        return os;
    }
    
    inline ostream& noshowpos(ostream& os) {
        os.set_show_plus(false);
        return os;
    }
    
    inline ostream out;
    
    class istream {
    private:
        char buffer[1024];
        size_t buffer_pos = 0;
        size_t buffer_size = 0;
        
        char getch() {
            if (buffer_pos < buffer_size) {
                return buffer[buffer_pos++];
            }
            buffer_size = 0;
            buffer_pos = 0;
            int c = getchar();
            if (c == EOF) return '\0';
            return static_cast<char>(c);
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
        
        // Tipos punto flotante
        istream& operator>>(float& value) {
            double tmp;
            *this >> tmp;
            value = static_cast<float>(tmp);
            return *this;
        }
        
        istream& operator>>(double& value) {
            skip_whitespace();
            scanf("%lf", &value);
            return *this;
        }
        
        istream& operator>>(long double& value) {
            skip_whitespace();
            scanf("%Lf", &value);
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
        
        /*
        note: add functions more later
        */
        operator bool() const {
            return true; 
        }
        
        bool eof() const {
            return false; 
        }
    };
    
    inline istream in;
}