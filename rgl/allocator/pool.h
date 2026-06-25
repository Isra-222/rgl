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

//pool
#pragma once

#include "rgl/core/types.h"
#include "rgl/memory/new.h"
#include "rgl/algorithm/algorithm.h"

namespace rgl{
	template<typename T>
	class PoolAllocator final{
		struct Node{ Node* next; };

		size_t m_blockSize;
		size_t m_numBlocks;
		void* m_memoryBuffer;
		Node* m_head;

	public:
		PoolAllocator(size_t amount)
		: m_blockSize(max(sizeof(T), sizeof(Node)) )
		, m_numBlocks(amount), m_head(nullptr) {
			m_memoryBuffer = ::operator new[](m_blockSize * m_numBlocks);
			reset();
		}
		~PoolAllocator(){
			::operator delete[](m_memoryBuffer);
		}

		PoolAllocator(const PoolAllocator&) = delete;
	    PoolAllocator& operator=(const PoolAllocator&) = delete;

		void reset() {
	        m_head = reinterpret_cast<Node*>(m_memoryBuffer);
	        Node* current = m_head;
	        
	        for (size_t i = 0; i < m_numBlocks - 1; ++i) {
	            Node* nextNode = reinterpret_cast<Node*>(reinterpret_cast<uint8_t*>(current) + m_blockSize);
	            current->next = nextNode;
	            current = nextNode;
	        }
	        current->next = nullptr;
	    }

		T* allocate(){
			if(m_head == nullptr) return nullptr;

			Node* freeBlock = m_head;
			m_head = m_head->next;
			return reinterpret_cast<T*>(freeBlock);
		}
		void deallocate(T* ptr){
			if(!ptr) return;

			Node* releasedBlock = reinterpret_cast<Node*>(ptr);
			releasedBlock->next = m_head;
			m_head = releasedBlock;
		}
	};
}