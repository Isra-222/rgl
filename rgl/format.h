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

//format

#pragma once

#include "string.h"

namespace rgl {


    inline double strtod(const char* str) {
        double val = 0.0;
        double divisor = 1.0;
        bool negative = false;

        if (*str == '-') {
            negative = true;
            str++;
        }

        while (is_digit(*str)) {
            val = val * 10.0 + (*str - '0');
            str++;
        }

        if (*str == '.') {
            str++;
            while (is_digit(*str)) {
                val = val * 10.0 + (*str - '0');
                divisor *= 10.0;
                str++;
            }
        }

        val /= divisor;
        return negative ? -val : val;
    }

    inline long long atoll_custom(const char* str) {
        long long val = 0;
        bool negative = false;
        if (*str == '-') {
            negative = true;
            str++;
        }
        while (*str && is_digit(*str)) {
            val = val * 10 + (*str - '0');
            str++;
        }
        return negative ? -val : val;
    }

    inline long long stoll(const string& str) {
        return atoll_custom(str.c_str());
    }

    inline int stoi(const string& str) {
        return static_cast<int>(stoll(str));
    }

    inline double stod(const string& str) {
        return strtod(str.c_str());
    }


    inline string ToString(int i) {
        char buf[16];
        int pos = 0;
        bool negative = false;
        
        if (i == 0) {
            buf[pos++] = '0';
        } else {
            if (i < 0) {
                negative = true;
                i = -i;
            }
            while (i > 0) {
                buf[pos++] = (i % 10) + '0';
                i /= 10;
            }
            if (negative) {
                buf[pos++] = '-';
            }
        }
        buf[pos] = '\0';

        for (int j = 0; j < pos / 2; ++j) {
            char temp = buf[j];
            buf[j] = buf[pos - 1 - j];
            buf[pos - 1 - j] = temp;
        }
        return string(buf);
    }
    
    inline string ToString(unsigned int i) {
        char buf[16];
        int pos = 0;
        if (i == 0) buf[pos++] = '0';
        while (i > 0) {
            buf[pos++] = (i % 10) + '0';
            i /= 10;
        }
        buf[pos] = '\0';
        for (int j = 0; j < pos / 2; ++j) {
            char temp = buf[j];
            buf[j] = buf[pos - 1 - j];
            buf[pos - 1 - j] = temp;
        }
        return string(buf);
    }

    inline string ToString(long long i) {
        char buf[32];
        int pos = 0;
        bool negative = false;
        
        if (i == 0) {
            buf[pos++] = '0';
        } else {
            if (i < 0) {
                negative = true;
                i = -i;
            }
            while (i > 0) {
                buf[pos++] = (i % 10) + '0';
                i /= 10;
            }
            if (negative) {
                buf[pos++] = '-';
            }
        }
        buf[pos] = '\0';

        for (int j = 0; j < pos / 2; ++j) {
            char temp = buf[j];
            buf[j] = buf[pos - 1 - j];
            buf[pos - 1 - j] = temp;
        }
        return string(buf);
    }

    inline string ToString(unsigned long long i) {
        char buf[32];
        int pos = 0;
        if (i == 0) buf[pos++] = '0';
        while (i > 0) {
            buf[pos++] = (i % 10) + '0';
            i /= 10;
        }
        buf[pos] = '\0';
        for (int j = 0; j < pos / 2; ++j) {
            char temp = buf[j];
            buf[j] = buf[pos - 1 - j];
            buf[pos - 1 - j] = temp;
        }
        return string(buf);
    }

    inline string to_string(const char* str) {
        return string(str);
    }

    inline string to_string(const string& str) {
        return str;
    }

    inline string to_string(char c) {
        return string(1, c);
    }

    inline string to_string(int i) {
        return ToString(i);
    }

    inline string to_string(unsigned int i) {
        return ToString(i);
    }

    inline string to_string(long i) {
        return ToString(static_cast<long long>(i));
    }

    inline string to_string(unsigned long i) {
        char buf[32];
        int pos = 0;
        if (i == 0) buf[pos++] = '0';
        while (i > 0) {
            buf[pos++] = (i % 10) + '0';
            i /= 10;
        }
        buf[pos] = '\0';
        for (int j = 0; j < pos / 2; ++j) {
            char temp = buf[j];
            buf[j] = buf[pos - 1 - j];
            buf[pos - 1 - j] = temp;
        }
        return string(buf);
    }

    inline string to_string(long long i) {
        return ToString(i);
    }

    inline string to_string(unsigned long long i) {
        char buf[32];
        int pos = 0;
        if (i == 0) buf[pos++] = '0';
        while (i > 0) {
            buf[pos++] = (i % 10) + '0';
            i /= 10;
        }
        buf[pos] = '\0';
        for (int j = 0; j < pos / 2; ++j) {
            char temp = buf[j];
            buf[j] = buf[pos - 1 - j];
            buf[pos - 1 - j] = temp;
        }
        return string(buf);
    }

    inline string to_string(double d) {
        char buf[64];
        int pos = 0;
        if (d < 0) {
            buf[pos++] = '-';
            d = -d;
        }
        
        long long int_part = static_cast<long long>(d);
        double frac_part = d - int_part;
        
        if (int_part == 0) {
            buf[pos++] = '0';
        } else {
            char temp[32];
            int temp_pos = 0;
            while (int_part > 0) {
                temp[temp_pos++] = (int_part % 10) + '0';
                int_part /= 10;
            }
            for (int i = temp_pos - 1; i >= 0; --i) {
                buf[pos++] = temp[i];
            }
        }

        buf[pos++] = '.';
        for (int i = 0; i < 6; ++i) { 
            frac_part *= 10;
            int digit = static_cast<int>(frac_part);
            buf[pos++] = digit + '0';
            frac_part -= digit;
            if (frac_part < 0.000001) break;
        }
        
        buf[pos] = '\0';
        return string(buf);
    }

    inline string to_string(float f) {
        return to_string(static_cast<double>(f));
    }

    inline string to_string(bool b) {
        return b ? string("true") : string("false");
    }

    inline string to_string(void*) {
        return string("nullptr");
    }

    template<typename T>
    inline string to_string(T* ptr) {
        if (ptr == nullptr) return string("nullptr");
        char buf[32];
        buf[0] = '0';
        buf[1] = 'x';
        uintptr_t val = reinterpret_cast<uintptr_t>(ptr);
        int pos = 2;
        for (int i = sizeof(uintptr_t) * 2 - 1; i >= 0; --i) {
            int nibble = (val >> (i * 4)) & 0xF;
            buf[pos++] = (nibble < 10) ? (nibble + '0') : (nibble - 10 + 'a');
        }
        buf[pos] = '\0';
        return string(buf);
    }
}