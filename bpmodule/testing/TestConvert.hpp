/*! \file
 *
 * \brief Various test for the core (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_TESTCONVERT_HPP_
#define _GUARD_TESTCONVERT_HPP_

#include <string>

#include "bpmodule/python_helper/Convert.hpp"
#include "bpmodule/testing/TestingBase.hpp"


namespace bpmodule {
namespace testing {



/*! \brief An class not exported to python
 *
 * Used to test failure of ConvertToPy
 */
template<typename T>
struct FailObject
{
    T t;
};



/*! \brief Testing of python-to-C++ conversions
 *
 * \tparam T C++ type to use
 *
 * \param [in] obj Object to convert
 * \return 1 if the conversion fails, 0 if it succeeds
 */
template<typename T>
int TestConvertToCpp(pybind11::object obj)
{
    return TestFunc(bpmodule::python_helper::ConvertToCpp<T>, obj);
}


/*! \brief Testing of C++-to-python conversions
 *
 * \tparam T C++ type to use
 *
 * \param [in] obj Object to convert
 * \return 1 if the conversion fails, 0 if it succeeds
 */
template<typename T>
int TestConvertToPy(const T & obj)
{
    return TestFunc(bpmodule::python_helper::ConvertToPy<T>, obj);
}


/*! \brief A single Python-to-C++-to-python round trip conversion
 *
 * \tparam T C++ type to use
 *
 * \param [in] obj Object to convert
 * \return 1 if the conversion fails, 0 if it succeeds
 */
template<typename T>
void PyCppPy(pybind11::object obj)
{
    T t = python_helper::ConvertToCpp<T>(obj);
    pybind11::object obj2 = python_helper::ConvertToPy<T>(t);
}



/*! \brief A single Python-to-C++-to-python round trip conversion that fails
 *
 * This should fail due to the object being passwd to ConvertToPy not
 * being exported to python
 *
 * \tparam T C++ type to use
 *
 * \param [in] obj Object to convert
 * \return 1 if the conversion fails, 0 if it succeeds
 */
template<typename T>
void PyCppPy_Fail(pybind11::object obj)
{
    T t = python_helper::ConvertToCpp<T>(obj);
    FailObject<T> fo{t};
    pybind11::object obj2 = python_helper::ConvertToPy<FailObject<T>>(fo);
}


/*! \brief Test Python-to-C++-to-python round trip conversion
 *
 * \tparam T C++ type to use
 *
 * \param [in] obj Object to convert
 * \return 1 if the conversion fails, 0 if it succeeds
 */
template<typename T>
int TestPyCppPy(pybind11::object obj)
{
    return TestFunc(PyCppPy<T>, obj);
}



/*! \brief Test Python-to-C++-to-python round trip conversion that fails
 *
 * \tparam T C++ type to use
 *
 * \param [in] obj Object to convert
 * \return 1 if the conversion fails, 0 if it succeeds
 */
template<typename T>
int TestPyCppPy_Fail(pybind11::object obj)
{
    return TestFunc(PyCppPy_Fail<T>, obj);
}


} // close namespace testing
} // close namespace bpmodule



#endif
