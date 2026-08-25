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

//function.h
#pragma once

#include "rgl/core/types.h"

namespace rgl{
	template<typename F>
	class function;

	template<typename Ret, typename ...Args>
	class function<Ret(Args...)>{
		
		struct CallableBase {
            virtual ~CallableBase() = default;
            virtual Ret invoke(Args... args) = 0;
        };
        
        template<typename SF>
        struct CallableImpl final : CallableBase{
        	SF func;
        	CallableImpl(SF&& f): func(static_cast<SF&&>(f)) {}

        	Ret invoke(Args... args) override{
        		return func(args...);
        	}
        };

        CallableBase* ptr = nullptr;
    public:
    	function(): ptr(nullptr) {}
        function(nullptr_t) noexcept : ptr(nullptr) {}

    	template <typename F>
        function(F f) {
            ptr = new CallableImpl<F>(static_cast<F&&>(f));
        }
	
        ~function() {
            delete ptr;
        }

        Ret operator()(Args... args) const {
        	if(!ptr){
        		panic("nullptr to function call!");
        	}
        	return ptr->invoke(static_cast<Args&&>(args)...);
        }
        function(const function&) = delete;
        function& operator=(const function&) = delete;

        function(function&& other) noexcept : ptr(other.ptr) {
            other.ptr = nullptr;
        }
        function& operator=(nullptr_t) noexcept {
            delete ptr;
            ptr = nullptr;
            return *this;
        }

        function& operator=(function&& other) noexcept {
            if (this != &other) {
                delete ptr;
                ptr = other.ptr;
                other.ptr = nullptr;
            }
            return *this;
        }
	};
}//namespace rgl