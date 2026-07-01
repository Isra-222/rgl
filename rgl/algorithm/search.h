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

//search.h
#pragma once

namespace rgl::algo{
	template<typename Iterator, typename T>
    Iterator binary_search(Iterator first, Iterator last, const T& value) {
        while (first < last) {
            Iterator mid = first + (last - first) / 2;
            if (*mid < value) {
                first = mid + 1;
            } else if (value < *mid) {
                last = mid;
            } else {
                return mid;
            }
        }
        return last;
    }
    template<typename T, typename Iterator>
    Iterator find(Iterator first, Iterator last, const T& value) {
        while (first != last) {
            if (*first == value) return first;
            ++first;
        }
        return last;
    }
}