#pragma once

#include "rgl/async/atomic.h"
#include "rgl/core/types.h"

namespace rgl::internal{
	class mem_pool_manager;

	struct free_node { free_node* next; };

	class page_metadata final{
		uint32_t magic;
		uint32_t thread_id;
		free_node* free_list_head;
		atomic<free_node*> remote_free_list;
		page_metadata* next_page;
		mem_pool_manager* owner_pool;

		atomic<uint16_t> active_blocks;


		void getRemoteBlocks(){
			free_node* nodes = remote_free_list.exchange(nullptr, memory_order_acquire);
			if(nodes){
				free_node* curr = nodes;
                while(curr->next != nullptr) {
                    curr = curr->next;
                }
                curr->next = free_list_head;
                free_list_head = nodes;
			}
		}
	public:
		friend class mem_pool_manager;

		page_metadata(uint32_t thread_id, free_node* free, uint16_t active_blocks, 
                      page_metadata* np, mem_pool_manager* pool)
         : magic(page_metadata_magic), thread_id(thread_id), free_list_head(free),
        	remote_free_list(nullptr), next_page(np), owner_pool(pool), active_blocks(active_blocks) {}

		page_metadata()
		 : magic(0), thread_id(0), free_list_head(nullptr), 
			remote_free_list(nullptr), next_page(nullptr), owner_pool(nullptr), active_blocks(0) {}

		void* allocate(){
			if(!free_list_head){
				getRemoteBlocks();
				if(!free_list_head) return nullptr;
			}
			free_node* old_head = free_list_head;
			free_list_head = old_head->next;

        	active_blocks++;
        	return static_cast<void*>(old_head);
		}
		void free(void* ptr){
			//if(!ptr) return;

			free_node* newNode = static_cast<free_node*>(ptr);
			newNode->next = free_list_head;
			free_list_head = newNode;

			active_blocks.fetch_sub(1, memory_order_release);
		}
		void external_free(void* ptr){
			//if(!ptr) return;

			free_node* node_to_free = static_cast<free_node*>(ptr);
			free_node* old_head = remote_free_list.load(memory_order_relaxed);
			do{
				node_to_free->next = old_head;
			}while(!remote_free_list.compare_exchange_weak(
				old_head, node_to_free, memory_order_release,
				memory_order_relaxed
			));

			active_blocks.fetch_sub(1, memory_order_release);
		}

		bool checkMagic(uint32_t number) const { return magic == number; }
		bool checkThread(uint32_t x) const { return thread_id == x; }
		uint16_t getActiveBlocks() const { return active_blocks.load(memory_order_acquire); }
		mem_pool_manager* getOwnerPool() const { return owner_pool; }

		page_metadata* getNextPage() { return next_page; }
		const page_metadata* getNextPage() const { return next_page; }
		void setNextPage(page_metadata* next) { next_page = next; }
	};
}//namespace rgl::internal