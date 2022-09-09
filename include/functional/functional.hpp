#ifndef MGUTILITY_FUNCTIONAL_HPP
#define MGUTILITY_FUNCTIONAL_HPP

#ifdef _MSC_VER
#define FUNCTIONAL_CPLUSPLUS_VER _MSVC_LANG
#else
#define FUNCTIONAL_CPLUSPLUS_VER __cplusplus
#endif

#if FUNCTIONAL_CPLUSPLUS_VER < 201103L
#error "mgutility::function requires C++11 or higher!"
#endif


#if FUNCTIONAL_CPLUSPLUS_VER >= 201103L
#include <type_traits>
#include <utility>
#include <functional>
#if FUNCTIONAL_CPLUSPLUS_VER > 201703L
#include <concepts>
#endif // FUNCTIONAL_CPLUSPLUS_VER > 201703L

namespace mgutility
{
template <typename Func>
struct function{
#if FUNCTIONAL_CPLUSPLUS_VER > 201402L
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
#if FUNCTIONAL_CPLUSPLUS_VER > 201703L
    requires std::convertible_to<F, func_t>
#endif // FUNCTIONAL_CPLUSPLUS_VER > 201703L
    function(F function) : function_(std::move(function))
    {
#if FUNCTIONAL_CPLUSPLUS_VER <= 201703L
        static_assert(std::is_convertible<F, func_t>::value, && 
                      std::is_constructible<std::function<Func>, 
                      std::reference_wrapper<typename std::remove_reference<F>::type>>::value, 
                      "'mgutility::function<Func>::function_' has incomplete type!");
#endif // FUNCTIONAL_CPLUSPLUS_VER <= 201703L
    }
    
    template <typename... Args>
#if FUNCTIONAL_CPLUSPLUS_VER > 201703L
    requires std::invocable<Func&, Args...>
#endif // FUNCTIONAL_CPLUSPLUS_VER > 201703L

    inline auto operator()(Args&&... args)
#if FUNCTIONAL_CPLUSPLUS_VER > 201103L
    -> decltype(auto)
#else // FUNCTIONAL_CPLUSPLUS_VER == 201103L
    -> typename std::result_of<func_t(Args&&...)>::type 
#endif // FUNCTIONAL_CPLUSPLUS_VER > 201103L
    {
#if FUNCTIONAL_CPLUSPLUS_VER <= 201703L
        static_assert(is_invocable<func_t, Args...>::value, 
                      "Argument constraints not satisfied!");
#endif // FUNCTIONAL_CPLUSPLUS_VER <= 201703L
#if FUNCTIONAL_CPLUSPLUS_VER > 201402L
        return std::invoke(function_, std::forward<Args>(args)...);
#else  // FUNCTIONAL_CPLUSPLUS_VER <= 201402L
        return function_(std::forward<Args>(args)...);
#endif // FUNCTIONAL_CPLUSPLUS_VER > 201402L
    }
    
    template <typename... Args>
#if FUNCTIONAL_CPLUSPLUS_VER > 201703L
    requires std::invocable<Func&, Args...>
#endif // FUNCTIONAL_CPLUSPLUS_VER > 201703L

    inline auto operator()(Args&&... args) const 
#if FUNCTIONAL_CPLUSPLUS_VER > 201103L
    -> decltype(auto)
#else // FUNCTIONAL_CPLUSPLUS_VER == 201103L
    -> typename std::result_of<func_t(Args&&...)>::type 
#endif // FUNCTIONAL_CPLUSPLUS_VER > 201103L
    {
#if FUNCTIONAL_CPLUSPLUS_VER <= 201703L
        static_assert(is_invocable<func_t, Args...>::value, 
                      "Argument constraints not satisfied!");
#endif // FUNCTIONAL_CPLUSPLUS_VER <= 201703L
#if FUNCTIONAL_CPLUSPLUS_VER > 201402L
        return std::invoke(function_, std::forward<Args>(args)...);
#else  // FUNCTIONAL_CPLUSPLUS_VER <= 201402L
        return function_(std::forward<Args>(args)...);
#endif // FUNCTIONAL_CPLUSPLUS_VER > 201402L
    }
    
private:
    func_t function_;
};

#if FUNCTIONAL_CPLUSPLUS_VER > 201402L
template <typename F>
function(F) -> function<F>;
#endif // FUNCTIONAL_CPLUSPLUS_VER > 201402L
} // namespace mgutility
#endif // FUNCTIONAL_CPLUSPLUS_VER > 201103L

#endif // MGUTILITY_FUNCTIONAL_HPP
