/*! \file
 *
 * \brief Holder for generic data types
 */


#pragma once

#include "pulsar/datastore/GenericHolder.hpp"


namespace pulsar {
namespace datastore {
namespace detail {


/*! \brief Storage type for serialized data */
struct SerializedGenericData
{
    ByteArray data;
    std::string type;
    bphash::HashValue hash;
    unsigned int policy;
};


/*! \brief Storage of generic data that has been serialized
 *
 * This class stores serialized data (as a SerializedGenericData
 * object). It otherwise implements the GenericBase interface.
 *
 * One slight difference is that the type(), demangled_type(),
 * and my_hash() functions return the type of the serialized data, not the
 * type of the actual serialized storage object.
 *
 * \threadunsafe
 */
template<>
class GenericHolder<SerializedGenericData> : public GenericBase
{
    public:
        GenericHolder(const SerializedGenericData & m);
        GenericHolder(SerializedGenericData && m);

        GenericHolder(void)                                   = delete;
        GenericHolder(const GenericHolder & oph)              = delete;
        GenericHolder(GenericHolder && oph)                   = delete;
        GenericHolder & operator=(const GenericHolder & oph)  = delete;
        GenericHolder & operator=(GenericHolder && oph)       = delete;
        virtual ~GenericHolder()                              = default;


        std::shared_ptr<const SerializedGenericData>
        get(void) const noexcept;


        template<typename T>
        typename std::enable_if<util::SerializeCheck<T>::value,
                                std::unique_ptr<T>>::type
        unserialize(void) const
        {
            std::string desired_type = typeid(T).name();
            if(desired_type != obj->type)
                throw exception::GeneralException("Desired type does not match serialized data",
                                                  "desired", util::demangle_cpp(desired_type),
                                                  "stored", util::demangle_cpp(obj->type));

            return util::new_from_byte_array<T>(obj->data);
        }

        template<typename T>
        typename std::enable_if<!util::SerializeCheck<T>::value,
                                std::unique_ptr<T>>::type
        unserialize(void) const
        {
            // this is mostly to make compilers happy, since calls
            // unserialize() cannot be optimized out
            // (if that makes any sense, you have been programming in C++ for too long)
            std::string desired_type = typeid(T).name();
            throw exception::GeneralException("Attempting to unserialize non-serializable data??",
                                                  "desired", util::demangle_cpp(desired_type),
                                                  "stored", util::demangle_cpp(obj->type));
        }


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
        std::shared_ptr<const SerializedGenericData> obj;
};



/////////////////////////////////////////////
// GenericHolder member function definitions
/////////////////////////////////////////////
inline
GenericHolder<SerializedGenericData>::GenericHolder<SerializedGenericData>(const SerializedGenericData & m)
    : obj(std::make_shared<const SerializedGenericData>(m))
{ }

inline
GenericHolder<SerializedGenericData>::GenericHolder<SerializedGenericData>(SerializedGenericData && m)
    : obj(std::make_shared<const SerializedGenericData>(std::move(m)))
{ }

inline
std::shared_ptr<const SerializedGenericData>
GenericHolder<SerializedGenericData>::get(void) const noexcept
{
    return obj;
}

inline
const char * GenericHolder<SerializedGenericData>::type(void) const noexcept
{
    return obj->type.c_str();
}

inline
std::string GenericHolder<SerializedGenericData>::demangled_type(void) const
{
    return util::demangle_cpp(obj->type);
}

inline
bool GenericHolder<SerializedGenericData>::is_serializable(void) const noexcept
{
    return true;
}

inline
ByteArray GenericHolder<SerializedGenericData>::to_byte_array(void) const
{
    // it's already been serialized!
    return obj->data;
}

inline
bool GenericHolder<SerializedGenericData>::is_hashable(void) const noexcept
{
    // if the hash is empty, then it isn't hashable
    return obj->hash.size() != 0;
}

inline
bphash::HashValue GenericHolder<SerializedGenericData>::my_hash(void) const
{
    if(is_hashable())
        return obj->hash;
    else
        throw exception::GeneralException("hash called for unhashable cache data");
}


} //closing namespace detail
} //closing namespace datastore
} //closing namespace pulsar

