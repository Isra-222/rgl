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
//ostream

#pragma once

#include "../string/format.h"
#include <unistd.h>

namespace rgl {
    class ostream {
    protected:
        int fd;
    private:    
        static constexpr size_t BUFFER_SIZE = 4096;
        char buffer[BUFFER_SIZE];
        size_t pos = 0;

        void buffer_char(char c) {
            if (pos >= BUFFER_SIZE) flush_buffer();
            buffer[pos++] = c;
        }

        void _write(const char* str, size_t len) {
            for (size_t i = 0; i < len; ++i) {
                put(str[i]);
            }
        }

        void print_unsigned(unsigned long long value, int base = 10, bool uppercase = false) {
            if (value == 0) {
                put('0');
                return;
            }
            
            char temp[65];
            int index = 0;
            
            while (value > 0) {
                int digit = value % base;
                temp[index++] = (digit < 10) ? ('0' + digit) : (uppercase ? ('A' + digit - 10) : ('a' + digit - 10));
                value /= base;
            }
            
            while (index > 0) {
                put(temp[--index]);
            }
        }
        
        void print_signed(long long value) {
            if (value < 0) {
                put('-');
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
        explicit ostream(int file_descriptor = 1) : fd(file_descriptor) {}
        virtual ~ostream() { flush_buffer(); }

        void flush_buffer() {
            if (pos > 0 && fd != -1) {
                ::write(fd, buffer, pos);
                pos = 0;
            }
        }

        ostream& set_base(FormatFlag base) {
            flush_buffer();
            current_base = base;
            return *this;
        }
        
        ostream& set_uppercase(bool uppercase) {
            flush_buffer();
            uppercase_hex = uppercase;
            return *this;
        }
        
        ostream& set_show_base(bool show) {
            flush_buffer();
            show_base_prefix = show;
            return *this;
        }
        
        ostream& set_show_plus(bool show) {
            flush_buffer();
            show_plus = show;
            return *this;
        }

        ostream& operator<<(char c) {
            put(c);
            return *this;
        }
        
        ostream& operator<<(signed char c) {
            put(static_cast<char>(c));
            return *this;
        }
        
        ostream& operator<<(unsigned char c) {
            put(static_cast<char>(c));
            return *this;
        }

        ostream& operator<<(const char* str) {
            while (*str) put(*str++);
            return *this;
        }
        
        ostream& operator<<(char* str) {
            while (*str) put(*str++);
            return *this;
        }
        
        ostream& operator<<(const rgl::string& str) {
            _write(str.c_str(), str.length());
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
                    put('-');
                    value = -value;
                } else if (show_plus && current_base == FormatFlag::DEC) {
                    put('+');
                }
                print_unsigned(static_cast<unsigned long long>(value), 
                              static_cast<int>(current_base), uppercase_hex);
            } else {
                if (show_plus && value >= 0) put('+');
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
            char temp[64];
            int len = double_to_buffer(temp, value);    
            write(temp, len); 
            flush_buffer();
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
            buffer_char(c);
            return *this;
        }
        
        ostream& write(const char* str, size_t count) {
            _write(str, count);
            return *this;
        }
        
        template<typename Func>
        ostream& operator<<(Func func) {
            return *this << reinterpret_cast<const void*>(func);
        }
    };

    inline ostream& endl(ostream& os) {
        os << '\n';
        os.flush_buffer();
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
}