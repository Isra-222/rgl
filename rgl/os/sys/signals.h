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

//signals.h
#ifndef RGL_SYS_SYGNALS_H
#define RGL_SYS_SYGNALS_H

#include "rgl/core/types.h"

namespace rgl::sys {
	enum class signal : int {
	    SIGHUP    = 1,
	    SIGINT    = 2,
	    SIGQUIT   = 3,
	    SIGILL    = 4,
	    SIGTRAP   = 5,
	    SIGABRT   = 6,
	    SIGIOT    = SIGABRT,
	    SIGBUS    = 7,
	    SIGFPE    = 8,
	    SIGKILL   = 9,
	    SIGUSR1   = 10,
	    SIGSEGV   = 11,
	    SIGUSR2   = 12,
	    SIGPIPE   = 13,
	    SIGALRM   = 14,
	    SIGTERM   = 15,
	    SIGSTKFLT = 16,
	    SIGCHLD   = 17,
	    SIGCONT   = 18,
	    SIGSTOP   = 19,
	    SIGTSTP   = 20,
	    SIGTTIN   = 21,
	    SIGTTOU   = 22,
	    SIGURG    = 23,
	    SIGXCPU   = 24,
	    SIGXFSZ   = 25,
	    SIGVTALRM = 26,
	    SIGPROF   = 27,
	    SIGWINCH  = 28,
	    SIGIO     = 29,
	    SIGPOLL   = SIGIO,
	    SIGPWR    = 30,
	    SIGSYS    = 31,
	    SIGUNUSED = 31
	};
}// namespace rgl::sys

#endif