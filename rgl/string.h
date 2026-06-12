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


//string
#pragma once

#include "string_view.h"
#include "new.h"
#include "utility.h"

namespace rgl {

	class string {
		static const size_t npos = size_t(-1);
		char* buffer;
		size_t len;

		void reserve(size_t new_len) {
			if (new_len > len) {
				char* new_buffer = new char[new_len + 1];
				if (buffer) {
					memcpy(new_buffer, buffer, len);
					delete[] buffer;
				}
				buffer = new_buffer;
				buffer[len] = '\0';
			}
		}
		
	public:
		string() : buffer(nullptr), len(0) {}

		string(const string_view& sv);

		string(const char* str) : len(str? strlen(str) : 0){
			if(len > 0){
				buffer = new char[len + 1];
				memcpy(buffer, str, len + 1);
			} else {
				buffer = nullptr;
			}
		}
		
		string(size_t count, char ch) : len(count) {
			if (len > 0) {
				buffer = new char[len + 1];
				for (size_t i = 0; i < len; ++i) {
					buffer[i] = ch;
				}
				buffer[len] = '\0';
			} else {
				buffer = nullptr;
			}
		}
		
		string(string&& other) noexcept : buffer(other.buffer), len(other.len) {
		    other.buffer = nullptr;
			other.len = 0;
		}
		
		string& operator=(string&& other) noexcept {
		    if (this != &other) {
		        delete[] buffer; 
		        buffer = other.buffer;
		        len = other.len;
		        other.buffer = nullptr;
		        other.len = 0;
		    }
		    return *this;
		}
		
	    string(const string& other) : len(other.len) {
            if (other.buffer) {
                buffer = new char[len + 1];
                memcpy(buffer, other.buffer, len + 1);
            } else {
                buffer = nullptr;
            }
        }
        
        string& operator=(const string& other) {
            if (this != &other) {
                delete[] buffer;
                len = other.len;
                if (other.buffer) {
                    buffer = new char[len + 1];
                    memcpy(buffer, other.buffer, len + 1);
                } else {
                    buffer = nullptr;
                }
            }
            return *this;
        }

		string& operator=(const char* str) {
			delete[] buffer;
			len = str ? strlen(str) : 0;
			if (len > 0) {
				buffer = new char[len + 1];
				memcpy(buffer, str, len + 1);
			} else {
				buffer = nullptr;
			}
			return *this;
		}

        bool operator==(const string& other) const {
		    if (len != other.len) return false;
		    for (size_t i = 0; i < len; ++i) {
		        if (buffer[i] != other.buffer[i]) return false;
		    }
		    return true;
		}
		
		bool operator==(const char* str) const {
    		return (len == strlen(str)) && (memcmp(buffer, str, len) == 0);
		}

		string& operator+=(char c) {
			size_t new_len = len + 1;
			char* new_buffer = new char[new_len + 1];
			if (buffer) {
				memcpy(new_buffer, buffer, len);
				delete[] buffer;
			}
			new_buffer[len] = c;
			new_buffer[new_len] = '\0';
			buffer = new_buffer;
			len = new_len;
			return *this;
		}

		string& operator+=(const char* str) {
			if (!str || !*str) return *this;
			size_t str_len = strlen(str);
			size_t new_len = len + str_len;
			char* new_buffer = new char[new_len + 1];
			if (buffer) {
				memcpy(new_buffer, buffer, len);
				delete[] buffer;
			}
			memcpy(new_buffer + len, str, str_len);
			new_buffer[new_len] = '\0';
			buffer = new_buffer;
			len = new_len;
			return *this;
		}

		string& operator+=(const string& other) {
			return *this += other.c_str();
		}

		char& operator[](size_t index) {
			return buffer[index];
		}
		
		const char& operator[](size_t index) const {
			return buffer[index];
		}

		void clear() {
			delete[] buffer;
			buffer = nullptr;
			len = 0;
		}

		bool empty() const {
			return len == 0;
		}

	    ~string() {
        	delete[] buffer; 
    	}

		const char* c_str() const { return buffer ? buffer : ""; }
        size_t size() const { return len; }
        size_t length() const { return len; }
        string_view view() const;

        operator bool() const { return len > 0; }

        void push_back(char c) {
            *this += c;
        }

        void append(const char* str) {
            *this += str;
        }
        
        void append(const string& str) {
            *this += str;
        }

        string substr(size_t pos = 0, size_t count = size_t(-1)) const {
            if (pos >= len) return string();
            if (count == size_t(-1) || pos + count > len) {
                count = len - pos;
            }
            char* new_buf = new char[count + 1];
            memcpy(new_buf, buffer + pos, count);
            new_buf[count] = '\0';
            string result;
            result.buffer = new_buf;
            result.len = count;
            return result;
        }
		size_t find_first_of(const char* chars, size_t pos = 0) const {
		    if (!chars) return npos;
		    
		    for (size_t i = pos; i < len; ++i) {
		        for (const char* c = chars; *c; ++c) {
		            if (buffer[i] == *c) return i;
		        }
		    }
		    return npos;
		}

		string& replace(size_t pos, size_t count, const char* str) {
		    if (pos > len) return *this;

		    if (count > len - pos) count = len - pos;
		    
		    size_t str_len = str ? strlen(str) : 0;
		    size_t new_len = len - count + str_len;
		    
		    char* new_buffer = new char[new_len + 1];

		    if (pos > 0) {
		        memcpy(new_buffer, buffer, pos);
		    }

		    if (str_len > 0) {
		        memcpy(new_buffer + pos, str, str_len);
		    }

		    if (pos + count < len) {
		        memcpy(new_buffer + pos + str_len, buffer + pos + count, len - (pos + count));
		    }
		    
		    new_buffer[new_len] = '\0';
		    
		    delete[] buffer;
		    buffer = new_buffer;
		    len = new_len;
		    
		    return *this;
		}

		string& replace(size_t pos, size_t count, const string& str) {
		    return replace(pos, count, str.c_str());
		}

		string& replace_all(const char* from, const char* to) {
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
		    
		    size_t new_len = len + count * (to_len - from_len);
		    char* new_buffer = new char[new_len + 1];
		    
		    size_t src_pos = 0;
		    size_t dst_pos = 0;
		    pos = 0;
		    
		    while ((pos = find(from, src_pos)) != size_t(-1)) {
		        size_t chunk_len = pos - src_pos;
		        if (chunk_len > 0) {
		            memcpy(new_buffer + dst_pos, buffer + src_pos, chunk_len);
		            dst_pos += chunk_len;
		        }
		        
		        if (to_len > 0) {
		            memcpy(new_buffer + dst_pos, to, to_len);
		            dst_pos += to_len;
		        }
		        
		        src_pos = pos + from_len;
		    }

		    if (src_pos < len) {
		        memcpy(new_buffer + dst_pos, buffer + src_pos, len - src_pos);
		        dst_pos += len - src_pos;
		    }
		    
		    new_buffer[new_len] = '\0';
		    
		    delete[] buffer;
		    buffer = new_buffer;
		    len = new_len;
		    
		    return *this;
		}

		string& replace(char from, char to) {
		    for (size_t i = 0; i < len; ++i) {
		        if (buffer[i] == from) buffer[i] = to;
		    }
		    return *this;
		}

		size_t find(char ch, size_t pos = 0) const {
		    for (size_t i = pos; i < len; ++i) {
		        if (buffer[i] == ch) return i;
		    }
		    return size_t(-1);
		}

		size_t find(const char* str, size_t pos = 0) const {
		    if (!str || !*str) return pos;
		    size_t str_len = strlen(str);
		    if (str_len > len) return size_t(-1);
		    
		    for (size_t i = pos; i <= len - str_len; ++i) {
		        bool found = true;
		        for (size_t j = 0; j < str_len; ++j) {
		            if (buffer[i + j] != str[j]) {
		                found = false;
		                break;
		            }
		        }
		        if (found) return i;
		    }
		    return size_t(-1);
		}

		size_t find(const string& str, size_t pos = 0) const {
		    return find(str.c_str(), pos);
		}

		size_t rfind(char ch, size_t pos = size_t(-1)) const {
		    if (pos == size_t(-1) || pos >= len) pos = len - 1;
		    for (size_t i = pos; i != size_t(-1); --i) {
		        if (buffer[i] == ch) return i;
		    }
		    return size_t(-1);
		}

		size_t rfind(const char* str, size_t pos = size_t(-1)) const {
		    if (!str || !*str) return pos == size_t(-1) ? len : pos;
		    size_t str_len = strlen(str);
		    if (str_len > len) return size_t(-1);
		    
		    if (pos == size_t(-1) || pos > len - str_len) {
		        pos = len - str_len;
		    }
		    
		    for (size_t i = pos; i != size_t(-1); --i) {
		        bool found = true;
		        for (size_t j = 0; j < str_len; ++j) {
		            if (buffer[i + j] != str[j]) {
		                found = false;
		                break;
		            }
		        }
		        if (found) return i;
		    }
		    return size_t(-1);
		}

		size_t rfind(const string& str, size_t pos = size_t(-1)) const {
		    return rfind(str.c_str(), pos);
		}
	};

	inline string::string(const string_view& sv) : len(sv.size()) {
	    if (len > 0) {
	        buffer = new char[len + 1];
	        memcpy(buffer, sv.data(), len);
	        buffer[len] = '\0';
	    } else {
	        buffer = nullptr;
	    }
	}
	inline string_view string::view() const { 
        return string_view(this->c_str()); 
    }

	inline string operator+(const string& lhs, char rhs) {
		string result = lhs;
		result += rhs;
		return result;
	}
	
	inline string operator+(const string& lhs, const char* rhs) {
		string result = lhs;
		result += rhs;
		return result;
	}
	
	inline string operator+(const string& lhs, const string& rhs) {
		string result = lhs;
		result += rhs;
		return result;
	}
	
	inline string operator+(char lhs, const string& rhs) {
		string result(1, lhs);
		result += rhs;
		return result;
	}
	
	inline string operator+(const char* lhs, const string& rhs) {
		string result(lhs);
		result += rhs;
		return result;
	}
	
};