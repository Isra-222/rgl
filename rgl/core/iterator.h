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
//iterator.h
#pragma once

#include "types.h"

namespace rgl {
	struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};
	
	template<typename Iter>
	struct iterator_traits{
		using iterator_category = typename Iter::iterator_category;
        using value_type        = typename Iter::value_type;
        using difference_type   = typename Iter::difference_type;
        using pointer           = typename Iter::pointer;
        using reference         = typename Iter::reference;
	};
	template<typename T>
    struct iterator_traits<T*> {
        using iterator_category = random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = rgl::ssize_t;
        using pointer           = T*;
        using reference         = T&;
    };
    template<typename T>
    struct iterator_traits<const T*> {
        using iterator_category = random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = rgl::ssize_t;
        using pointer           = const T*;
        using reference         = const T&;
    };

	template<typename Iterator>
	class reverse_iterator{
		Iterator current;
	public:
		using iterator_type = Iterator;
		using difference_type = typename rgl::ssize_t;
		using value_type = typename iterator_traits<Iterator>::value_type;
		using reference = typename rgl::iterator_traits<Iterator>::reference;
        using pointer = typename rgl::iterator_traits<Iterator>::pointer;
	
        reverse_iterator() : current() {}
        explicit reverse_iterator(Iterator it) : current(it) {}

        template<typename U>
        reverse_iterator(const reverse_iterator<U>& other) : current(other.base()) {}

        Iterator base() const {
            return current;
        }

        reference operator*() const {
            Iterator temp = current;
            return *--temp;
        }

        pointer operator->() const {
            return &(operator*());
        }

        reverse_iterator& operator++() {
            --current;
            return *this;
        }

        reverse_iterator operator++(int) {
            reverse_iterator temp = *this;
            --current;
            return temp;
        }

        reverse_iterator& operator--() {
            ++current;
            return *this;
        }

        reverse_iterator operator--(int) {
            reverse_iterator temp = *this;
            ++current;
            return temp;
        }

        bool operator==(const reverse_iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const reverse_iterator& other) const {
            return current != other.current;
        }
	};
}//namespace rgl