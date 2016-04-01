/*! \file
 *
 * \brief Various test for datastore (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_TESTING__TESTSERIALIZATION_HPP_
#define BPMODULE_GUARD_TESTING__TESTSERIALIZATION_HPP_

#include <string>
#include "bpmodule/python/Convert.hpp"
#include "bpmodule/testing/TestingBase.hpp"
#include "bpmodule/util/HashSerializable.hpp"
#include "bpmodule/util/Serialize.hpp"

namespace bpmodule {
namespace testing {

/*! \brief Serialize an object, then unserialize
 *
 * \tparam T C++ type to use
 *
 * \warning Object must be registered to python
 */
template<typename T>
bool RoundTripSerialization(pybind11::object obj)
{
    using namespace bpmodule::util;

    T cppobj = bpmodule::python::ConvertToCpp<T>(obj);

    // Initial hash
    Hash hash1 = HashSerializable(cppobj);

    MemoryArchive mar;
    mar.BeginSerialization();
    mar.Serialize(cppobj);

    // Hash of serialized object
    Hash hash2 = mar.EndSerialization();

    mar.BeginUnserialization();
    T newobj(std::move(mar.UnserializeSingle<T>()));
    mar.EndUnserialization();

    // Hash after unserialization
    Hash hash3 = HashSerializable(newobj);

    /*
    output::GlobalDebug("Hash1: %s \n", hash1.String());
    output::GlobalDebug("Hash2: %s \n", hash2.String());
    output::GlobalDebug("Hash3: %s \n", hash3.String());
    */

    // All three hashes should match, and the two objects should
    // be equal
    return hash1 == hash2 &&
           hash2 == hash3 &&
           cppobj == newobj;

}



/*! \brief Test serialization and unserialization via round trip conversion
 *
 * \tparam T C++ type to use
 *
 * \param [in] obj Object to convert
 * \return False if the conversion fails, True if it succeeds
 */
template<typename T>
int TestSerialization(pybind11::object obj)
{
    return TestBoolFunc(RoundTripSerialization<T>, obj);
}


} // close namespace testing
} // close namespace bpmodule



#endif
