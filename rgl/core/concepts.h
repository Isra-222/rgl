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

//concepts
#pragma once

#include "traits.h"

namespace rgl{
    template<typename T>
    struct is_simple_type : false_type {};

    template<typename T, typename U>
    concept same_as = is_same_v<T, U>;

    template<typename T, template<typename> class Trait>
    concept satisfies = Trait<T>::value;

    template<typename T>
    concept DefaultConstructible = is_default_constructible_v<T>;

    template<typename T>
    concept SimpleType = is_simple_type<T>::value;

    template<typename To, typename From>
    concept Castable = requires(From& f) {
        { isa<To>(f) } -> same_as<bool>; 
    };

    template<typename To, typename From>
    concept DerivedFrom = is_base_of_v<From, To>;
}