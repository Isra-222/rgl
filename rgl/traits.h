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

//traits
#pragma once

namespace rgl {

    template<typename T, T v>
    struct integral_constant {
        static constexpr T value = v;
        using value_type = T;
        using type = integral_constant<T, v>;
    };

    using true_type  = integral_constant<bool, true>;
    using false_type = integral_constant<bool, false>;

    template<typename T, typename U>
    struct is_same : false_type {};

    template<typename T>
    struct is_same<T, T> : true_type {};

    template<typename T, typename U>
    inline constexpr bool is_same_v = is_same<T, U>::value;

    template<typename T> struct remove_reference { using type = T; };
    template<typename T> struct remove_reference<T&> { using type = T; };
    template<typename T> struct remove_reference<T&&> { using type = T; };
    template<typename T>
    using remove_reference_t = typename remove_reference<T>::type;

    template<typename T> struct remove_const { using type = T; };
    template<typename T> struct remove_const<const T> { using type = T; };
    template<typename T> using remove_const_t = typename remove_const<T>::type;

    template<typename T> struct remove_volatile { using type = T; };
    template<typename T> struct remove_volatile<volatile T> { using type = T; };
    template<typename T> using remove_volatile_t = typename remove_volatile<T>::type;

    template<typename T>
    struct remove_cv {
        using type = typename remove_volatile<typename remove_const<T>::type>::type;
    };
    template<typename T> using remove_cv_t = typename remove_cv<T>::type;


    template<typename B, typename D>
    struct is_base_of {
        static char test(const B*);
        static long test(...);
        static constexpr bool value = sizeof(test((D*)0)) == sizeof(char);
    };

    template<typename T, typename U>
    inline constexpr bool is_base_of_v = is_base_of<T, U>::value;

    template<bool B, typename T = void> struct enable_if {};
    template<typename T> struct enable_if<true, T> { using type = T; };
    template<bool B, typename T = void> using enable_if_t = typename enable_if<B, T>::type;

    template<typename T>
    constexpr T&& forward(remove_reference_t<T>& arg) noexcept {
        return static_cast<T&&>(arg);
    }
    
    template<typename T>
    constexpr T&& forward(remove_reference_t<T>&& arg) noexcept {
        return static_cast<T&&>(arg);
    }

    template<typename T>
    struct is_default_constructible {
        template<typename U, typename = decltype(U())>
        static true_type test(int);
        template<typename>
        static false_type test(...);
        
        static constexpr bool value = decltype(test<T>(0))::value;
    };

    template<typename T>
    inline constexpr bool is_default_constructible_v = is_default_constructible<T>::value;
}