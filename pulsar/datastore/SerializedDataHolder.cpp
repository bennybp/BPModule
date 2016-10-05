/*! \file
 *
 * \brief Storage of generic data (inner class) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "pulsar/datastore/SerializedDataHolder.hpp"


namespace pulsar {
namespace datastore {
namespace detail {


SerializedDataHolder::SerializedDataHolder(std::shared_ptr<SerializedCacheData> m)
    : obj(std::move(m))
{ }


const char * SerializedDataHolder::type(void) const noexcept
{
    return obj->type.c_str();
}

std::string SerializedDataHolder::demangled_type(void) const
{
    return util::demangle_cpp(obj->type);
}

bool SerializedDataHolder::is_serializable(void) const noexcept
{
    return true;
}

bool SerializedDataHolder::is_hashable(void) const noexcept
{
    // hash will be empty if not hashable
    return obj->hash.size();
}

bphash::HashValue SerializedDataHolder::my_hash(void) const
{
    if(is_hashable())
        return obj->hash;
    else
        throw exception::GeneralException("hash called for unhashable cache data");
}

ByteArray SerializedDataHolder::to_byte_array(void) const
{
    return obj->data;
}

void SerializedDataHolder::from_byte_array(const ByteArray & arr)
{
    obj->data = arr;
}

unsigned int SerializedDataHolder::policy(void) const
{
    return obj->policy;
}


} //closing namespace detail
} //closing namespace datastore
} //closing namespace pulsar

