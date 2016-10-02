/*! \file
 *
 * \brief Storage of generic data (inner class) 
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


template<typename T>
class GenericHolder : public GenericBase
{
    public:
        /*! \brief Construct via copying a data object
         * 
         * Will invoke copy constructor for type T
         *
         * \throwno Throws an exception only if the move
         *          constructor for T throws an exception
         *
         *  \param [in] m The object to copy
         */
        GenericHolder(const T & m)
            : obj(std::make_shared<const T>(m))
        {
            if(is_hashable())
                make_my_hash_();
        }


        /*! \brief Construct via moving a data object
         * 
         * Will invoke move constructor for type T
         *
         * \throwno Throws an exception only if the move
         *          constructor for T throws an exception
         *
         * \param [in] m The object to move
         */
        GenericHolder(T && m)
            : obj(std::make_shared<const T>(std::move(m)))
        { }


        // no other constructors, etc
        GenericHolder(void)                                       = delete;
        GenericHolder(const GenericHolder & oph)              = delete;
        GenericHolder(GenericHolder && oph)                   = delete;
        GenericHolder & operator=(const GenericHolder & oph)  = delete;
        GenericHolder & operator=(GenericHolder && oph)       = delete;
        virtual ~GenericHolder()                                  = default;


        /*! Return a const reference to the underlying data
         *
         * \exnothrow
         *
         * \return A const reference to the underlying data
         */ 
        std::shared_ptr<const T> get(void) const noexcept
        {
            return obj;
        }


        ////////////////////////////////////////
        // Virtual functions from GenericBase
        ////////////////////////////////////////
        virtual const char * type(void) const noexcept
        {
            return typeid(T).name();
        }

        virtual std::string demangled_type(void) const
        {
            return util::demangle_cpp_or_py_type(*obj);
        }



        ///////////////////////////////
        // Serialization
        ///////////////////////////////

        virtual bool is_serializable(void) const noexcept
        {
            return util::SerializeCheck<T>::value;
        }

        virtual ByteArray to_byte_array(void) const
        {
            return to_byte_array_helper_();
        }

        virtual void from_byte_array(const ByteArray & arr)
        {
            from_byte_array_helper_(arr);
        }

        ///////////////////////////////
        // Hashing
        ///////////////////////////////
        virtual bool is_hashable(void) const noexcept
        {
            return bphash::is_hashable<T>::value;
        }

        virtual bphash::HashValue my_hash(void) const
        {
            if(is_hashable())
                return hash_;
            else
                throw exception::GeneralException("hash called for unhashable cache data");
        }

    private:
        //! The actual data
        std::shared_ptr<const T> obj;

        //! Hash of the object (if hashable)
        bphash::HashValue hash_;


        template<typename U = T>
        typename std::enable_if<util::SerializeCheck<U>::value, ByteArray>::type
        to_byte_array_helper_(void) const
        {
            return util::to_byte_array(*(GenericHolder<T>::obj));
        }

        template<typename U = T>
        typename std::enable_if<!util::SerializeCheck<U>::value, ByteArray>::type
        to_byte_array_helper_(void) const
        {
            //! \todo can be static assert?
            throw exception::GeneralException("to_byte_array called for unserializable cache data");
        }

        template<typename U = T>
        typename std::enable_if<util::SerializeCheck<U>::value, void>::type
        from_byte_array_helper_(const ByteArray & arr)
        {
            GenericHolder<T>::obj = std::make_shared<T>(util::from_byte_array<T>(arr));
        }

        template<typename U = T>
        typename std::enable_if<!util::SerializeCheck<U>::value, void>::type
        from_byte_array_helper_(const ByteArray & arr)
        {
            //! \todo can be static assert?
            throw exception::GeneralException("from_byte_array called for unserializable cache data");
        }


        template<typename U = T>
        typename std::enable_if<bphash::is_hashable<U>::value, void>::type
        make_my_hash_(void)
        {
            hash_ = bphash::make_hash(bphash::HashType::Hash128, *obj);
        }

        template<typename U = T>
        typename std::enable_if<!bphash::is_hashable<U>::value, void>::type
        make_my_hash_(void)
        {
            //! \todo can be static assert?
            throw exception::GeneralException("hash called for unhashable cache data");
        }
};


} //closing namespace detail
} //closing namespace datastore
} //closing namespace pulsar

