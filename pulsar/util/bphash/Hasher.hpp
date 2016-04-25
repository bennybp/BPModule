/*! \file
 *
 * \brief A class that hashes objects
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_BPHASH__HASHER_HPP_
#define PULSAR_GUARD_BPHASH__HASHER_HPP_

#include <typeinfo>
#include <cstring>

#include "pulsar/util/bphash/DataHasher.hpp"
#include "pulsar/util/bphash/Hash.hpp"
#include "pulsar/util/bphash/Hasher_fwd.hpp" // for DECLARE_HASHING_FRIENDS


namespace pulsar{
namespace util {



namespace detail {

// Empty structure to use as the base
template<typename T> struct ObjectHasher : public std::false_type { };

// Struct that wraps poitners (particularly arrays)
template<typename T>
struct PointerWrapper
{
    const char * type;
    const T * ptr;
    size_t len;
};

} // close namespace detail



/*! \brief Class that is used to hash objects
 *
 * This prevents the need for serializing the whole
 * object into memory, and then hashing. Instead,
 * we can do it on the fly.
 */
class Hasher
{
    public:
        Hasher() = default;

        Hasher(const Hasher &)             = delete;
        Hasher & operator=(const Hasher &) = delete;
        Hasher(Hasher &&)                  = default;
        Hasher & operator=(Hasher &&)      = default;


        /*! \brief Add an object to the hash
         *
         * Objects are progressively hashed until Finalize() is called
         *
         * Used to terminate variadic template
         */ 
        void operator()(void) { }


        /*! \brief Add an object to the hash
         *
         * Objects are progressively hashed until Finalize() is called
         */ 
        template<typename T, typename... Targs>
        void operator()(const T & obj, const Targs &... objs)
        {
            // hash the type of the object
            const char * typestr = typeid(T).name();
            size_t len = strlen(typestr);
            AddData(reinterpret_cast<void const *>(&len), sizeof(len));
            AddData(reinterpret_cast<void const *>(typestr), len);

            // Now hash the object itself
            HashSingle_(obj);

            // and the rest
            (*this)(objs...);
        }


        /*! \brief Add raw data to the hash
         * 
         * \param [in] data Pointer to the data to hash
         * \param [in] size Number of bytes of \p data to hash
         */
        void AddData(void const * data, size_t size)
        {
            dhasher_.Update(data, size);
        }


        /*! \brief Perform any remaining steps and return the hash
         */
        Hash Finalize(void) { return dhasher_.Finalize(); }

    private:

        // Hash a single object
        // Enabled if the type is an enum type
        template<typename T>
        typename std::enable_if<std::is_enum<T>::value, void>::type
        HashSingle_(const T & obj)
        {
            HashSingle_(static_cast<typename std::underlying_type<T>::type>(obj));
        }

        // Hash a single object
        // Enabled only if we have an ObjectHasher structure with the
        // appropriate type
        template<typename T>
        typename std::enable_if<detail::ObjectHasher<T>::value, void>::type
        HashSingle_(const T & obj)
        {
            detail::ObjectHasher<T>::Hash(*this, obj);
        }


        // Hash a single object
        // If we don't have an ObjectHasher structure with the
        // appropriate type, try calling the hash() function of
        // the object
        template<typename T>
        typename std::enable_if<!detail::ObjectHasher<T>::value &&
                                !std::is_enum<T>::value, void>::type
        HashSingle_(const T & obj)
        {
            obj.hash(*this);
        }


        // Internal hasher object
        DataHasher dhasher_;
};

/*! \brief Convenience function for hashing stuff in a single function call
 */
template<typename ... Targs>
Hash MakeHash(const Targs &... args)
{
    Hasher hasher;
    hasher(args...);
    return hasher.Finalize();
}


/*! \brief Convenience function for a range from a container
 */
template<typename InputIterator>
Hash MakeHashRange(InputIterator first, InputIterator last)
{
    Hasher hasher;

    for(auto it = first; it != last; ++it)
        hasher(*it);
    return hasher.Finalize();
}


//////////////////////////////////////////
// Helper for pointers
//////////////////////////////////////////


/*! \brief Wrap a raw pointer so that it can be hashed */
template<typename T>
detail::PointerWrapper<T> HashPointer(const T * ptr, size_t len = 1)
{
    return detail::PointerWrapper<T>{typeid(T *).name(), ptr, len};
}




namespace detail {


//////////////////////////////////////////
// Hashing pointers and the pointer wrapper
//////////////////////////////////////////
template<typename T>
struct ObjectHasher<PointerWrapper<T>> : public std::true_type
{
    static void
    Hash(Hasher & hasher, const PointerWrapper<T> & obj)
    {
        if(obj.ptr != nullptr)
        {
            hasher(obj.len);
            for(size_t i = 0; i < obj.len; i++)
                hasher(obj.ptr[i]);
        }
        else
            hasher(0);
    }
};

template<typename T>
struct ObjectHasher<T *> : public std::true_type
{
    static void
    Hash(Hasher & hasher, const T * obj)
    {
        hasher(HashPointer(obj, 1));
    }
};


//////////////////////////////////////////
// Arithmetic types
//////////////////////////////////////////
template<typename T>
struct FundamentalHasher : public std::true_type
{
    static void
    Hash(Hasher & hasher, const T & obj)
    {
        hasher.AddData(reinterpret_cast<void const *>(&obj),
                       static_cast<size_t>(sizeof(T)));
    }
};

#define DECLARE_FUNDAMENTAL_HASHER(type) \
    template<> struct ObjectHasher<type> : public FundamentalHasher<type> { };

DECLARE_FUNDAMENTAL_HASHER(bool)
DECLARE_FUNDAMENTAL_HASHER(char)
DECLARE_FUNDAMENTAL_HASHER(unsigned char)
DECLARE_FUNDAMENTAL_HASHER(signed char)
DECLARE_FUNDAMENTAL_HASHER(unsigned short)
DECLARE_FUNDAMENTAL_HASHER(signed short)
DECLARE_FUNDAMENTAL_HASHER(unsigned int)
DECLARE_FUNDAMENTAL_HASHER(signed int)
DECLARE_FUNDAMENTAL_HASHER(unsigned long)
DECLARE_FUNDAMENTAL_HASHER(signed long)
DECLARE_FUNDAMENTAL_HASHER(unsigned long long)
DECLARE_FUNDAMENTAL_HASHER(signed long long)

DECLARE_FUNDAMENTAL_HASHER(float)
DECLARE_FUNDAMENTAL_HASHER(double)
DECLARE_FUNDAMENTAL_HASHER(long double)

#undef DECLARE_FUNDAMENTAL_HASHER

} // close namespace detail





} // close namespace util
} // close namespace pulsar



#endif
