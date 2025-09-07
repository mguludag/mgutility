#ifndef MGUTILITY_TUPLE_FOR_EACH_HPP
#define MGUTILITY_TUPLE_FOR_EACH_HPP

#include <tuple>

#if __cplusplus == 201103L

#define CNSTXPR

namespace mgutility {
namespace detail {
template <std::size_t...> struct index_sequence {};

template <std::size_t N, std::size_t... Next>
struct index_sequence_helper
    : public index_sequence_helper<N - 1U, N - 1U, Next...> {};

template <std::size_t... Next> struct index_sequence_helper<0U, Next...> {
  using type = index_sequence<Next...>;
};

template <std::size_t N>
using make_index_sequence = typename index_sequence_helper<N>::type;
} // namespace detail
} // namespace mgutility

#elif __cplusplus > 201103L

#define CNSTXPR constexpr

namespace mgutility {
namespace detail {
using std::index_sequence;
using std::make_index_sequence;
} // namespace detail
} // namespace mgutility

#endif

namespace mgutility {
namespace tuple {
template <typename T> struct element {
  const size_t index;
  T &value;
};

template <typename F, std::size_t... Is, typename... Ts,
          template <typename...> class Tuple>
CNSTXPR inline void __for_each_impl(Tuple<Ts...> t, F func,
                                    detail::index_sequence<Is...>) {
  using expander = int[];
  (void)expander{0, (func(element<Ts>{Is, std::get<Is>(t)}), 0)...};
}

template <typename F, typename... Ts, template <typename...> class Tuple>
CNSTXPR inline void for_each(Tuple<Ts...> t, F func) {
  __for_each_impl(t, func, detail::make_index_sequence<sizeof...(Ts)>());
}
} // namespace tuple
} // namespace mgutility

#endif
