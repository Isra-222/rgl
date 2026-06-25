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

//smallVector
#pragma once

#include "rgl/core/types.h"
#include "rgl/core/utility/move.h"
#include "rgl/memory/new.h"

namespace rgl{
	template<typename T, size_t N>
	class smallVector final {
	    alignas(T) char _stack_buffer[N * sizeof(T)];
	    T* _ptr;
	    size_t _size;
	    size_t _capacity;
	    bool _is_heap;

	    T* stack_ptr() { return reinterpret_cast<T*>(_stack_buffer); }
	public:
	    smallVector() : _ptr(stack_ptr()), _size(0), _capacity(N), _is_heap(false) {}

	    ~smallVector() {
	        clear();
	        if (_is_heap) {
	            ::operator delete[](_ptr);
	        }
	    }

	    smallVector(const smallVector&) = delete;
	    smallVector& operator=(const smallVector&) = delete;

	    smallVector& operator=(smallVector&& other) noexcept {
            if (this != &other) {
                clear();
                if (other._is_heap) {
                    _ptr = other._ptr;
                    _capacity = other._capacity;
                    _is_heap = true;
                    other._ptr = other.stack_ptr();
                    other._is_heap = false;
                } else {
                    _ptr = stack_ptr();
                    _capacity = N;
                    _is_heap = false;
                    for(size_t i = 0; i < other._size; ++i) {
                        new (_ptr + i) T(move(other._ptr[i]));
                        other._ptr[i].~T();
                    }
                }
                _size = other._size;
                other._size = 0;
            }
            return *this;
        }

	    smallVector(smallVector&& other) noexcept {
		    if (!other._is_heap) {
		        _size = other._size;
		        _capacity = N;
		        _is_heap = false;
		        _ptr = stack_ptr();
		        for(size_t i = 0; i < _size; ++i) {
		            new (_ptr + i) T(move(other._ptr[i]));
		            other._ptr[i].~T();
		        }
		    } else {
		        _size = other._size;
		        _capacity = other._capacity;
		        _is_heap = true;
		        _ptr = other._ptr;
		        other._ptr = other.stack_ptr();
		        other._is_heap = false;
		        other._size = 0;
		    }
		    other._size = 0;
		}

	    void push_back(const T& value) {
	        if (_size == _capacity) grow();
	        new (_ptr + _size) T(value);
	        _size++;
	    }

	    void grow() {
	        size_t new_capacity = _capacity * 2;
	        T* new_ptr = static_cast<T*>(::operator new[](new_capacity * sizeof(T)));

	        for (size_t i = 0; i < _size; ++i) {
	            new (new_ptr + i) T(move(_ptr[i]));
	            _ptr[i].~T();
	        }

	        if (_is_heap) {
	            ::operator delete[](_ptr);
	        }

	        _ptr = new_ptr;
	        _capacity = new_capacity;
	        _is_heap = true;
	    }

		void clear() {
            for (size_t i = 0; i < _size; ++i) {
                _ptr[i].~T();
            }
            _size = 0;

            if (_is_heap) {
                ::operator delete[](_ptr);
                _ptr = stack_ptr();
                _capacity = N;
                _is_heap = false;
            }
        }

	    T& operator[](size_t index) { return _ptr[index]; }
        const T& operator[](size_t index) const { return _ptr[index]; }

        T& front() { return _ptr[0]; }
        T& back() { return _ptr[_size - 1]; }

        using iterator = T*;
        using const_iterator = const T*;

        iterator begin() { return _ptr; }
        iterator end() { return _ptr + _size; }

        const_iterator begin() const { return _ptr; }
        const_iterator end() const { return _ptr + _size; }

        bool empty() const { return _size == 0; }
        
        void pop_back() {
            if (_size > 0) {
                _ptr[--_size].~T();
            }
        }

        template<typename... Args>
        void emplace_back(Args&&... args) {
            if (_size == _capacity) grow();
            new (_ptr + _size) T(rgl::forward<Args>(args)...);
            _size++;
        }

	    size_t size() const { return _size; }
	};
}