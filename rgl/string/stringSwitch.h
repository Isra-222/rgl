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

//stringSwitch.h
#pragma once

#include "rgl/core/utility/memory.h"
#include "string_view.h"

namespace rgl{
	template<typename T>
	class stringSwitch {
	    rgl::string_view Str;
	    T Result;
	    bool Found;

	public:
	    explicit stringSwitch(rgl::string_view S) : Str(S), Result(), Found(false) {}

	    stringSwitch& Case(rgl::string_view S, T Value) {
	        if (!Found && Str.size() == S.size() && 
	            rgl::memcmp(Str.data(), S.data(), S.size()) == 0) {
	            Result = Value;
	            Found = true;
	        }
	        return *this;
	    }

	    T Default(T Value) const { return Found ? Result : Value; }
	};
}//namespace 