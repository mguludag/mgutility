#ifndef ARITHMETIC_RANGES_HPP
#define ARITHMETIC_RANGES_HPP
#include <type_traits>
#include <utility>

namespace mgutility {

template <bool B, class T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

template <typename T,
          enable_if_t<std::is_arithmetic<T>::value, bool> = true>
bool check_in_range(T value, std::pair<T, T> bounds, bool include_bounds = false) {
    return include_bounds ? (value >= bounds.first && value <= bounds.second)
                          : (value > bounds.first && value < bounds.second);
}

} // namespace mgutility

#endif // ARITHMETIC_RANGES_HPP
