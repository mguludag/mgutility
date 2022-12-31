#ifndef MGUTILITY_TUPLE_FOR_EACH_HPP
#define MGUTILITY_TUPLE_FOR_EACH_HPP

#include <tuple>

#if __cplusplus > 201103L

namespace mgutility
{
  namespace tuple
  {
    template<typename T, std::size_t N>
    struct tuple_element { T& value; static constexpr auto index = N; };

    template <typename ...Ts, class F, std::size_t... Is>
    constexpr inline void __for_each_impl(std::tuple<Ts...> t, F func, std::index_sequence<Is...>)
    {
      using expander = int[];
      static_cast<void>(expander{0, (func(tuple_element<decltype(std::get<Is>(t)), Is>{std::get<Is>(t)}), 0)...});
    }

    template <typename ...Ts, typename F>
    constexpr inline void for_each(std::tuple<Ts...> t, F func)
    {
      __for_each_impl(t, func, std::make_index_sequence<std::tuple_size<decltype(t)>::value>());
    }
  }
}

#endif

#endif
