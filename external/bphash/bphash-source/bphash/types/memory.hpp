/*! \file
 *
 * \brief Hashing of std::unique_ptr and std::shared_ptr
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPHASH_GUARD_MEMORY_HPP_
#define BPHASH_GUARD_MEMORY_HPP_

#include "bphash/Hasher.hpp"
#include <memory>


namespace bphash {

/*! \brief Create a wrapper around a unique_ptr
 * 
 * \param [in] ptr The shared pointer to wrap
 * \param [in] len The number of elements that \p ptr points to
 * \return The wrapped pointer that is ready for hashing
 */
template<typename T, typename Deleter>
detail::PointerWrapper<T> hash_pointer(const std::unique_ptr<T, Deleter> & ptr, size_t len = 1)
{
    return detail::PointerWrapper<T> {typeid(std::unique_ptr<T, Deleter>).name(),
                                      ptr.get(), len
                                     };
}

/*! \brief Create a wrapper around a unique_ptr
 * 
 * \note Since this unique_ptr uses the array syntax, you must specify the length
 *
 * \param [in] ptr The shared pointer to wrap
 * \param [in] len The number of elements that \p ptr points to
 * \return The wrapped pointer that is ready for hashing
 */
template<typename T, typename Deleter>
detail::PointerWrapper<T> hash_pointer(const std::unique_ptr<T[], Deleter> & ptr, size_t len)
{
    return detail::PointerWrapper<T> {typeid(std::unique_ptr<T[], Deleter>).name(),
                                      ptr.get(), len
                                     };
}

/*! \brief Create a wrapper around a shared_ptr
 *
 * \param [in] ptr The shared pointer to wrap
 * \param [in] len The number of elements that \p ptr points to
 * \return The wrapped pointer that is ready for hashing
 */
template<typename T>
detail::PointerWrapper<T> hash_pointer(const std::shared_ptr<T> & ptr, size_t len = 1)
{
    return detail::PointerWrapper<T> {typeid(std::shared_ptr<T>).name(),
                                      ptr.get(), len
                                     };
}


/*
template<typename T>
detail::PointerWrapper<T> hash_pointer(const std::shared_ptr<T[]> & ptr, size_t len)
{
    return detail::PointerWrapper<T>{typeid(std::shared_ptr<T[]>).name(),
                                     ptr.get(), len};
}
*/

namespace detail {


/*! \brief Hashing of std::unique_ptr
 *
 * It is assumed that the pointer points to a single element.
 * If not, you must wrap the pointer in a PointerWrapper manually (via hash_pointer).
 */
template<typename T, typename Deleter>
struct ObjectHasher<std::unique_ptr<T, Deleter>> : public is_hashable<T>
{
    template<typename U = T>
    static
    typename std::enable_if<is_hashable<U>::value, void>::type
    hash(Hasher & hasher, const std::unique_ptr<U, Deleter> & obj)
    {
        hasher(hash_pointer(obj));
    }

    template<typename U = T>
    static
    typename std::enable_if<!is_hashable<U>::value, void>::type
    hash(Hasher & hasher, const std::unique_ptr<U, Deleter> & obj)
    {
        hasher(hash_pointer(obj));
    }
};


/*! \brief Hashing of std::shared_ptr
 *
 * It is assumed that the pointer points to a single element.
 * If not, you must wrap the pointer in a PointerWrapper manually (via hash_pointer).
 */
template<typename T>
struct ObjectHasher<std::shared_ptr<T>> : public is_hashable<T>
{
    template<typename U = T>
    static
    typename std::enable_if<is_hashable<U>::value, void>::type
    hash(Hasher & hasher, const std::shared_ptr<U> & obj)
    {
        hasher(hash_pointer(obj));
    }
};


} // close namespace detail
} // close namespace bphash

#endif
