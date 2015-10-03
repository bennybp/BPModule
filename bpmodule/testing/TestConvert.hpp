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
 * \param [in] itest A test number
 * \param [in] desc Some description
 * \param [in] expected True if this is supoosed to succeed, false if it should
 *                      throw an exception
 * \param [in] obj Object to convert
 */
template<typename T>
int TestConvertToCpp(int itest, const std::string & desc, bool expected, const boost::python::object & obj)
{
    return TestFunc(itest, desc, expected, bpmodule::python_helper::ConvertToCpp<T>, obj);
}


/*! \brief Testing of C++-to-python conversions
 *
 * \tparam T C++ type to use
 *
 * \param [in] itest A test number
 * \param [in] desc Some description
 * \param [in] expected True if this is supoosed to succeed, false if it should
 *                      throw an exception
 * \param [in] obj Object to convert
 */
template<typename T>
int TestConvertToPy(int itest, const std::string & desc, bool expected, const T & obj)
{
    return TestFunc(itest, desc, expected, bpmodule::python_helper::ConvertToPy<T>, obj);
}


/*! \brief A single Python-to-C++-to-python round trip conversion
 *
 * \tparam T C++ type to use
 *
 * \param [in] obj Object to convert
 */
template<typename T>
void PyCppPy(const boost::python::object & obj)
{
    T t = python_helper::ConvertToCpp<T>(obj);
    boost::python::object obj2 = python_helper::ConvertToPy<T>(t);
}



/*! \brief A single Python-to-C++-to-python round trip conversion that fails
 *
 * This should fail due to the object being passwd to ConvertToPy not
 * being exported to python
 *
 * \tparam T C++ type to use
 *
 * \param [in] obj Object to convert
 */
template<typename T>
void PyCppPy_Fail(const boost::python::object & obj)
{
    T t = python_helper::ConvertToCpp<T>(obj);
    FailObject<T> fo{t};
    boost::python::object obj2 = python_helper::ConvertToPy<FailObject<T>>(fo);
}


/*! \brief Test Python-to-C++-to-python round trip conversion
 *
 * \tparam T C++ type to use
 *
 * \param [in] itest A test number
 * \param [in] desc Some description
 * \param [in] expected True if this is supoosed to succeed, false if it should
 *                      throw an exception
 * \param [in] obj Object to convert
 */
template<typename T>
int TestPyCppPy(int itest, const std::string & desc, bool expected, const boost::python::object & obj)
{
    return TestFunc(itest, desc, expected, PyCppPy<T>, obj);
}



/*! \brief Test Python-to-C++-to-python round trip conversion that fails
 *
 * \tparam T C++ type to use
 *
 * \param [in] itest A test number
 * \param [in] desc Some description
 * \param [in] expected True if this is supoosed to succeed, false if it should
 *                      throw an exception
 * \param [in] obj Object to convert
 */
template<typename T>
int TestPyCppPy_Fail(int itest, const std::string & desc, bool expected, const boost::python::object & obj)
{
    return TestFunc(itest, desc, expected, PyCppPy_Fail<T>, obj);
}


} // close namespace testing
} // close namespace bpmodule



#endif
