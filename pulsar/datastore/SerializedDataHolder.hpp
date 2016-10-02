/*! \file
 *
 * \brief Storage of generic data (inner class) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#pragma once

#include "pulsar/util/Mangle.hpp"
#include "pulsar/datastore/GenericBase.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/util/Serialization.hpp"


namespace pulsar {
namespace datastore {
namespace detail {


/*! \brief Storage type for serialized data */
struct SerializedCacheData
{
    ByteArray data;
    std::string type;
    bphash::HashValue hash;
    unsigned int policy;
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
        SerializedDataHolder(std::shared_ptr<SerializedCacheData> m);

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
        virtual const char * type(void) const noexcept;

        virtual std::string demangled_type(void) const;

        virtual bool is_serializable(void) const noexcept;

        virtual bool is_hashable(void) const noexcept;

        virtual bphash::HashValue my_hash(void) const;

        virtual ByteArray to_byte_array(void) const;

        virtual void from_byte_array(const ByteArray & arr);

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

        unsigned int policy(void) const;

    private:
        //! The actual data
        std::shared_ptr<SerializedCacheData> obj;
};




} //closing namespace detail
} //closing namespace datastore
} //closing namespace pulsar

