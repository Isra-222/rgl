//abort
#pragma once

#include <csignal>
#include <unistd.h>

namespace rgl {
	[[noreturn]] void abort() noexcept {
		raise(SIGABRT);
		_exit(1);
	}
}