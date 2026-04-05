/*
MIT License

Copyright (c) 2024 mguludag

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef MGUTILITY_DETAIL_UTILITY_HPP
#define MGUTILITY_DETAIL_UTILITY_HPP

#include "mgutility/_common/definitions.hpp"
#include <initializer_list>
#include <type_traits>
#include <utility>

namespace mgutility {
namespace detail {

/**
 * @brief Represents a compile-time sequence of indices.
 *
 * @tparam Ints The sequence of indices.
 */
template <std::size_t... Ints> struct index_sequence {};

/**
 * @brief Concatenates two index sequences.
 *
 * @tparam Seq1 The first index sequence.
 * @tparam Seq2 The second index sequence.
 */
template <typename Seq1, typename Seq2> struct index_sequence_concat;

template <std::size_t... I1, std::size_t... I2>
struct index_sequence_concat<index_sequence<I1...>, index_sequence<I2...>> {
  using type = index_sequence<I1..., (sizeof...(I1) + I2)...>;
};

/**
 * @brief Implementation helper for creating index sequences.
 *
 * @tparam N The size of the index sequence to create.
 */
template <std::size_t N> struct make_index_sequence_impl;

template <std::size_t N> struct make_index_sequence_impl {
private:
  static constexpr std::size_t half = N / 2;

  using first = typename make_index_sequence_impl<half>::type;
  using second = typename make_index_sequence_impl<N - half>::type;

public:
  using type = typename index_sequence_concat<first, second>::type;
};

// base cases
/**
 * @brief Base case for index sequence of size 0.
 */
template <> struct make_index_sequence_impl<0> {
  using type = index_sequence<>;
};

/**
 * @brief Base case for index sequence of size 1.
 */
template <> struct make_index_sequence_impl<1> {
  using type = index_sequence<0>;
};

/**
 * @brief Alias for creating an index sequence of size N.
 *
 * @tparam N The size of the index sequence.
 */
template <std::size_t N>
using make_index_sequence = typename make_index_sequence_impl<N>::type;
} // namespace detail

} // namespace mgutility

#endif // DETAIL_META_HPP
