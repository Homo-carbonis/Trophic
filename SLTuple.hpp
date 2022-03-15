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

#include <cstddef>
namespace SLTuple {
template<typename T, typename... Ts>
struct Tuple {
  Tuple(T a, Ts... bs) : a(a), b(bs...) {}
  Tuple() {}
  T a;
  Tuple<Ts...> b;
};
template<typename T>
struct Tuple<T> {
  Tuple(T a) : a(a) {}
  Tuple() {}
  T a;
};

template <size_t I, typename... Ts>
auto& get(Tuple<Ts...>& t) {
  if constexpr(I)
    return get<I-1>(t.b);
  else {
    auto& a = t.a;
    return a;
  }
}
}
