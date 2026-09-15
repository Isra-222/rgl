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

//linear
#pragma once

#include "rgl/core/types.h"
#include "rgl/memory/new.h"

namespace rgl {
	class linearAllocator final{
		uint8_t* m_begin;
		uint8_t* m_current;
		size_t m_capacity;
		linearAllocator* next = nullptr;
	public:
		linearAllocator(size_t s)
		: m_begin(nullptr), m_current(nullptr), m_capacity(s){
			m_begin = reinterpret_cast<uint8_t*>(::operator new[](m_capacity));
			m_current = m_begin;
		}
		~linearAllocator(){
			if(next) delete next;
			::operator delete[](m_begin);
			m_begin = nullptr;
			m_current = nullptr;
		}

		template<typename T>
		T* push(size_t bytes){
			if(m_current + bytes > m_begin + m_capacity){
				if(!next){
					size_t new_cap = (bytes > m_capacity * 2) ? bytes : m_capacity * 2;
                    next = new linearAllocator(new_cap);
				}
				return next->push<T>(bytes);
			}
			
			T* ptr = reinterpret_cast<T*>(m_current);
			m_current += bytes;
			return ptr;
		}
		void reset(){
			m_current = m_begin;
			if(next)
				next->reset();
		}
	};
}//namespace rgl