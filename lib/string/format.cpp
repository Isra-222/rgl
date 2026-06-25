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

//format.cpp
#include "rgl/string/string_view.h"
#include "rgl/string/format.h"


namespace rgl {
    long double strtold(const char* str, char** endptr) {
        long double val = 0.0;
        long double divisor = 1.0;
        bool negative = false;

        const char* start = str;

        if (*str == '-') {
            negative = true;
            str++;
        } else if (*str == '+') {
            str++;
        }

        while (is_digit(*str)) {
            val = val * 10.0L + (*str - '0');
            str++;
        }

        if (*str == '.') {
            str++;
            while (is_digit(*str)) {
                val = val * 10.0L + (*str - '0');
                divisor *= 10.0L;
                str++;
            }
        }

        val /= divisor;

        if (endptr != nullptr) {
            if (str == start) {
                *endptr = (char*)start;
            } else {
                *endptr = (char*)str;
            }
        }

        return negative ? -val : val;
    }

    double strtod(const char* str) {
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

    long long atoll_custom(const char* str) {
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

    int double_to_buffer(char* buf, double d) {
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
            while (temp_pos > 0) {
                buf[pos++] = temp[--temp_pos];
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
        return pos;
    }

    long long stoll(const string& str) {
        return atoll_custom(str.c_str());
    }

    int stoi(const string& str) {
        return static_cast<int>(stoll(str));
    }

    double stod(const string& str) {
        return strtod(str.c_str());
    }

    static string int_to_string(long long i) {
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

    static string uint_to_string(unsigned long long i) {
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

    string to_string(int i) {
        return int_to_string(i);
    }

    string to_string(unsigned int i) {
        return uint_to_string(i);
    }

    string to_string(long i) {
        return int_to_string(static_cast<long long>(i));
    }

    string to_string(unsigned long i) {
        return uint_to_string(i);
    }

    string to_string(long long i) {
        return int_to_string(i);
    }

    string to_string(unsigned long long i) {
        return uint_to_string(i);
    }

    string to_string(double d) {
        char buf[64];
        double_to_buffer(buf, d);
        return string(buf);
    }

    string to_string(float f) {
        return to_string(static_cast<double>(f));
    }

    string to_string(bool b) {
        return b ? string("true") : string("false");
    }

    string to_string(const char* str) {
        return string(str);
    }

    string to_string(const string& str) {
        return str;
    }

    string to_string(char c) {
        return string(1, c);
    }

    string to_string(void*) {
        return string("nullptr");
    }
}