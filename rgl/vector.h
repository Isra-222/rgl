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

//vector
#pragma once

#include "types.h"
#include "utility.h"
#include "new.h"

namespace rgl {
	template<typename T>
	class vector final{
		size_t _size;
		size_t _capacity;
		T* ptr;
	
		void reallocate(size_t qty) {
            T* tmp = static_cast<T*>(::operator new[](qty * sizeof(T)));
            
            for(size_t i = 0; i < _size; ++i) {
                new (&tmp[i]) T(rgl::move(ptr[i]));
                ptr[i].~T();
            }

            ::operator delete[](ptr);
            ptr = tmp;
            _capacity = qty;
        }
	public:
		~vector(){
			clear();
			::operator delete[](ptr);
		}
		
		vector() : _size(0), _capacity(0), ptr(nullptr) {}
		vector(size_t s) : _size(0), _capacity(s), 
            ptr(s > 0 ? static_cast<T*>(::operator new[](s * sizeof(T))) : nullptr) {}

		vector(vector&& other) noexcept
		 : _size(other._size), _capacity(other._capacity), ptr(other.ptr) {
		 	other._size = 0;
		 	other._capacity = 0;
		 	other.ptr = nullptr;
		}

		vector& operator=(vector&& other) noexcept {
            if(this != &other) {
                clear();
                ::operator delete[](ptr);

                ptr = other.ptr;
                _size = other._size;
                _capacity = other._capacity;

                other.ptr = nullptr;
                other._size = 0;
                other._capacity = 0;
            }
            return *this;
        }
		vector(const vector&) = delete;

		void push_back(const T& value) {
            if (_size == _capacity) reserve(_capacity == 0 ? 1 : _capacity * 2);
            new (&ptr[_size]) T(value);
            _size++;
        }

		void push_back(T&& value) {
		    if (_size == _capacity) {
		        reserve(_capacity == 0 ? 1 : _capacity * 2);
		    }
			new (&ptr[_size]) T(rgl::move(value));
			_size++;
		}
		void reserve(size_t s){
			if(s > _capacity) reallocate(s);
		}

		size_t size() const {
			return _size;
		}
		template<typename... Args>
		void emplace_back(Args&&... args) {
		    if (_size == _capacity) {
		        reserve(_capacity == 0 ? 1 : _capacity * 2);
		    }
		    new (&ptr[_size]) T(rgl::forward<Args>(args)...);
		    _size++;
		}
		void clear() {
		    for (size_t i = 0; i < _size; ++i) {
		        ptr[i].~T();
		    }
		    _size = 0;
		}
		void pop_back() {
 			if(_size > 0)
 				ptr[--_size].~T();
        }
        bool empty() const {
        	return _size == 0;
        }


		T& operator[](size_t index){ return ptr[index]; }
		const T& operator[](size_t index) const { return ptr[index]; }

		using iterator = T*;
		using const_iterator = const T*;

		iterator begin() { return ptr; }
		iterator end() { return ptr + _size; }

		const_iterator begin() const { return ptr; }
		const_iterator end() const { return ptr + _size; }
	};

	//Array
	template<typename T, size_t N>
	class array{
		T data[N];
	public:
		constexpr size_t size() const { return N; }
		T& operator[](size_t index){ return data[index]; }
		const T& operator[](size_t index) const { return data[index]; }

		using iterator = T*;
		using const_iterator = const T*;

		iterator begin() { return data; }
    	iterator end() { return data + N; }

    	const_iterator begin() const { return data; }
    	const_iterator end() const { return data + N; }

    	T* data_ptr() { return data;}
	};
};