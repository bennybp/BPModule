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


namespace pulsar{
namespace datastore {
namespace detail {


/*! \brief A container that can hold anything
 *
 * \tparam T The type of the data this object is holding
 */ 
template<typename T, bool = util::SerializeCheck<T>::value>
class GenericHolder;


template<typename T>
class GenericHolderBase : public GenericBase
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
        GenericHolderBase(const T & m)
            : obj(std::make_shared<const T>(m))
        { }


        /*! \brief Construct via moving a data object
         * 
         * Will invoke move constructor for type T
         *
         * \throwno Throws an exception only if the move
         *          constructor for T throws an exception
         *
         * \param [in] m The object to move
         */
        GenericHolderBase(T && m)
            : obj(std::make_shared<const T>(std::move(m)))
        { }


        // no other constructors, etc
        GenericHolderBase(void)                                       = delete;
        GenericHolderBase(const GenericHolderBase & oph)              = delete;
        GenericHolderBase(GenericHolderBase && oph)                   = delete;
        GenericHolderBase & operator=(const GenericHolderBase & oph)  = delete;
        GenericHolderBase & operator=(GenericHolderBase && oph)       = delete;
        virtual ~GenericHolderBase()                                  = default;


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

        virtual bool is_serializable(void) const
        {
            return util::SerializeCheck<T>::value;
        }


    protected:
        //! The actual data
        std::shared_ptr<const T> obj;
};



template<typename T>
class GenericHolder<T, false> : public GenericHolderBase<T>
{
    public:
        using GenericHolderBase<T>::GenericHolderBase;

        virtual ByteArray to_byte_array(void) const
        {
            throw exception::GeneralException("to_byte_array called for unserializable cache data");
        }
};


template<typename T>
class GenericHolder<T, true> : public GenericHolderBase<T>
{
    public:
        using GenericHolderBase<T>::GenericHolderBase;

        virtual ByteArray to_byte_array(void) const
        {
            return util::to_byte_array(*(GenericHolderBase<T>::obj));
        }
};



/*! \brief Storage type for serialized data */
struct SerializedCacheData
{
    ByteArray data;
    std::string type;
    std::string demangled_type;
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
        SerializedDataHolder(SerializedCacheData && m)
            : obj(std::move(m))
        { }


        // no other constructors, etc
        SerializedDataHolder(void)                                      = delete;
        SerializedDataHolder(const SerializedDataHolder &)              = delete;
        SerializedDataHolder(SerializedDataHolder &&)                   = delete;
        SerializedDataHolder & operator=(const SerializedDataHolder &)  = delete;
        SerializedDataHolder & operator=(SerializedDataHolder &&)       = delete;
        virtual ~SerializedDataHolder()                                 = default;


        /*! Return a const reference to the underlying data
         *
         * \exnothrow
         *
         * \return A const reference to the underlying data
         */ 
        const SerializedCacheData & get(void) const noexcept
        {
            return obj;
        }


        ////////////////////////////////////////
        // Virtual functions from GenericBase
        ////////////////////////////////////////
        virtual const char * type(void) const noexcept
        {
            return obj.type.c_str();
        }

        virtual std::string demangled_type(void) const
        {
            return obj.demangled_type.c_str();
        }

        virtual bool is_serializable(void) const
        {
            return false;
        }

        virtual ByteArray to_byte_array(void) const
        {
            throw exception::GeneralException("to_byte_array called for already-serialized data");
        }


    private:
        //! The actual data
        SerializedCacheData obj;
};




} //closing namespace detail
} //closing namespace datastore
} //closing namespace pulsar


#endif
