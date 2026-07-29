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

#include "rgl/core/types.h"
#include "rgl/core/iterator.h"
#include "rgl/core/utility/move.h"
#include "rgl/memory/new.h"

namespace rgl {
	template<typename T>
	class vector final{
		size_t _size;
		size_t _capacity;
		T* _ptr;
	
		void reallocate(size_t qty) {
            T* tmp = static_cast<T*>(::operator new[](qty * sizeof(T)));
            
            for(size_t i = 0; i < _size; ++i) {
                new (&tmp[i]) T(move(_ptr[i]));
                _ptr[i].~T();
            }

            ::operator delete[](_ptr);
            _ptr = tmp;
            _capacity = qty;
        }
	public:
		using iterator = T*;
		using const_iterator = const T*;
		using reverse_iterator = typename rgl::reverse_iterator<iterator>;
    	using const_reverse_iterator = typename rgl::reverse_iterator<const_iterator>;

		~vector(){
			clear();
			::operator delete[](_ptr);
		}
		
		vector() : _size(0), _capacity(0), _ptr(nullptr) {}
		vector(size_t s) : _size(0), _capacity(s), 
            _ptr(s > 0 ? static_cast<T*>(::operator new[](s * sizeof(T))) : nullptr) {}

		vector(vector&& other) noexcept
		 : _size(other._size), _capacity(other._capacity), _ptr(other._ptr) {
		 	other._size = 0;
		 	other._capacity = 0;
		 	other._ptr = nullptr;
		}
		vector(const vector& other) : _size(other._size), _capacity(other._size) {
		    _ptr = _capacity > 0 ? static_cast<T*>(::operator new[](_capacity * sizeof(T))) : nullptr;
		    for (size_t i = 0; i < _size; ++i) {
		        new (&_ptr[i]) T(other._ptr[i]);
		    }
		}

		vector& operator=(const vector& other) {
		    if (this != &other) {
		    	vector temp(other);
		    	swap(temp);
		    }
		    return *this;
		}

		vector& operator=(vector&& other) noexcept {
            if(this != &other) {
                clear();
                ::operator delete[](_ptr);

                _ptr = other._ptr;
                _size = other._size;
                _capacity = other._capacity;

                other._ptr = nullptr;
                other._size = 0;
                other._capacity = 0;
            }
            return *this;
        }

		void push_back(const T& value) {
            if (_size == _capacity) reserve(_capacity == 0 ? 1 : _capacity * 2);
            new (&_ptr[_size]) T(value);
            _size++;
        }

		void push_back(T&& value) {
		    if (_size == _capacity) {
		        reserve(_capacity == 0 ? 1 : _capacity * 2);
		    }
			new (&_ptr[_size]) T(move(value));
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
		    new (&_ptr[_size]) T(rgl::forward<Args>(args)...);
		    _size++;
		}
		void clear() {
		    for (size_t i = 0; i < _size; ++i) {
		        _ptr[i].~T();
		    }
		    _size = 0;
		}
		void pop_back() {
 			if(_size > 0)
 				_ptr[--_size].~T();
        }
        bool empty() const {
        	return _size == 0;
        }
        iterator erase(size_t index) {
            if (index >= _size) return end();
            return erase(_ptr + index);
        }
        iterator erase(iterator pos) {
            size_t index = pos - _ptr;
            if (index >= _size) return end();
            _ptr[index].~T();

            for (size_t i = index; i < _size - 1; ++i) {
                new (_ptr + i) T(rgl::move(_ptr[i + 1]));
                _ptr[i + 1].~T();
            }

            _size--;
            return _ptr + index;
        }

        iterator insert(iterator pos, const T& value) {
            size_t index = pos - _ptr;
            if (index > _size) index = _size;

            if (_size == _capacity) {
                size_t next_capacity = (_capacity == 0) ? 4 : (_capacity * 2);
                reallocate(next_capacity);
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
                size_t next_capacity = (_capacity == 0) ? 4 : (_capacity * 2);
                reallocate(next_capacity);
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

        void swap(vector& other) noexcept {
    		rgl::swap(_size, other._size);
    		rgl::swap(_capacity, other._capacity);
    		rgl::swap(_ptr, other._ptr);
		}

		T& operator[](size_t index){ return _ptr[index]; }
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
};