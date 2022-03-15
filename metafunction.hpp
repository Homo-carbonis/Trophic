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
