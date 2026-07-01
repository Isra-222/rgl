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

//modifiers.h
#pragma once

namespace rgl::algo {

    template<typename Iterator>
    void rotate(Iterator first, Iterator middle, Iterator last) {
        auto next = middle;
        while (first != next) {
            swap(*first++, *next++);
            if (next == last) next = middle;
            else if (first == middle) middle = next;
        }
    }
    template<typename Iterator>
    void reverse(Iterator first, Iterator last) {
        while (first < last - 1) {
            --last;
            swap(*first, *last);
            ++first;
        }
    }

}