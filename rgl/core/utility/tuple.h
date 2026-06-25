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

//tuple
#pragma once

#include "rgl/core/types.h"
#include "rgl/core/traits.h"

namespace rgl {


    template<size_t... Ints>
    struct index_sequence {
        using type = index_sequence<Ints...>;
        static constexpr size_t size() noexcept { return sizeof...(Ints); }
    };

    template<size_t N, size_t... Ints>
    struct make_index_sequence_impl : make_index_sequence_impl<N - 1, N - 1, Ints...> {};

    template<size_t... Ints>
    struct make_index_sequence_impl<0, Ints...> {
        using type = index_sequence<Ints...>;
    };

    template<size_t N>
    using make_index_sequence = typename make_index_sequence_impl<N>::type;

    template<typename... Ts>
    class tuple;

    template<>
    class tuple<> {};

    template<typename T, typename... Rest>
    class tuple<T, Rest...> : private tuple<Rest...> {
    public:
        T head;

        tuple() : tuple<Rest...>(), head() {}
        
        template<typename T_, typename... Rest_>
        tuple(T_&& h, Rest_&&... rest) 
            : tuple<Rest...>(move(rest)...), head(move(h)) {}

        template<typename T_>
        tuple(const T_& h, const Rest&... rest) 
            : tuple<Rest...>(rest...), head(h) {}
    };

    template<size_t I, typename Tuple>
    struct get_impl;

    template<typename T, typename... Rest>
    struct get_impl<0, tuple<T, Rest...>> {
        using type = T;
        static type& get(tuple<T, Rest...>& t) noexcept { return t.head; }
        static const type& get(const tuple<T, Rest...>& t) noexcept { return t.head; }
    };

    template<size_t I, typename T, typename... Rest>
    struct get_impl<I, tuple<T, Rest...>> {
        using type = typename get_impl<I - 1, tuple<Rest...>>::type;
        static type& get(tuple<T, Rest...>& t) noexcept {
            return get_impl<I - 1, tuple<Rest...>>::get(t);
        }
        static const type& get(const tuple<T, Rest...>& t) noexcept {
            return get_impl<I - 1, tuple<Rest...>>::get(t);
        }
    };

    template<size_t I, typename... Ts>
    typename get_impl<I, tuple<Ts...>>::type& get(tuple<Ts...>& t) noexcept {
        return get_impl<I, tuple<Ts...>>::get(t);
    }

    template<typename Tuple>
    struct tuple_size;

    template<typename... Ts>
    struct tuple_size<tuple<Ts...>> {
        static constexpr size_t value = sizeof...(Ts);
    };

    namespace detail {
        template<typename F, typename Tuple, size_t... I>
        decltype(auto) apply_impl(F&& f, Tuple&& t, index_sequence<I...>) {
            return f(get<I>(t)...);
        }
        template<typename T>
        struct decay_impl {
            using type = typename remove_cv<T>::type;
        };

        template<typename T, unsigned long N>
        struct decay_impl<T[N]> {
            using type = T*;
        };

        template<typename T>
        struct decay_impl<T[]> {
            using type = T*;
        };

        template<typename R, typename... Args>
        struct decay_impl<R(Args...)> {
            using type = R(*)(Args...);
        };
    }

    template<typename F, typename Tuple>
    decltype(auto) apply(F&& f, Tuple&& t) {
        return detail::apply_impl(
            rgl::move(f), 
            rgl::move(t), 
            make_index_sequence<tuple_size<typename remove_reference<Tuple>::type>::value>{}
        );
    }
    template<typename T>
    struct decay {
        using type = typename detail::decay_impl<typename remove_reference<T>::type>::type;
    };

    template<typename T>
    using decay_t = typename decay<T>::type;

} // namespace rgl