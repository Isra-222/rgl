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

//sort.h
#pragma once

#include "rgl/core/utility/move.h"

namespace rgl::algo{
	template<typename Iterator>
    void sort(Iterator first, Iterator last) {
        if (first >= last - 1) return;

        auto mid = first + (last - first) / 2;
        auto pivot = *mid;

        Iterator i = first;
        Iterator j = last - 1;

        while (i <= j) {
            while (*i < pivot) ++i;
            while (*j > pivot) --j;
            if (i <= j) {
                swap(*i, *j);
                ++i;
                --j;
            }
        }

        if (first < j) sort(first, j + 1);
        if (i < last) sort(i, last);
    }
}