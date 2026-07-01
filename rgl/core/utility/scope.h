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

//scope
#pragma once

#include "move.h"

namespace rgl{
	template<typename Func>
	class scope_exit{
		Func f;
		bool active;
	public:
		explicit scope_exit(Func f) : f(move(f)), active(true) {}

		~scope_exit(){if(active) f(); }

		scope_exit(const scope_exit&) = delete;
        scope_exit& operator=(const scope_exit&) = delete;
		
		scope_exit(scope_exit&& other) noexcept : f(move(other.f)), active(other.active) {
            other.active = false;
        }
        void dismiss() noexcept { active = false; }
	};
	template<typename Func>
	scope_exit<Func> make_scope_exit(Func f){
		return scope_exit<Func>(move(f));
	}
}