#pragma once

#include "constants.h"
#include "enlisted.h"
#include "rgl/os/sys/mman.h"

namespace rgl::internal{
	class thread_global final {
		struct chunk_segment { void* raw_ptr; chunk_segment* next; };
		atomic<page_metadata*> top;
		chunk_segment* nextSeg;
	public:
		thread_global() : top(nullptr), nextSeg(nullptr) {}

		~thread_global() {
            chunk_segment* current = nextSeg;
            while (current) {
                chunk_segment* next = current->next;
                sys::munmap(current->raw_ptr, chunkSize + pageSize + sizeof(chunk_segment));
                current = next;
            }
        }

		void getRawPages(){
            size_t request_size = chunkSize + pageSize + sizeof(chunk_segment);
            void* raw_ptr = sys::mmap(nullptr, request_size, sys::prot::read | sys::prot::write,
                sys::map::_private | sys::map::anonymous, -1, 0);

            if(raw_ptr == sys::MAP_FAILED) return;

            uintptr_t raw_addr = reinterpret_cast<uintptr_t>(raw_ptr);
            uintptr_t aligned_addr = (raw_addr + sizeof(chunk_segment) + pageSize - 1) & ~(pageSize - 1);
            
            chunk_segment* segment = reinterpret_cast<chunk_segment*>(aligned_addr - sizeof(chunk_segment));
            segment->raw_ptr = raw_ptr;
            segment->next = nextSeg;
            nextSeg = segment;

            uint8_t* aligned_ptr = reinterpret_cast<uint8_t*>(aligned_addr);
            for(size_t i = 0; i < chunkPages; ++i){
                uint8_t* page_bytes = aligned_ptr + (i * pageSize);
                page_metadata* page = reinterpret_cast<page_metadata*>(page_bytes);
                push(page);
            }        
        }

		void push(page_metadata* page){
			page_metadata* old_top = top.load(memory_order_relaxed);
			do{
				page->setNextPage(old_top);
			}while(!top.compare_exchange_weak(
                old_top, page, memory_order_release, memory_order_relaxed
            ));
		}
		page_metadata* pop(){
			page_metadata* old_top = top.load(memory_order_relaxed);

			do{
				if (!old_top){
					getRawPages();
                    old_top = top.load(memory_order_relaxed);
                    if (!old_top) return nullptr;
				}
			}while(!top.compare_exchange_weak(
		        old_top, old_top->getNextPage(), 
		        memory_order_acquire,
		        memory_order_relaxed
		    ));

		    return old_top;
		}
	};

	extern thread_global GLOBAL_CENTRAL_CACHE;
}//namespace rgl::internal