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

#pragma once

#include "../core/types.h"
#include "../mem/new.h"

namespace rgl {
	
	class stackAllocator final{
		uint8_t* m_begin;
		uint8_t* m_current;
		size_t m_capacity;

	public:
		stackAllocator(size_t c)
		: m_begin(nullptr), m_current(nullptr), m_capacity(c) {
			m_begin = reinterpret_cast<uint8_t*>(::operator new[](m_capacity));
			m_current = m_begin;
		}
		~stackAllocator(){
			if(m_begin == nullptr) return;
			::operator delete[](m_begin);
			m_begin = nullptr;
			m_current = nullptr;
		}

		template<typename T>
		T* push(size_t bytes){
			if(m_current + (bytes + sizeof(size_t)) > (m_begin + m_capacity)) return nullptr; 
			T* ptr = reinterpret_cast<T*>(m_current);
			m_current += bytes;
			size_t* extraInfo = reinterpret_cast<size_t*>(m_current);
			*extraInfo = bytes;
			m_current += sizeof(size_t);
			return ptr;
		}
		void pop(){
			size_t* size = reinterpret_cast<size_t*>(m_current - sizeof(size_t));
			m_current -= (*size + sizeof(size_t));
		}

	};
}//namespace rgl