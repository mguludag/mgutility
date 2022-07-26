#ifndef TOKENIZE_HPP
#define TOKENIZE_HPP

#include <string>
#if __cplusplus > 201700L
#include <string_view>
#endif
#include <vector>

namespace mgutility {
#if __cplusplus > 201700L
inline std::vector<std::string> tokenize(std::string_view str, std::string_view delim)
#else
inline std::vector<std::string> tokenize(const std::string& str, const std::string& delim)
#endif
{
    std::vector<std::string> tokens_vec;
    tokens_vec.reserve(str.size());
    size_t start = 0, end = 0;

    while ((end = str.find(delim, start)) != std::string::npos) {
        auto token = str.substr(start, end - start);
        tokens_vec.emplace_back(token);
        start = end + delim.size();
    }

    tokens_vec.emplace_back(str.substr(start, str.size()));

    return tokens_vec;
}

#if __cplusplus > 201700L
inline std::vector<std::string> tokenize(std::string_view str, char delim = ' ')
#else
inline std::vector<std::string> tokenize(const std::string& str, char delim = ' ')
#endif
{
    std::vector<std::string> tokens_vec;
    tokens_vec.reserve(str.size());
    size_t start = 0, end = 0;

    while ((end = str.find(delim, start)) != std::string::npos) {
        auto token = str.substr(start, end - start);
        tokens_vec.emplace_back(token);
        start = end + 1;
    }

    tokens_vec.emplace_back(str.substr(start, str.size()));

    return tokens_vec;
}
} // namespace mgutility

#endif // TOKENIZE_HPP
