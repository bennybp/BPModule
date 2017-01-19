/*! \file
 *
 * \brief Serialization helpers
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_UTIL__SERIALIZATION_HPP_
#define PULSAR_GUARD_UTIL__SERIALIZATION_HPP_

// Included via SerializationArchives.hpp
//#include <cereal/cereal.hpp>
//#include <cereal/access.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/polymorphic.hpp>

#include "pulsar/util/Serialization_fwd.hpp" // for ByteArray
#include "pulsar/util/SerializationArchives.hpp"
#include "pulsar/util/PythonHelper.hpp"

#define DECLARE_SERIALIZATION_FRIENDS \
    friend class cereal::access; \
    template<typename NoShadowT> friend class pulsar::detail::StdStreamArchive;


//! \todo There is an extra copy that in to_byte_array (ie, data -> sstream -> vector).
//        This could be fixed with a custom stream buffer that writes to a std::vector

namespace pybind11{

///Serializes a pybind11 object via the PyObject* pointer
template<typename Archive>
void save(Archive& ar, const object & pyob)
{
  if(!pulsar::is_pickleable(pyob))
        throw pulsar::PulsarException("Object is not pickleable");
  object pickle=module::import("pickle");
  object dumps=getattr(pickle,"dumps");
  object as_string=pulsar::call_py_func<object>(dumps,pyob,2);
  std::string real_string=as_string.cast<std::string>();
  ar(real_string);
}

///Deserializes a pybind11 object via the PyObject* pointer
template<typename Archive>
void load(Archive & ar,object & pyob)
{
    std::string real_string;
    {
        ar(real_string);
    }
    object pickle=module::import("pickle");
    object loads=getattr(pickle,"loads");
    bytes py_bytes(real_string);
    pyob=pulsar::call_py_func<object>(loads,py_bytes);
}

}//End namespace pybind11


namespace pulsar{

/*! \brief Convert a c++ object to a byte array
 *
 * The C++ object must be serializable
 */
template<typename T>
ByteArray to_byte_array(const T & obj)
{
    MemoryArchive mar;
    mar.begin_serialization();
    mar.serialize(obj);
    mar.end_serialization();
    return mar.to_byte_array(); 
}


/*! \brief Create c++ object from a byte array
 *
 * The C++ object must be serializable
 */
template<typename T>
T from_byte_array(const ByteArray & arr)
{
    MemoryArchive mar;
    mar.from_byte_array(arr);
    mar.begin_unserialization();
    T obj;
    mar.unserialize(obj);
    mar.end_unserialization();
    return obj; 
}



/*! \brief Create c++ object from a byte array
 *
 * The C++ object must be serializable
 */
template<typename T>
T from_byte_array(const char * bytes, size_t size)
{
    MemoryArchive mar;
    mar.from_byte_array(bytes, size);
    mar.begin_unserialization();
    T obj;
    mar.unserialize(obj);
    mar.end_unserialization();
    return obj; 
}


/*! \brief Create c++ object from a byte array
 *
 * The C++ object must be serializable
 */
template<typename T>
std::unique_ptr<T> new_from_byte_array(const ByteArray & arr)
{
    MemoryArchive mar;
    mar.from_byte_array(arr);
    mar.begin_unserialization();
    std::unique_ptr<T> objptr(new T);
    mar.unserialize(*objptr);
    mar.end_unserialization();
    return std::move(objptr);
}




template<typename T>
struct SerializeCheck 
   : public std::integral_constant<bool, cereal::traits::is_output_serializable<T, cereal::BinaryOutputArchive>::value &&
                                         cereal::traits::is_input_serializable<T, cereal::BinaryInputArchive>::value >
{ };


template<typename T>
struct SerializeCheck<std::shared_ptr<T>> : public SerializeCheck<T> { };

template<typename T, typename D>
struct SerializeCheck<std::unique_ptr<T, D>> : public SerializeCheck<T> { };

template<typename T>
struct SerializeCheck<std::vector<T>> : public SerializeCheck<T> { };

template<typename T>
struct SerializeCheck<std::set<T>> : public SerializeCheck<T> { };

template<typename T, typename V>
struct SerializeCheck<std::map<T, V>> : public std::integral_constant<bool, SerializeCheck<T>::value &&
                                                                             SerializeCheck<V>::value> { };

} // close namespace pulsar

#endif

