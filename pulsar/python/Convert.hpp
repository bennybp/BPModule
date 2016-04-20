/*! \file
 *
 * \brief Conversion between python and C++ (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_PYTHON__CONVERT_HPP_
#define PULSAR_GUARD_PYTHON__CONVERT_HPP_

#include "pulsar/python/Pybind11.hpp"
#include "pulsar/python/Pybind11_stl.hpp"
#include "pulsar/python/Errors.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/exception/Assert.hpp"
#include "pulsar/util/Mangle.hpp"

namespace pulsar{
namespace python {



/*! \brief Convert a python object to a C++ object
 *
 * \throw pulsar::exception::PythonConvertException on error
 *
 * \tparam T The C++ type to convert to
 * \param [in] obj The python object to convert
 */
template<typename T>
T ConvertToCpp(const pybind11::object & obj)
{
    using pulsar::exception::PythonConvertException;
    using pulsar::exception::GeneralException;
    using pulsar::exception::Assert;

    Assert<GeneralException>(obj.ptr() != nullptr, "Python object pointer is null");

    try {
        return obj.cast<T>();
    }
    catch(const std::exception & ex)
    {
        throw PythonConvertException("Cannot convert from python to C++: Conversion failed",
                                     "fromtype", GetPyClass(obj), "totype", util::DemangleCppType<T>(),
                                     "what", ex.what());
    }
    catch(...)
    {
        throw PythonConvertException("Cannot convert from python to C++: Conversion failed",
                                     "fromtype", GetPyClass(obj), "totype", util::DemangleCppType<T>(),
                                     "what", "unknown exception type");
    }
}



/*! \brief Convert a C++ object to a python object
 *
 * \throw pulsar::exception::PythonConvertException on error
 *
 * \tparam T The C++ type to convert from
 * \param [in] obj The python object to convert
 * \param [in] pol The policy to use for the resulting cast
 */
template<typename T>
pybind11::object ConvertToPy(const T & obj,
                             pybind11::return_value_policy pol = pybind11::return_value_policy::copy)
{
    using pulsar::exception::PythonConvertException;


    // may NOT throw if there is an issue
    try {
        pybind11::object pyobj = pybind11::cast(obj, pol);

        //! \todo fix if this pybind11 is changed to throw an exception
        if(!pyobj)
        {
            throw PythonConvertException(detail::GetPyException(),
                                         "when", "in converting from C++ to python",
                                         "fromtype", util::DemangleCppType<T>(),
                                         "info", "Resulting object pointer is null");
        }
        else
            return pyobj;
        
    }
    catch (const std::exception & ex)
    {
        throw PythonConvertException(ex.what(), 
                                     "fromtype", util::DemangleCppType<T>(),
                                     "when", "in converting from C++ to python");
    }
    catch(...)
    {
        throw PythonConvertException("Caught unknown exception in converting from C++ to python object", 
                                     "from type", util::DemangleCppType<T>());
    }
}



/*! \brief Convert a plain array of C++ objects to a python object
 *
 * \throw exception::PythonConvertException if the
 *        data could not be converted
 *
 * \tparam T The C++ type to convert from
 *
 * \param [in] obj An array of objects
 * \param [in] n Length of the array
 * \return Converted data as a python object
 */
template<typename T>
pybind11::object ConvertToPy(const T * const obj, size_t n)
{
    std::vector<T> v(obj, obj+n);

    return ConvertToPy(v);
}



} // close namespace python
} // close namespace pulsar

#endif
