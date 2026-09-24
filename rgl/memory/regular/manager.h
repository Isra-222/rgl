#pragma once

#include "thread_local.h"
#include "rgl/os/internal/thread_control_block.h"

namespace rgl::internal{
	inline thread_cache& get_thread_cache(){
    	return get_control_block()->my_cache;
    }
	struct BigHeader {
        uint64_t magic;
        size_t total_size;
    };

	inline void* bigAlloc(size_t size){
		size_t total = size + sizeof(BigHeader);
		total = (total + 3) & ~size_t(3);

		void* raw_ptr = sys::mmap(nullptr, total, sys::prot::read | sys::prot::write,
            sys::map::_private | sys::map::anonymous, -1, 0);

		if(raw_ptr == sys::MAP_FAILED) return nullptr;

		BigHeader* header = reinterpret_cast<BigHeader*>(raw_ptr);
		header->magic = rgl_magic;
		header->total_size = total;

		return static_cast<void*>(++header);
	}

	inline void* internalAlloc(size_t size){
		if(size == 0) 
			return nullptr;
		else if(size <= MaxAllocSize) 
			return get_thread_cache().allocate(size);
		else
			return bigAlloc(size);
	}

	inline void internalFree(void* ptr){
		if(!ptr) return;

		BigHeader* header = reinterpret_cast<BigHeader*>(ptr) - 1;

		if(header->magic == rgl_magic)
			sys::munmap(header, header->total_size);
		else
			get_thread_cache().free(ptr);

	}
}//namespace rgl::internal