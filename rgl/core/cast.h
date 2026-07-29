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

//cast
#pragma once

#include "error.h"
#include "traits.h"
#include "concepts.h"

namespace rgl {
	template <typename To, typename From, typename Enable = void>
	struct CastInfo {
	    static inline To* doCast(From* Val) {
	        return static_cast<To*>(Val);
	    }
	};
    template <typename To, typename From, typename SimpleFrom>
	struct cast_convert_val {
	    static inline To& doit(From &Val) {
	        return static_cast<To&>(Val);
	    }
	};
    template <typename T>
    struct has_classof {
        template <typename U> static auto test(int) -> decltype(U::classof(nullptr), true_type());
        template <typename U> static false_type test(...);
        static constexpr bool value = decltype(test<T>(0))::value;
    };
    template <typename T>
	struct simplify_type {
	    using SimpleType = T;
	    static SimpleType& getSimplifiedValue(T& Val) { return Val; }
	};

	template <typename To, typename From, typename Enable = void>
	struct isa_impl {
	    static inline bool doit(const From &Val) { return To::classof(&Val); }
	};

	template <typename To, typename From>
	struct isa_impl<To, From, enable_if_t<is_base_of_v<To, From>>> {
	    static inline bool doit(const From &) { return true; }
	};
	template <typename To, typename From>
    struct isa_impl<To, From*, void> {
        static inline bool doit(const From *Val) { return To::classof(Val); }
    };

	template <typename To, typename From>
	[[nodiscard]] inline bool isa(const From &Val) {
	    return isa_impl<To, From>::doit(Val);
	}
	template <typename To, typename From>
    [[nodiscard]] inline bool isa(From *Val) {
        return isa_impl<To, From*, void>::doit(Val);
    }

	template <typename To, typename From>
	[[nodiscard]] inline auto dyn_cast(From *Val) requires DerivedFrom<To, From> || has_classof<To>::value {
	    if (isa<To>(Val)) return CastInfo<To, From>::doCast(Val);
	    return static_cast<To*>(nullptr);
	}

	template <typename To, typename From>
	[[nodiscard]] inline auto cast(From &Val) {
	    rgl_assert(isa<To>(Val) && "cast<> failed: Type mismatch!");
	    return CastInfo<To, From>::doCast(Val);
	}
	template<typename To, typename From>
    [[nodiscard]] inline To* cast(From* Val) {
        rgl_assert(isa<To>(Val) && "cast<> failed: Type mismatch!");
        return CastInfo<To, remove_cv_t<From>>::doCast(Val);
    }


	template <typename To, typename From>
	struct CastIsPossible {
	    static inline bool isPossible(const From &f) {
	        return isa<To>(f);
	    }
	};

}