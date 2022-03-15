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
