#pragma once

#include "thread_global.h"
#include "pool.h"

namespace rgl::internal{
	class thread_cache final{
    	mem_pool_manager pools[33];
    	uint32_t thread_id;
    public:
    	thread_cache(uint32_t thread_id) : thread_id(thread_id) {}
    	~thread_cache(){
    		for(auto& pool : pools)
    			while (page_metadata* page = pool.popPage())
            		GLOBAL_CENTRAL_CACHE.push(page);
    	}

    	void tryGetPage(mem_pool_manager& pool, uint8_t indexTable){
    		//							   //MEDIA / SLOW PATH
    		page_metadata* recycled_page = GLOBAL_CENTRAL_CACHE.pop();
    		if(recycled_page)
    			pool.init(static_cast<void*>(recycled_page), SizeList[indexTable], pageSize, thread_id);
    	}

    	void* allocate(size_t size){
    		uint8_t indexTable = get_class(size);
    		mem_pool_manager& current_pool = pools[indexTable];

    		if(!current_pool){
    			//MEDIA_PATH // BATH PATH
    			tryGetPage(current_pool, indexTable);
    			return current_pool.getCurrentPage()->allocate();
    		}

    		//FAST PATH
    		void* ptr = current_pool.getCurrentPage()->allocate();
    		if(ptr) return ptr;

    		tryGetPage(current_pool, indexTable);
			return current_pool.getCurrentPage()->allocate();
    	}

    	void free(void* ptr){
    		uintptr_t valPtr = reinterpret_cast<uintptr_t>(ptr);
			uintptr_t page_addr = valPtr & ~(pageSize - 1);

			page_metadata* page = reinterpret_cast<page_metadata*>(page_addr);
			if(!page->checkMagic(page_metadata_magic)){
				//panic("Allocator Error: Invalid page or corrupted owner_pool!");
				return;
			}
			if(!page->checkThread(thread_id)){
				page->external_free(ptr);
			}
			else{
				page->free(ptr);
			}

			uint16_t active_blocks = page->getActiveBlocks();

			if(!active_blocks){
				bool canRecycle = page->getOwnerPool()->removeFreePage(page);
				if(canRecycle)
					GLOBAL_CENTRAL_CACHE.push(page);
			}
    	}

    };

    inline thread_cache& get_thread_cache(){
    	static atomic<uint32_t> count = 0;
    	thread_local thread_cache cache_instance(count++);
    	return cache_instance;
    }
}//namespace rgl