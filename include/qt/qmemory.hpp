#ifndef QMEMORY_HPP
#define QMEMORY_HPP

#include <memory>
#include <type_traits>

class QObject;
class QThread;

namespace stdx {
namespace qt {
namespace _details {
template<typename T>
struct q_deleter
{
    // T is not QThread
    template <typename Q = T,
             typename std::enable_if<!std::is_base_of<QThread, Q>::value>::type* = nullptr>
    void operator()(T *ptr_)
    {
        static_assert(std::is_base_of<QObject, T>::value,
                      "stdx::qt::*_ptr<T>: T must be QObject or its children!");
        ptr_->deleteLater();
    }

    // T is QThread
    template <typename Q = T,
             typename std::enable_if<std::is_base_of<QThread, Q>::value>::type* = nullptr>
    void operator()(T *ptr_)
    {
        static_assert(std::is_base_of<QObject, T>::value,
                      "stdx::qt::*_ptr<T>: T must be QObject or its children!");
        ptr_->quit();
        ptr_->deleteLater();
    }
}; // struct q_deleter
} // namespace _details

template<typename T, typename D = _details::q_deleter<T>>
using unique_ptr = std::unique_ptr<T, _details::q_deleter<T>>;

template <class T, typename D = _details::q_deleter<T>>
unique_ptr<T> make_unique() {
    return unique_ptr<T, D>(new T());
}

template <class T, typename D = _details::q_deleter<T>, class... Ts>
unique_ptr<T> make_unique(Ts&&... args) {
    return unique_ptr<T, D>(new T(std::forward<Ts>(args)...));
}


template<typename T>
using shared_ptr = std::shared_ptr<T>;

template <class T, typename D = _details::q_deleter<T>>
shared_ptr<T> make_shared() {
    return shared_ptr<T>(new T(), _details::q_deleter<T>());
}

template <class T, typename D = _details::q_deleter<T>, class... Ts>
shared_ptr<T> make_shared(Ts&&... args) {
    return shared_ptr<T>(new T(std::forward<Ts>(args)...), _details::q_deleter<T>());
}
} // namespace qt
} // namespace stdx

#endif // QMEMORY_HPP
