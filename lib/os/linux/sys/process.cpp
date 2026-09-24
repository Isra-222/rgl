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
#include "rgl/os/sys/process.h"

using namespace rgl;

[[noreturn]] 
void sys::exit_group(int status){
    register int __status __asm__("rdi") = status;

    __asm__ volatile (
        "syscall"
        :
        : "a" (231), //  __NR_exit_group
          "r" (__status)
        : "rcx", "r11", "memory"
    );
    __builtin_unreachable();
}
[[noreturn]] 
void sys::exit(int status){
    register int __status __asm__("rdi") = status;

    __asm__ volatile (
        "syscall"
        :
        : "a" (60), //  __NR_exit
          "r" (__status)
        : "rcx", "r11", "memory"
    );
    __builtin_unreachable();
}
int sys::tgkill(int tgid, int tid, int sig){
    long ret;
    register int __tgid __asm__("rdi") = tgid;  
    register int __tid __asm__("rsi") = tid;  
    register int __sig __asm__("rdx") = sig; 

    __asm__ volatile(
        "syscall"
        : "=a" (ret)
        : "a" (234), // __NR_tgkill
          "r" (__tgid),
          "r" (__tid),
          "r" (__sig)
        : "rcx", "r11", "memory"
    );
    return static_cast<int>(ret);
}