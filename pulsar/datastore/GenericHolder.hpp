/*! \file
 *
 * \brief Holder for generic data types
 */


#pragma once

#include "pulsar/util/Mangle.hpp"
#include "pulsar/datastore/GenericBase.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/util/Serialization.hpp"

#include <bphash/Hasher.hpp>


namespace pulsar {
namespace datastore {
namespace detail {

/*! \brief Storage of generic data
 *
 * This class is used to store generic data (via a pointer
 * to the GenericBase class) in a container.
 *
 * \tparam T The type of data actually stored
 *
 * \threadunsafe
 */
template<typename T>
class GenericHolder : public GenericBase
{
    public:
        /*! \brief Construct via copying a data object
         *
         * Will invoke copy constructor for type \p T
         *
         * \throwno Throws an exception only if the move
         *          constructor for \p T throws an exception
         *
         *  \param [in] m The object to copy
         */
        GenericHolder(const T & m);


        /*! \brief Construct via moving a data object
         *
         * Will invoke move constructor for type T
         *
         * \throwno Throws an exception only if the move
         *          constructor for T throws an exception
         *
         * \param [in] m The object to move
         */
        GenericHolder(T && m);


        // no other constructors, etc
        GenericHolder(void)                                   = delete;
        GenericHolder(const GenericHolder & oph)              = delete;
        GenericHolder(GenericHolder && oph)                   = delete;
        GenericHolder & operator=(const GenericHolder & oph)  = delete;
        GenericHolder & operator=(GenericHolder && oph)       = delete;
        virtual ~GenericHolder()                              = default;


        /*! Returns the stored data
         *
         * The data contained in the shared pointer may be shared
         * with other GenericHolder objects.
         *
         * \return A shared_ptr containing the stored data
         */
        std::shared_ptr<const T> get(void) const noexcept;


        ////////////////////////////////////////
        // Virtual functions from GenericBase
        ////////////////////////////////////////

        virtual const char * type(void) const noexcept;

        virtual std::string demangled_type(void) const;



        ///////////////////////////////
        // Serialization
        ///////////////////////////////

        virtual bool is_serializable(void) const noexcept;

        virtual ByteArray to_byte_array(void) const;

        ///////////////////////////////
        // Hashing
        ///////////////////////////////
        virtual bool is_hashable(void) const noexcept;

        virtual bphash::HashValue my_hash(void) const;


    private:
        //! The actual data
        std::shared_ptr<const T> obj;

        //! Hash of the object (if hashable)
        bphash::HashValue hash_;


        // These are all helper function that may throw exceptions if you
        // are trying to do something invalid.
        // Only one function from each pair will be valid.
        template<typename U = T>
        typename std::enable_if<util::SerializeCheck<U>::value, ByteArray>::type
        to_byte_array_helper_(void) const;

        template<typename U = T>
        typename std::enable_if<!util::SerializeCheck<U>::value, ByteArray>::type

        to_byte_array_helper_(void) const;

        template<typename U = T>
        typename std::enable_if<bphash::is_hashable<U>::value, void>::type
        make_my_hash_(void);

        template<typename U = T>
        typename std::enable_if<!bphash::is_hashable<U>::value, void>::type
        make_my_hash_(void);
};



/////////////////////////////////////////////
// GenericHolder member function definitions
/////////////////////////////////////////////
template<typename T>
GenericHolder<T>::GenericHolder(const T & m)
    : obj(std::make_shared<const T>(m))
{
    if(is_hashable())
        make_my_hash_();
}

template<typename T>
GenericHolder<T>::GenericHolder(T && m)
    : obj(std::make_shared<const T>(std::move(m)))
{
    if(is_hashable())
        make_my_hash_();
}

template<typename T>
std::shared_ptr<const T> GenericHolder<T>::get(void) const noexcept
{
    return obj;
}

template<typename T>
const char * GenericHolder<T>::type(void) const noexcept
{
    return typeid(T).name();
}

template<typename T>
std::string GenericHolder<T>::demangled_type(void) const
{
    return util::demangle_cpp_or_py_type(*obj);
}

template<typename T>
bool GenericHolder<T>::is_serializable(void) const noexcept
{
    return util::SerializeCheck<T>::value;
}

template<typename T>
ByteArray GenericHolder<T>::to_byte_array(void) const
{
    return to_byte_array_helper_();
}

template<typename T>
bool GenericHolder<T>::is_hashable(void) const noexcept
{
    return bphash::is_hashable<T>::value;
}


template<typename T>
bphash::HashValue GenericHolder<T>::my_hash(void) const
{
    if(is_hashable())
        return hash_;
    else
        throw GeneralException("hash called for unhashable cache data");
}



template<typename T>
template<typename U>
typename std::enable_if<util::SerializeCheck<U>::value, ByteArray>::type
GenericHolder<T>::to_byte_array_helper_(void) const
{
    return util::to_byte_array(*obj);
}

template<typename T>
template<typename U>
typename std::enable_if<!util::SerializeCheck<U>::value, ByteArray>::type
GenericHolder<T>::to_byte_array_helper_(void) const
{
    //! \todo can be static assert?
    throw GeneralException("to_byte_array called for non-serializable cache data");
}

template<typename T>
template<typename U>
typename std::enable_if<bphash::is_hashable<U>::value, void>::type
GenericHolder<T>::make_my_hash_(void)
{
    hash_ = bphash::make_hash(bphash::HashType::Hash128, *obj);
}

template<typename T>
template<typename U>
typename std::enable_if<!bphash::is_hashable<U>::value, void>::type
GenericHolder<T>::make_my_hash_(void)
{
    //! \todo can be static assert?
    throw GeneralException("hash called for unhashable cache data");
}


} //closing namespace detail
} //closing namespace datastore
} //closing namespace pulsar

