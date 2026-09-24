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
#include "rgl/core/utility/memory.h"
#include "rgl/memory/new.h"
#include "rgl/os/internal/thread_control_block.h"

extern "C" {
    extern char __tdata_start[];
    extern char __tdata_end[];
    extern char __tbss_end[];
}

namespace rgl::internal {
    void* allocate_thread_storage() {
        rgl::size_t tls_size = __tbss_end - __tdata_start;
        rgl::size_t total_allocation = tls_size + sizeof(rgl::internal::thread_control_block);

        void* raw_memory = rgl::rmalloc(total_allocation);
        if (!raw_memory) return nullptr;

        char* tls_block = static_cast<char*>(raw_memory);
        auto* tcb = reinterpret_cast<rgl::internal::thread_control_block*>(tls_block + tls_size);

        rgl::memcpy(tls_block, __tdata_start, __tdata_end - __tdata_start);
        rgl::memset(tls_block + (__tdata_end - __tdata_start), 0, __tbss_end - __tdata_end);

        return tcb;
    }
}