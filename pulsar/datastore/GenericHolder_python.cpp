/*! \file
 *
 * \brief Storage of generic data (inner class) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "pulsar/datastore/GenericHolder_python.hpp"


namespace pulsar {
namespace datastore {
namespace detail {


GenericHolder<pybind11::object>::GenericHolder(const pybind11::object & m)
    : obj(std::make_shared<pybind11::object>(m))
{ }


/*! Return a const reference to the underlying data
 *
 * \return A const reference to the underlying data
 */ 
std::shared_ptr<const pybind11::object>
GenericHolder<pybind11::object>::get(void) const
{
    return obj;
}


////////////////////////////////////////
// Virtual functions from GenericBase
////////////////////////////////////////
const char * GenericHolder<pybind11::object>::type(void) const noexcept
{
    return typeid(pybind11::object).name();
}

std::string GenericHolder<pybind11::object>::demangled_type(void) const
{
    return python::get_py_class(*obj);
}

///////////////////////////////
// Serialization
///////////////////////////////

bool GenericHolder<pybind11::object>::is_serializable(void) const noexcept
{
    return false;
}

ByteArray GenericHolder<pybind11::object>::to_byte_array(void) const
{
    throw exception::GeneralException("to_byte_array called for unserializable cache data");
}

void GenericHolder<pybind11::object>::from_byte_array(const ByteArray & arr)
{
    throw exception::GeneralException("from_byte_array called for unserializable cache data");
}

///////////////////////////////
// Hashing
///////////////////////////////
bool GenericHolder<pybind11::object>::is_hashable(void) const noexcept
{
    return false;
}

bphash::HashValue GenericHolder<pybind11::object>::my_hash(void) const
{
    throw exception::GeneralException("hash called for unhashable cache data");
}


} //closing namespace detail
} //closing namespace datastore
} //closing namespace pulsar

