#pragma once

#include "thread_local.h"

namespace rgl::internal{
	struct BigHeader {
        uint64_t magic;
        size_t total_size;
    };

	inline void* bigAlloc(size_t size){
		size_t total = size + sizeof(BigHeader);
		total = (total + 3) & ~size_t(3);

		void* raw_ptr = mmap(nullptr, total, PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

		if(raw_ptr == MAP_FAILED) return nullptr;

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
			munmap(header, header->total_size);
		else
			get_thread_cache().free(ptr);

	}
}//namespace rgl::internal