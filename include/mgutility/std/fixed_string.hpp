/*
MIT License

Copyright (c) 2025 mguludag

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

#ifndef MGUTILITY_FIXED_STRING_HPP
#define MGUTILITY_FIXED_STRING_HPP

#include "mgutility/std/string_view.hpp"
#include "mgutility/std/utility.hpp"

namespace mgutility {

template <size_t N = 0> class fixed_string {
  template <size_t> friend class fixed_string;

public:
  template <size_t M>
  // NOLINTNEXTLINE [cppcoreguidelines-avoid-c-arrays]
  MGUTILITY_CNSTXPR static auto make(const char (&str)[M]) -> fixed_string<M> {
    return fixed_string<M>{str};
  }

  MGUTILITY_CNSTXPR fixed_string() = default;

  // --- safe pack expansion (no OOB) ---
  template <std::size_t M, std::size_t... Is>
  constexpr fixed_string(const char (&str)[M], detail::index_sequence<Is...>)
      : data_{(Is < M ? str[Is] : '\0')...}, cursor_(M ? M - 1 : 0) {}

  // single literal constructor (handles all M)
  template <std::size_t M>
  constexpr fixed_string(const char (&str)[M])
      : fixed_string(str, typename detail::make_index_sequence<N>{}) {}

  MGUTILITY_CNSTXPR fixed_string(const char *str, std::size_t len)
      : data_{}, cursor_(len < N ? len
                         : N     ? N - 1
                                 : 0) {
    for (std::size_t i = 0; i < N; ++i) {
      data_[i] = (i < len) ? str[i] : '\0';
    }
  }

  MGUTILITY_CNSTXPR fixed_string(mgutility::string_view str)
      : fixed_string(str.data(), str.size()) {}

  // Constructor to initialize from a string literal
  // NOLINTNEXTLINE [cppcoreguidelines-avoid-c-arrays]
  MGUTILITY_CNSTXPR explicit fixed_string(const char (&str)[N]) {
    for (size_t i = 0; i < N - 1; ++i) {
      data_[i] = str[i];
    }
    cursor_ = N - 1;
    data_[cursor_] = '\0';
  }

  // Concatenation operator
  template <size_t M>
  MGUTILITY_CNSTXPR auto operator+(const fixed_string<M> &other) const
      -> fixed_string<N + M - 1> {
    fixed_string<N + M - 1> result{};
    size_t idx = 0;
    for (; idx < cursor_; ++idx) {
      // NOLINTNEXTLINE [cppcoreguidelines-pro-bounds-constant-array-index]
      result.data_[idx] = data_[idx];
    }
    for (size_t j = 0; j < other.size(); ++j) {
      // NOLINTNEXTLINE [cppcoreguidelines-pro-bounds-constant-array-index]
      result.data_[idx + j] = other.data_[j];
    }
    result.cursor_ = cursor_ + other.size();
    // NOLINTNEXTLINE [cppcoreguidelines-pro-bounds-constant-array-index]
    result.data_[result.cursor_] = '\0';
    return result;
  }

  // Concatenation operator
  template <size_t M>
  // NOLINTNEXTLINE [cppcoreguidelines-avoid-c-arrays]
  MGUTILITY_CNSTXPR auto operator+(const char (&str)[M]) const
      -> fixed_string<N + M - 1> {
    return *this + fixed_string<M>{str};
  }

  template <size_t M>
  // NOLINTNEXTLINE [cppcoreguidelines-avoid-c-arrays]
  MGUTILITY_CNSTXPR auto append(const char (&str)[M]) -> fixed_string<N> & {
    static_assert(N > M,
                  "Capacity needs to be greater than string to be appended!");
    for (size_t i = 0; i < M - 1; ++i) {
      // NOLINTNEXTLINE [cppcoreguidelines-pro-bounds-constant-array-index]
      data_[cursor_++] = str[i];
    }
    // NOLINTNEXTLINE [cppcoreguidelines-pro-bounds-constant-array-index]
    data_[cursor_] = '\0';
    return *this;
  }

  MGUTILITY_CNSTXPR auto append(string_view str) -> fixed_string<N> & {
    auto len = str.size() > N - cursor_ ? N - cursor_ : str.size();
    for (std::size_t i = 0; i < len; ++i) {
      // NOLINTNEXTLINE [cppcoreguidelines-pro-bounds-constant-array-index]
      data_[cursor_++] = str[i];
    }
    // NOLINTNEXTLINE [cppcoreguidelines-pro-bounds-constant-array-index]
    data_[cursor_] = '\0';
    return *this;
  }

  MGUTILITY_CNSTXPR auto pop_back() -> void {
    if (cursor_ > 0) {
      // NOLINTNEXTLINE [cppcoreguidelines-pro-bounds-constant-array-index]
      data_[--cursor_] = '\0';
    }
  }

  MGUTILITY_CNSTXPR auto size() const -> size_t { return cursor_; }

  // NOLINTNEXTLINE [readability-identifier-length]
  constexpr size_t find(char c, size_t pos = 0) const noexcept {
    // NOLINTNEXTLINE [readability-avoid-nested-conditional-operator]
    return c == data_[pos] ? pos : (pos < cursor_ ? find(c, ++pos) : npos);
  }

  // Conversion to std::string_view for easy printing
  // NOLINTNEXTLINE[google-explicit-constructor]
  MGUTILITY_CNSTXPR operator string_view() const {
    return string_view(data_, cursor_);
  }

  MGUTILITY_CNSTXPR auto view() const -> string_view {
    return string_view(data_, cursor_);
  }

  constexpr const char *data() const noexcept { return data_; }

  constexpr bool empty() const noexcept { return cursor_ == 0; }

  constexpr const char &operator[](size_t index) const noexcept {
    // NOLINTNEXTLINE [cppcoreguidelines-pro-bounds-constant-array-index]
    return data_[index];
  }

  MGUTILITY_CNSTXPR inline bool operator==(const char *rhs) const {
    return view() == rhs;
  }

  // NOLINTNEXTLINE [readability-identifier-length]
  friend std::ostream &operator<<(std::ostream &os,
                                  const fixed_string<N> &str) {
    for (size_t i = 0; i < str.cursor_; ++i) {
      // NOLINTNEXTLINE [cppcoreguidelines-pro-bounds-constant-array-index]
      os << str.data_[i];
    }
    return os;
  }

  static constexpr auto npos = -1;

private:
  // NOLINTNEXTLINE [cppcoreguidelines-avoid-c-arrays]
  char data_[N]{'\0'};
  size_t cursor_{};
};

} // namespace mgutility

#if defined(__cpp_lib_format)

#include <format>

/**
 * @brief Formatter for enum types for use with std::format.
 *
 * @tparam Enum The enum type.
 */
template <size_t N>
struct std::formatter<mgutility::fixed_string<N>>
    : formatter<std::string_view> {
  auto constexpr format(const mgutility::fixed_string<N> &str,
                        format_context &ctx) const {
    return formatter<std::string_view>::format(str.view().data(), ctx);
  }
};

#endif

#if defined(ENUM_NAME_USE_FMT) ||                                              \
    (defined(MGUTILITY_HAS_HAS_INCLUDE) && __has_include(<fmt/format.h>))
#include <fmt/format.h>

template <size_t N>
struct fmt::formatter<mgutility::fixed_string<N>> : formatter<string_view> {
  auto format(const mgutility::fixed_string<N> &str, format_context &ctx) const
      -> appender {
    return formatter<string_view>::format(str.view().data(), ctx);
  }
};
#endif // MGUTILITY_USE_FMT || __has_include(<fmt/format.h>)

#endif // MGUTILITY_FIXED_STRING_HPP
