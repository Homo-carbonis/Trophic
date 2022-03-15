// Copyright Ptolom 2022
// This file is part of Trophic.
//
// Trophic is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with this
// program. If not, see <https://www.gnu.org/licenses/>. 

namespace meta {
template<typename... Ts>
struct list {
  using value = list<Ts...>;
};

template<template <typename> typename F, typename... Ts> struct apply;
template<template <typename> typename F, typename... Ts>
struct apply<F, list<Ts...>>{
  using value = F<Ts...>;
};

template<typename... Ts> struct cons;
template<typename T, typename... Ts>
struct cons<T,list<Ts...>> {
  using value = list<T,Ts...>; //list
};

template<template <typename> typename F, typename T, typename... Ts>
struct map {
  using value = typename cons<typename F<T>::value, typename map<F,Ts...>::value>::value; //list
};

template<typename... Ts> struct concat { using value = list<>; };
template<typename T, typename... Ts>
struct concat<T,Ts...> {
  using value = typename cons<typename T::value, typename concat<Ts...>::value>::value; //list
};

template<typename... Ts> const size_t sizeofall;
template<typename... Ts>
const size_t sizeofall<list<Ts...>> = (sizeof(Ts) + ...);
}
