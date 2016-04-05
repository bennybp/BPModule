/*! \file
 *
 * \brief Functions for testing serialization (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_TESTING__TESTSERIALIZATION_HPP_
#define BPMODULE_GUARD_TESTING__TESTSERIALIZATION_HPP_

#include <string>
#include "bpmodule/python/Convert.hpp"
#include "bpmodule/testing/TestingBase.hpp"
#include "bpmodule/util/Serialization.hpp"
#include "bpmodule/util/HashSerializable.hpp"

namespace bpmodule {
namespace testing {

/*! \brief Serialize an object, then unserialize
 *
 * \tparam T C++ type to use
 *
 * \warning Object must be registered to python
 */
template<typename T, typename Equality = std::equal_to<T>>
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

    // Round trip a byte array
    ByteArray ba = ToByteArray(cppobj);
    T newobj2 = FromByteArray<T>(ba);
    Hash hash4 = HashSerializable(newobj2);

    // Round trip a byte array, but using pointers
    std::unique_ptr<T> newobj3 = NewFromByteArray<T>(ba);
    Hash hash5 = HashSerializable(*newobj3);


    Equality eq;

/*
    output::GlobalDebug("Hash1: %s \n", hash1.String());
    output::GlobalDebug("Hash2: %s \n", hash2.String());
    output::GlobalDebug("Hash3: %s \n", hash3.String());
    output::GlobalDebug("Hash4: %s \n", hash4.String());
    output::GlobalDebug("Hash5: %s \n", hash5.String());
    output::GlobalDebug("Hash Eq 1: %? \n", hash1 == hash2);
    output::GlobalDebug("Hash Eq 2: %? \n", hash2 == hash3);
    output::GlobalDebug("Hash Eq 3: %? \n", hash3 == hash4);
    output::GlobalDebug("Hash Eq 4: %? \n", hash4 == hash5);
    output::GlobalDebug("Eqality 1: %s \n",  eq(cppobj, newobj) ? "True" : "False");
    output::GlobalDebug("Eqality 2: %s \n",  eq(cppobj, newobj2) ? "True" : "False");
    output::GlobalDebug("Eqality 3: %s \n",  eq(cppobj, *newobj3) ? "True" : "False");
*/

    // All hashes should match, and the two objects should
    // be equal
    return hash1 == hash2 &&
           hash2 == hash3 &&
           hash3 == hash4 &&
           hash4 == hash5 &&
           eq(cppobj, newobj) &&
           eq(cppobj, newobj2) &&
           eq(cppobj, *newobj3);

}



/*! \brief Test serialization and unserialization via round trip conversion
 *
 * \tparam T C++ type to use
 *
 * \param [in] obj Object to convert
 * \return False if the conversion fails, True if it succeeds
 */
template<typename T, typename Equality = std::equal_to<T>>
int TestSerialization(pybind11::object obj)
{
    return TestBoolFunc(RoundTripSerialization<T, Equality>, obj);
}


} // close namespace testing
} // close namespace bpmodule



#endif
