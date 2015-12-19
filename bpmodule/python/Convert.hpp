/*! \file
 *
 * \brief Conversion between python and C++ (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_CONVERT_HPP_
#define _GUARD_CONVERT_HPP_

#include "bpmodule/python/Pybind11.hpp"
#include "bpmodule/python/Pybind11_stl.hpp"
#include "bpmodule/python/Errors.hpp"
#include "bpmodule/exception/PythonConvertException.hpp"
#include "bpmodule/exception/Assert.hpp"
#include "bpmodule/util/Mangle.hpp"

namespace bpmodule {
namespace python {


/*! \brief Prevent some conversions
 */
template<typename T>
bool ValidConvert(pybind11::object obj)
{
    //! \todo Won't catch elements in lists, etc. Would have to override in pybind11?
    std::string cls = GetPyClass(obj);
    if(std::is_integral<T>::value && !std::is_same<bool,T>::value && cls != "int")
        return false;
    if(std::is_floating_point<T>::value && cls != "float")
        return false;
    if(std::is_same<T, bool>::value && cls != "bool")
        return false;

    return true;
}



/*! \brief Convert a python object to a C++ object
 *
 * \throw bpmodule::exception::PythonConvertException on error
 *
 * \tparam T The C++ type to convert to
 * \param [in] obj The python object to convert
 */
template<typename T>
T ConvertToCpp(pybind11::object obj)
{
    using bpmodule::exception::PythonConvertException;
    using bpmodule::exception::GeneralException;
    using bpmodule::exception::Assert;

    Assert<GeneralException>(obj.ptr() != nullptr, "Python object pointer is null");

    if(!ValidConvert<T>(obj))
        throw PythonConvertException("Cannot convert from python to C++: Incompatible types",
                                     GetPyClass(obj), util::DemangleCppType<T>());


    try {
        return obj.cast<T>();
    }
    catch(const std::exception & ex)
    {
        throw PythonConvertException("Cannot convert from python to C++: Conversion failed",
                                     GetPyClass(obj), util::DemangleCppType<T>(),
                                     "what", ex.what());
    }
    catch(...)
    {
        throw PythonConvertException("Cannot convert from python to C++: Conversion failed",
                                     GetPyClass(obj), util::DemangleCppType<T>(),
                                     "what", "unknown exception type");
    }
}



/*! \brief Convert a C++ object to a python object
 *
 * \throw bpmodule::exception::PythonConvertException on error
 *
 * \tparam T The C++ type to convert from
 * \param [in] obj The python object to convert
 */
template<typename T>
pybind11::object ConvertToPy(const T & obj,
                             pybind11::return_value_policy pol = pybind11::return_value_policy::copy)
{
    using bpmodule::exception::PythonConvertException;


    // may NOT throw if there is an issue
    try {
        pybind11::object pyobj = pybind11::cast(obj, pol);

        //! \todo fix if this pybind11 is changed to throw an exception
        if(!pyobj)
        {
            throw PythonConvertException(detail::GetPyException(),
                                         util::DemangleCppType<T>(), "python object",
                                         "info", "Resulting object pointer is null");
        }
        else
            return pyobj;
        
    }
    catch (const std::exception & ex)
    {
        throw PythonConvertException(ex.what(), 
                                     util::DemangleCppType<T>(), "python object",
                                     "info", "In converting from C++ to python object");
    }
    catch(...)
    {
        throw PythonConvertException("Caught unknown exception", 
                                     util::DemangleCppType<T>(), "python object",
                                     "info", "In converting from C++ to python object");
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
} // close namespace bpmodule

#endif
