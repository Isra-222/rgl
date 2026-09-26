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

        constexpr rgl::size_t alignment = 16;
        tls_size = (tls_size + (alignment - 1)) & ~(alignment - 1);

        rgl::size_t total_allocation = tls_size + sizeof(rgl::internal::thread_control_block) + alignment;

        void* raw_memory = rgl::rmalloc(total_allocation);
        if (!raw_memory) return nullptr;

        uintptr_t raw_addr = reinterpret_cast<uintptr_t>(raw_memory);
        uintptr_t aligned_addr = (raw_addr + (alignment - 1)) & ~(alignment - 1);

        char* tls_block = reinterpret_cast<char*>(aligned_addr);
        auto* tcb = reinterpret_cast<rgl::internal::thread_control_block*>(tls_block + tls_size);

        rgl::memcpy(tls_block, __tdata_start, __tdata_end - __tdata_start);
        rgl::memset(tls_block + (__tdata_end - __tdata_start), 0, __tbss_end - __tdata_end);

        tcb->raw_tls_allocation = raw_memory;

        return tcb;
    }
    thread_control_block* init_main_tcb(){
        rgl::size_t tls_size = __tbss_end - __tdata_start;

        constexpr rgl::size_t alignment = 16;
        tls_size = (tls_size + (alignment - 1)) & ~(alignment - 1);
        rgl::size_t total_allocation = tls_size + sizeof(rgl::internal::thread_control_block) + alignment;

        void* raw_memory = rgl::sys::mmap(
            nullptr, total_allocation, 
            rgl::sys::prot::read | rgl::sys::prot::write,
            rgl::sys::map::_private | rgl::sys::map::anonymous, -1, 0
        );

        rgl::uintptr_t raw_addr = reinterpret_cast<rgl::uintptr_t>(raw_memory);
        rgl::uintptr_t aligned_addr = (raw_addr + (alignment - 1)) & ~(alignment - 1);
        char* tls_block = reinterpret_cast<char*>(aligned_addr);
        auto* main_tcb = reinterpret_cast<rgl::internal::thread_control_block*>(tls_block + tls_size);

        rgl::memcpy(tls_block, __tdata_start, __tdata_end - __tdata_start);
        rgl::memset(tls_block + (__tdata_end - __tdata_start), 0, __tbss_end - __tdata_end);
        main_tcb->raw_tls_allocation = raw_memory;


        new (&main_tcb->my_cache) rgl::internal::thread_cache(0);

        return main_tcb;
    }
}