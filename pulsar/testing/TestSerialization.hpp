/*! \file
 *
 * \brief Functions for testing serialization (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_TESTING__TESTSERIALIZATION_HPP_
#define PULSAR_GUARD_TESTING__TESTSERIALIZATION_HPP_

#include <string>
#include "pulsar/python/Convert.hpp"
#include "pulsar/testing/TestingBase.hpp"
#include "pulsar/util/Serialization.hpp"
#include "bphash/Hasher.hpp"

namespace pulsar{
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
    using namespace bphash;
    using namespace pulsar::util;

    T & cppobj = pulsar::python::ConvertToCpp<T &>(obj);

    // Initial hash
    HashValue hash1 = MakeHash(HashType::Hash128, cppobj);

    MemoryArchive mar;
    mar.BeginSerialization();
    mar.Serialize(cppobj);
    mar.EndSerialization();

    mar.BeginUnserialization();
    T newobj(std::move(mar.UnserializeSingle<T>()));
    mar.EndUnserialization();

    // Hash after unserialization
    HashValue hash2 = MakeHash(HashType::Hash128, newobj);

    // Round trip a byte array
    ByteArray ba = ToByteArray(cppobj);
    T newobj2 = FromByteArray<T>(ba);
    HashValue hash3 = MakeHash(HashType::Hash128, newobj2);

    // Round trip a byte array, but using pointers
    std::unique_ptr<T> newobj3 = NewFromByteArray<T>(ba);
    HashValue hash4 = MakeHash(HashType::Hash128, *newobj3);


    Equality eq;


    output::GlobalDebug("Hash1: %s \n", hash_to_string(hash1));
    output::GlobalDebug("Hash2: %s \n", hash_to_string(hash2));
    output::GlobalDebug("Hash3: %s \n", hash_to_string(hash3));
    output::GlobalDebug("Hash4: %s \n", hash_to_string(hash4));
    output::GlobalDebug("Hash Eq 1: %? \n", hash1 == hash2);
    output::GlobalDebug("Hash Eq 2: %? \n", hash2 == hash3);
    output::GlobalDebug("Hash Eq 3: %? \n", hash3 == hash4);
    output::GlobalDebug("Equality 1: %s \n",  eq(cppobj, newobj) ? "True" : "False");
    output::GlobalDebug("Equality 2: %s \n",  eq(cppobj, newobj2) ? "True" : "False");
    output::GlobalDebug("Equality 3: %s \n",  eq(cppobj, *newobj3) ? "True" : "False");


    // All hashes should match, and the two objects should
    // be equal
    return hash1 == hash2 &&
           hash2 == hash3 &&
           hash3 == hash4 &&
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
} // close namespace pulsar



#endif
