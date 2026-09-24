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

#include "rgl/core/types.h"
#include "rgl/os/sys/mman.h"
#include "rgl/async/atomic.h"

namespace rgl::internal{
    class stack_global final{
        static constexpr size_t DEFAULT_STACK_SIZE = 2 * 1024 * 1024; // 2 MB
        static constexpr int page_size = 4096;

        struct stack_segment { 
            void* raw_ptr;
            stack_segment* next; 
        };
        
        struct stack_node { 
            stack_node* next; 
        };
    
        atomic<stack_node*> top;
        stack_segment* nextSeg;
        atomic<bool> segment_lock = false;

        void* allocate_new_stack(){
            size_t aligned_size = DEFAULT_STACK_SIZE;

            void* raw_ptr = sys::mmap(nullptr, aligned_size, sys::prot::read | sys::prot::write,
                sys::map::_private | sys::map::anonymous, -1, 0);

            if(raw_ptr == sys::MAP_FAILED) return nullptr;

            uintptr_t end_of_map = reinterpret_cast<uintptr_t>(raw_ptr) + aligned_size;
            stack_segment* info = reinterpret_cast<stack_segment*>(end_of_map - sizeof(stack_segment));
            info->raw_ptr = raw_ptr;

            while(segment_lock.exchange(true, memory_order_acquire)) {
                __builtin_ia32_pause();
            }

            info->next = nextSeg;
            nextSeg = info;

            segment_lock.store(false, memory_order_release);

            return raw_ptr;
        }
    public:
        stack_global() : top(nullptr), nextSeg(nullptr) {}
        
        ~stack_global() {
            stack_segment* current = nextSeg;
            while(current) {
                stack_segment* next = current->next;
                sys::munmap(current->raw_ptr, DEFAULT_STACK_SIZE);
                current = next;
            }
        }

        void* getStack() {
            stack_node* old_head = top.load(memory_order_relaxed);
            do {
                if(!old_head) return allocate_new_stack();
            } while(!top.compare_exchange_weak(
                old_head, old_head->next, 
                memory_order_acquire,
                memory_order_relaxed
            ));

            return reinterpret_cast<void*>(old_head);
        }

        void releaseStack(void* stack){
            auto* node = reinterpret_cast<stack_node*>(stack);
            stack_node* old_head = top.load(memory_order_relaxed);
            do {
                node->next = old_head;
            } while(!top.compare_exchange_weak(
                old_head, node,
                memory_order_release,
                memory_order_relaxed
            ));
        }
    };

    extern stack_global GLOBAL_STACK_CACHE;
} // namespace rgl::internal