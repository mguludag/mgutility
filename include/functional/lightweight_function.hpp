#ifndef LIGHTWEIGHT_FUNC_HPP
#define LIGHTWEIGHT_FUNC_HPP

#ifdef _MSC_VER
#define LWFUNC_CPLUSPLUS _MSVC_LANG
#else
#define LWFUNC_CPLUSPLUS __cplusplus
#endif

#if LWFUNC_CPLUSPLUS < 201103L
#error "mgutility::lightweight_function requires C++11 or higher!"
#endif


#if LWFUNC_CPLUSPLUS >= 201103L
#include <type_traits>
#include <utility>
#include <functional>
#if LWFUNC_CPLUSPLUS > 201703L
#include <concepts>
#endif // LWFUNC_CPLUSPLUS > 201703L

namespace mgutility
{
template <typename Func>
struct lightweight_function{
#if LWFUNC_CPLUSPLUS > 201402L
    using func_t = std::conditional_t<std::is_function_v<Func>, 
                   std::add_pointer_t<Func>, Func>;
    
    template <typename F, typename... Args>
    using is_invocable = std::is_invocable<F, Args...>;   
#else
    using func_t = typename std::conditional<std::is_function<Func>::value,
                        typename std::add_pointer<Func>::type, Func>::type;

    template <typename F, typename... Args>
    struct is_invocable :
    std::is_constructible<std::function<void(Args ...)>, 
    std::reference_wrapper<typename std::remove_reference<F>::type>>
    {};
#endif
    template <typename F>
#if LWFUNC_CPLUSPLUS > 201703L
    requires std::convertible_to<F, func_t>
#endif // LWFUNC_CPLUSPLUS > 201703L
    lightweight_function(F function) : function_(std::move(function))
    {
#if LWFUNC_CPLUSPLUS <= 201703L
        static_assert(std::is_convertible<F, func_t>::value, && 
                      std::is_constructible<std::function<Func>, 
                      std::reference_wrapper<typename std::remove_reference<F>::type>>::value, 
                      "'mgutility::lightweight_function<Func>::function_' has incomplete type!");
#endif // LWFUNC_CPLUSPLUS <= 201703L
    }
    
    template <typename... Args>
#if LWFUNC_CPLUSPLUS > 201703L
    requires std::invocable<Func&, Args...>
#endif // LWFUNC_CPLUSPLUS > 201703L
#if LWFUNC_CPLUSPLUS > 201103L
    inline decltype(auto) operator()(Args&&... args)
#else
    inline auto operator()(Args&&... args) -> typename std::result_of<func_t(Args&&...)>::type 
#endif // LWFUNC_CPLUSPLUS > 201103L
    {
#if LWFUNC_CPLUSPLUS <= 201703L
        static_assert(is_invocable<func_t, Args...>::value, 
                      "Argument constraints not satisfied!");
#endif // LWFUNC_CPLUSPLUS <= 201703L
#if LWFUNC_CPLUSPLUS > 201402L
        return std::invoke(function_, std::forward<Args>(args)...);
#else
        return function_(std::forward<Args>(args)...);
#endif // LWFUNC_CPLUSPLUS > 201402L
    }
    
    template <typename... Args>
#if LWFUNC_CPLUSPLUS > 201703L
    requires std::invocable<Func&, Args...>
#endif // LWFUNC_CPLUSPLUS > 201703L
#if LWFUNC_CPLUSPLUS > 201103L
    inline decltype(auto) operator()(Args&&... args) const
#else
    inline auto operator()(Args&&... args) const -> typename std::result_of<func_t(Args&&...)>::type 
#endif // LWFUNC_CPLUSPLUS > 201103L
    {
#if LWFUNC_CPLUSPLUS <= 201703L
        static_assert(is_invocable<func_t, Args...>::value, 
                      "Argument constraints not satisfied!");
#endif // LWFUNC_CPLUSPLUS <= 201703L
#if LWFUNC_CPLUSPLUS > 201402L
        return std::invoke(function_, std::forward<Args>(args)...);
#else
        return function_(std::forward<Args>(args)...);
#endif // LWFUNC_CPLUSPLUS > 201402L
    }
    
private:
    func_t function_;
};

#if LWFUNC_CPLUSPLUS > 201402L
template <typename F>
lightweight_function(F) -> lightweight_function<F>;
#endif // LWFUNC_CPLUSPLUS > 201402L
} // namespace mgutility
#endif // LWFUNC_CPLUSPLUS > 201103L

#endif // LIGHTWEIGHT_FUNC_HPP
