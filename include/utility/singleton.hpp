#ifndef MGUTILITY_SINGLETON_HPP
#define MGUTILITY_SINGLETON_HPP

#include <memory>

namespace mgutility
{
template <typename T>
class singleton 
{
    using Type = typename std::remove_all_extents<T>::type;
    using Storage = typename std::aligned_storage<sizeof(Type), alignof(max_align_t)>;
    
    template <typename To>
    static inline To* stored(Storage& s) { return static_cast<To*>(static_cast<void*>(&s)); } 
    
public:
    template <typename ...Args>
    singleton(Args&& ...args)
    {
        struct static_creator {
            static_creator(Storage &s, Args&& ...arguments) {
                new(static_cast<void*>(&s)) Type(std::forward<Args>(arguments)...);
            }
        };
        static static_creator _creator(storage_, std::forward<Args>(args)... );
    }
    static inline Type *instance() { return stored<Type>(storage_); }

    inline Type * operator->() { return &**this; }
    inline const Type* operator->() const { return &**this; }
    inline Type& operator *() { return *stored<Type>(storage_); }
    inline const Type& operator *() const { return *stored<const Type>(storage_); }
private:
    static Storage storage_;
};

template <typename T> typename singleton<T>::Storage singleton<T>::storage_;

}


#endif //MGUTILITY_SINGLETON_HPP
