/*! \file
 *
 * \brief Storage of generic data (inner class) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_DATASTORE__GENERICHOLDER_HPP_
#define PULSAR_GUARD_DATASTORE__GENERICHOLDER_HPP_

#include "pulsar/util/Mangle.hpp"
#include "pulsar/datastore/GenericBase.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/util/Serialization.hpp"

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <bphash/Hasher.hpp>


namespace pulsar{
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

    protected:
        //! The actual data
        std::shared_ptr<const T> obj;

        //! Hash of the object (if hashable)
        bphash::HashValue hash_;

    private:
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


/*! \brief Storage type for serialized data */
struct SerializedCacheData
{
    ByteArray data;
    std::string type;
    bphash::HashValue hash;
};



/*! \brief Holds serialized data in the cache
 *
 * When serialized data is loaded, it is kept as 
 * this type in the CacheData object. The CacheData
 * object will detect this and attempt to unserialize
 * when get() is called.
 */
class SerializedDataHolder : public GenericBase
{
    public:
        /*! \brief Construct via moving a data object
         * 
         * Will invoke move constructor for type T
         *
         * \throwno Throws an exception only if the move
         *          constructor for T throws an exception
         *
         * \param [in] m The object to move
         */
        SerializedDataHolder(std::shared_ptr<SerializedCacheData> m)
            : obj(std::move(m))
        { }


        // no other constructors, etc
        SerializedDataHolder(void)                                      = delete;
        SerializedDataHolder(const SerializedDataHolder &)              = delete;
        SerializedDataHolder(SerializedDataHolder &&)                   = delete;
        SerializedDataHolder & operator=(const SerializedDataHolder &)  = delete;
        SerializedDataHolder & operator=(SerializedDataHolder &&)       = delete;
        virtual ~SerializedDataHolder()                                 = default;



        ////////////////////////////////////////
        // Virtual functions from GenericBase
        ////////////////////////////////////////
        virtual const char * type(void) const noexcept
        {
            return obj->type.c_str();
        }

        virtual std::string demangled_type(void) const
        {
            return util::demangle_cpp(obj->type);
        }

        virtual bool is_serializable(void) const noexcept
        {
            return false;
        }

        virtual bool is_hashable(void) const noexcept
        {
            // hash will be empty if not hashable
            return obj->hash.size();
        }

        virtual bphash::HashValue my_hash(void) const
        {
            if(is_hashable())
                return obj->hash;
            else
                throw exception::GeneralException("hash called for unhashable cache data");
        }

        virtual ByteArray to_byte_array(void) const
        {
            throw exception::GeneralException("to_byte_array called for already-serialized data");
        }

        virtual void from_byte_array(const ByteArray & arr)
        {
            throw exception::GeneralException("from_byte_array called for already-serialized data");
        }

        template<typename T>
        std::unique_ptr<T> get(void) const
        {
            std::string desired_type = typeid(T).name();
            if(desired_type != obj->type)
                throw exception::GeneralException("Desired type does not match serialized data",
                                                  "desired", util::demangle_cpp(desired_type),
                                                  "stored", util::demangle_cpp(obj->type));

            return util::new_from_byte_array<T>(obj->data);
        }

    private:
        //! The actual data
        std::shared_ptr<SerializedCacheData> obj;
};




} //closing namespace detail
} //closing namespace datastore
} //closing namespace pulsar


#endif
