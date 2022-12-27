/*****************************************************************************
 * shared_memory.hpp
 *
 * Created: 5/15/2021 2021 by mguludag
 *
 * Copyright 2021 mguludag. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef SHARED_MEMORY_HPP
#define SHARED_MEMORY_HPP

#include <vector>
#include <deque>
#include <mutex>


namespace mgutility {

template<typename T, template<typename Val, typename...> class Container = std::deque>
class shared_memory
{
public:
    shared_memory() {}
    
    shared_memory(size_t reserve) 
    {
        static_assert(std::is_member_function_pointer<decltype(&Container::reserve)>::value,
                  "Container::reserve member function is required!");
        m_data.reserve(reserve);
    }

    /*!
     * \brief at function returns item in shared memory with given index
     * \param index
     * \return T&
     */
    static T at(int index)
    {
        static_assert(std::is_member_function_pointer<decltype(&Container::at)>::value,
                  "Container::at member function is required!"); 
        std::lock_guard<std::mutex> guard(m_mtx);
        return m_data.at(index);
    }

    /*!
     * \brief data function returns the container that used in shared memory
     * \return Container<T>
     */
    static Container<T> data()
    {
        std::lock_guard<std::mutex> guard(m_mtx);
        return m_data;
    }


    /*!
     * \brief push_back function adds the element at the last position in the shared memory
     * \param data
     */
    static void push_back(const T &data)
    {
        static_assert(std::is_member_function_pointer<decltype(&Container::push_back)>::value,
                  "Container::push_back member function is required!"); 
        std::lock_guard<std::mutex> guard(m_mtx);
        m_data.push_back(data);
    }
    
        /*!
     * \brief push_back function adds the element at the last position in the shared memory
     * \param data
     */
    static void push_back(T &&data)
    {
        static_assert(std::is_member_function_pointer<decltype(&Container::push_back)>::value,
                  "Container::push_back member function is required!"); 
        std::lock_guard<std::mutex> guard(m_mtx);
        m_data.push_back(data);
    }

    /*!
     * \brief clear function clears the shared memory
     */
    static void clear()
    {
        static_assert(std::is_member_function_pointer<decltype(&Container::clear)>::value,
                  "Container::clear member function is required!"); 
        std::lock_guard<std::mutex> guard(m_mtx);
        m_data.clear();
    }

    /*!
     * \brief pop_front pops and return the first element from the shared memory
     * \return T
     */
    static T pop_front()
    {
        static_assert(std::is_member_function_pointer<decltype(&Container::at)>::value,
                  "Container::at member function is required!"); 
        static_assert(std::is_member_function_pointer<decltype(&Container::erase)>::value,
                  "Container::erase member function is required!");
        std::lock_guard<std::mutex> guard(m_mtx);
        auto data = m_data.at(0);
        m_data.erase(0);
        return data;
    }


private:
    static Container<T> m_data;
    static std::mutex m_mtx;
};

template<typename T, template<typename Val, typename...> class Container = std::deque>
Container<T> shared_memory<T, Container>::m_data;

template<typename T, template<typename Val, typename...> class Container = std::deque>
std::mutex shared_memory<T, Container>::m_mtx;

} // namespace mgutility


#endif // SHARED_MEMORY_HPP
