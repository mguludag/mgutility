#ifndef MGUTILITY_SINGLETON_HPP
#define MGUTILITY_SINGLETON_HPP

#include <memory>

// original implementation linked here: https://oguzhankatli.medium.com/dogru-singleton-tasarimi-3f5c10bac6c7
// I modified the implementation using std::aligned_storage instead of pointers and heap

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
    
// its a CRTP base class for change child class to singleton
template <typename T>
class singleton_from_this
{
public:
    template <typename ...Ts>
    static void init_instance(Ts&& ...args) {
        static_assert(std::is_constructible<T, Ts...>::value, "Cannot construct instance with these arguments!");
        struct static_creator
        {
            static_creator(Ts&& ...args){
                if(instance_ != nullptr) return;
                static T instance_l{std::forward<Ts>(args)...};
                instance_ = &instance_l;
            }
        };

        static static_creator creator(std::forward<Ts>(args)...);
        
    }

    static T &instance() {
        return *instance_;
    }

    template <typename TT, typename ...TTs>
    static T &instance(TT&& arg, TTs&& ...args) {
        init_instance(std::forward<TT>(arg), std::forward<TTs>(args)...);
        return *instance_;
    }

private:
static T* instance_;
};

template <typename T> T* singleton_from_this<T>::instance_{nullptr};

}


#endif //MGUTILITY_SINGLETON_HPP
