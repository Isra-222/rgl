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
#include "rgl/core/iterator.h"
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

	    void reallocate() {
	    	size_t next_capacity = (_capacity == N) ? (N * 2) : (_capacity * 2);
    		reserve(next_capacity);
	    }
	public:
        using iterator = T*;
        using const_iterator = const T*;
		using reverse_iterator = typename rgl::reverse_iterator<iterator>;
    	using const_reverse_iterator = typename rgl::reverse_iterator<const_iterator>;
	    
	    smallVector() : _ptr(stack_ptr()), _size(0), _capacity(N), _is_heap(false) {}

	    ~smallVector() {
	        clear();
	        if (_is_heap) {
	            ::operator delete[](_ptr);
	        }
	    }

	    smallVector(const smallVector& other)
    	: _ptr(stack_ptr()), _size(0), _capacity(N), _is_heap(false) {
		    if (other._size > 0) {
		        if (other._size > N) {
		            _ptr = static_cast<T*>(::operator new[](other._size * sizeof(T)));
		            _capacity = other._size;
		            _is_heap = true;
		        }
		        for (size_t i = 0; i < other._size; ++i) {
		            new (_ptr + i) T(other._ptr[i]);
		        }
		        _size = other._size;
		    }
		}
	    smallVector& operator=(const smallVector& other){
	    	if(this != &other){
	    		clear();
	    		if (other._size > 0) {
		            reserve(other._size);
		            for(size_t i = 0; i < other._size; ++i)
		                new (_ptr + i) T(other._ptr[i]);
		            _size = other._size;
		        }
	    	}
	    	return *this;
	    }

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

	    smallVector(smallVector&& other) noexcept 
		: _ptr(stack_ptr()), _size(other._size), _capacity(other._capacity), _is_heap(other._is_heap) {
		    if (other._is_heap) {
		        _ptr = other._ptr;
		        other._ptr = other.stack_ptr();
		        other._is_heap = false;
		    } else {
		        for (size_t i = 0; i < other._size; ++i) {
		            new (_ptr + i) T(move(other._ptr[i]));
		            other._ptr[i].~T();
		        }
		    }
		    other._size = 0;
		    other._capacity = N;
		}
		
	    void push_back(const T& value) {
	        if (_size == _capacity) reallocate();
	        new (_ptr + _size) T(value);
	        _size++;
	    }
	    void push_back(T&& value){
	    	if(_size == _capacity) reallocate();
	    	new (_ptr + _size) T(rgl::move(value));
	    	_size++;
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
		bool empty() const { return _size == 0; }
        
        void pop_back() {
            if (_size > 0) {
                _ptr[--_size].~T();
            }
        }

        template<typename... Args>
        void emplace_back(Args&&... args) {
            if (_size == _capacity) reallocate();
            new (_ptr + _size) T(rgl::forward<Args>(args)...);
            _size++;
        }

	    size_t size() const { return _size; }

	    void reserve(size_t new_cap){
	    	if(new_cap <= _capacity) return;

	    	T* new_ptr = static_cast<T*>(::operator new[](new_cap * sizeof(T)));

	    	for (size_t i = 0; i < _size; ++i) {
		        new (new_ptr + i) T(move(_ptr[i]));
		        _ptr[i].~T();
    		}
    		if (_is_heap) {
        		::operator delete[](_ptr);
    		}
	    
    		_ptr = new_ptr;
		    _capacity = new_cap;
		    _is_heap = true;
	    }

	    iterator erase(iterator pos){
	    	size_t index = pos - _ptr;
	    	if(index >= _size) return end();

	    	_ptr[index].~T();
	    	for(size_t i = index; i < _size - 1; ++i){
	    		new (_ptr + i) T(rgl::move(_ptr[i + 1]));
	    		_ptr[i + 1].~T();
	    	}

	    	_size--;
	    	return _ptr + index;
	    }
	    iterator erase(size_t index) {
            if (index >= _size) return end();
            return erase(_ptr + index);
        }

        iterator insert(iterator pos, const T& value) {
            size_t index = pos - _ptr;
            if (index > _size) index = _size;

            if (_size == _capacity) {
                reallocate();
                pos = _ptr + index;
            }
            for (size_t i = _size; i > index; --i) {
                new (_ptr + i) T(rgl::move(_ptr[i - 1]));
                _ptr[i - 1].~T();
            }

            new (_ptr + index) T(value);
            _size++;

            return _ptr + index;
        }

        iterator insert(iterator pos, T&& value) {
            size_t index = pos - _ptr;
            if (index > _size) index = _size;

            if (_size == _capacity) {
                reallocate();
                pos = _ptr + index;
            }

            for (size_t i = _size; i > index; --i) {
                new (_ptr + i) T(rgl::move(_ptr[i - 1]));
                _ptr[i - 1].~T();
            }

            new (_ptr + index) T(rgl::move(value));
            _size++;

            return _ptr + index;
        }

        iterator insert(size_t index, const T& value) {
            return insert(_ptr + index, value);
        }

        iterator insert(size_t index, T&& value) {
            return insert(_ptr + index, rgl::move(value));
        }

	    bool operator==(const smallVector& other) const {
		    if (_size != other._size) return false;
		    for (size_t i = 0; i < _size; ++i) {
		        if (_ptr[i] != other._ptr[i]) return false;
		    }
		    return true;
		}

		bool operator!=(const smallVector& other) const {
		    return !(*this == other);
		}

	    T& operator[](size_t index) { return _ptr[index]; }
        const T& operator[](size_t index) const { return _ptr[index]; }

        T& front() { return _ptr[0]; }
        const T& front() const { return _ptr[0]; }
        T& back() { return _ptr[_size - 1]; }
        const T& back() const { return _ptr[_size - 1]; }

        iterator begin() { return _ptr; }
        iterator end() { return _ptr + _size; }

        const_iterator begin() const { return _ptr; }
        const_iterator end() const { return _ptr + _size; }

        reverse_iterator rbegin() { return reverse_iterator(end()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }

		const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
        const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
	};
}