/*! \file
 *
 * \brief Conversion between python and C++ (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_PYTHON__CONVERT_HPP_
#define PULSAR_GUARD_PYTHON__CONVERT_HPP_

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "pulsar/python/Errors.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/exception/Assert.hpp"
#include "pulsar/util/Mangle.hpp"

namespace pulsar{
namespace python {



/*! \brief Convert a python object to a C++ object
 *
 * \throw pulsar::PythonConvertException on error
 *
 * \tparam T The C++ type to convert to
 * \param [in] obj The python object to convert
 */
template<typename T>
T convert_to_cpp(const pybind11::object & obj)
{
    using pulsar::PythonConvertException;
    using pulsar::GeneralException;
    using pulsar::psr_assert;

    psr_assert<GeneralException>(obj.ptr() != nullptr, "Python object pointer is null");

    try {
        return obj.cast<T>();
    }
    catch(const std::exception & ex)
    {
        throw PythonConvertException("Cannot convert from python to C++: Conversion failed",
                                     "fromtype", get_py_class(obj), "totype", util::demangle_cpp_type<T>(),
                                     "what", ex.what());
    }
    catch(...)
    {
        throw PythonConvertException("Cannot convert from python to C++: Conversion failed",
                                     "fromtype", get_py_class(obj), "totype", util::demangle_cpp_type<T>(),
                                     "what", "unknown exception type");
    }
}



/*! \brief Convert a C++ object to a python object
 *
 * \throw pulsar::PythonConvertException on error
 *
 * \tparam T The C++ type to convert from
 * \param [in] obj The python object to convert
 * \param [in] pol The policy to use for the resulting cast
 */
template<typename T>
pybind11::object convert_to_py(const T & obj,
                             pybind11::return_value_policy pol = pybind11::return_value_policy::copy)
{
    using pulsar::PythonConvertException;


    // may NOT throw if there is an issue
    try {
        pybind11::object pyobj = pybind11::cast(obj, pol);

        //! \todo fix if this pybind11 is changed to throw an exception
        if(!pyobj)
        {
            auto pyex = detail::get_py_exception();

            throw PythonConvertException(pyex.first,
                                         "when", "in converting from C++ to python",
                                         "fromtype", util::demangle_cpp_type<T>(),
                                         "info", "Resulting object pointer is null");
        }
        else
            return pyobj;
        
    }
    catch (const std::exception & ex)
    {
        throw PythonConvertException(ex.what(), 
                                     "fromtype", util::demangle_cpp_type<T>(),
                                     "when", "in converting from C++ to python");
    }
    catch(...)
    {
        throw PythonConvertException("Caught unknown exception in converting from C++ to python object", 
                                     "from type", util::demangle_cpp_type<T>());
    }
}



/*! \brief Convert a plain array of C++ objects to a python object
 *
 * \throw PythonConvertException if the
 *        data could not be converted
 *
 * \tparam T The C++ type to convert from
 *
 * \param [in] obj An array of objects
 * \param [in] n Length of the array
 * \return Converted data as a python object
 */
template<typename T>
pybind11::object convert_to_py(const T * const obj, size_t n)
{
    std::vector<T> v(obj, obj+n);

    return convert_to_py(v);
}



} // close namespace python
} // close namespace pulsar

#endif
