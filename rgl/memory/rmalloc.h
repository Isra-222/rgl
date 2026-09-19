#pragma once

#include "regular/manager.h"

namespace rgl{
	inline void* rmalloc(size_t size) { return internal::internalAlloc(size); }
	inline void  rfree(void* ptr) { internal::internalFree(ptr); }
}