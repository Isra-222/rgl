#pragma once

#include "constants.h"
#include "enlisted.h"

namespace rgl::internal{
	class mem_pool_manager final {
		page_metadata* currentPage;
	public:
		void init(void* buffer, size_t blockSize, size_t rawMemoryCount, uint32_t thread_id){
            
            page_metadata* newPage = reinterpret_cast<page_metadata*>(buffer);

            newPage->magic = page_metadata_magic;
            newPage->thread_id = thread_id;
            newPage->active_blocks.store(0, memory_order_release);
            newPage->remote_free_list.store(nullptr, memory_order_relaxed);

            newPage->next_page = currentPage;
            newPage->owner_pool = this;

            uint8_t* current_memory = reinterpret_cast<uint8_t*>(buffer) + sizeof(page_metadata);
            size_t b_count = (rawMemoryCount - sizeof(page_metadata)) / blockSize;

            newPage->free_list_head = reinterpret_cast<free_node*>(current_memory);
            free_node* current_node = newPage->free_list_head;
            
            for(size_t i = 0; i < b_count - 1; ++i){
                uint8_t* next_addr = reinterpret_cast<uint8_t*>(current_node) + blockSize;
                current_node->next = reinterpret_cast<free_node*>(next_addr);
                current_node = current_node->next;
            }
            if (b_count > 0)
                current_node->next = nullptr;
            else
                newPage->free_list_head = nullptr;

            currentPage = newPage;
        }
		mem_pool_manager(void* buffer, size_t blockSize, size_t rawMemoryCount, 
			uint32_t thread_id) { 
			currentPage = reinterpret_cast<page_metadata*>(buffer);
			currentPage->next_page = nullptr;

			init(buffer, blockSize, rawMemoryCount, thread_id); 
		}
		mem_pool_manager()
		: currentPage(nullptr) {}

		mem_pool_manager(const mem_pool_manager&) = delete;
        mem_pool_manager& operator=(const mem_pool_manager&) = delete;

        page_metadata* popPage() {
		    if (!currentPage) return nullptr;

		    page_metadata* page = currentPage;
		    currentPage = page->next_page;
		    page->next_page = nullptr;
		    return page;
		}
		bool removeFreePage(page_metadata* page) {
			if(!page || !currentPage) return false;

			if (currentPage == page){
				if (!currentPage->next_page) return false;

				currentPage = currentPage->next_page;
                page->next_page = nullptr;
                return true;
			}

			page_metadata* curr = currentPage;
            while (curr->next_page != nullptr && curr->next_page != page) {
                curr = curr->next_page;
            }
            if (curr->next_page == page) {
                curr->next_page = page->next_page;
                page->next_page = nullptr;
                return true;
            }
            return false;
        }
		page_metadata* getCurrentPage() const { return currentPage; }

		explicit operator bool() const { return currentPage; }
	};
}//namespace rgl::internal