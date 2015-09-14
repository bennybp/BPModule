#ifndef _GUARD_CONVERT_HPP_
#define _GUARD_CONVERT_HPP_

#include <boost/python.hpp>

#include "bpmodule/exception/PythonConvertException.hpp"


using bpmodule::exception::PythonConvertException;



namespace bpmodule {
namespace python_helper {


/*! \brief Small helper to extract data and throw exceptions if needed
 *
 * \throw bpmodule::exception::PythonConvertException if the
 *        data could not be converted
 *
 * \tparam T The type to extract
 *
 * \param [in] obj The object to convert
 */
template<typename T>
T ConvertToCpp(const boost::python::object & obj)
{
    boost::python::extract<T> conv(obj);
    if(!conv.check())
    {
        std::string fromtype = boost::python::extract<std::string>(obj.attr("__class__").attr("__name__"));
        throw PythonConvertException("Cannot convert from python to C++", fromtype, typeid(T).name(), "Check failed"); 
    }

    try {
        return conv();
    }
    catch(...)
    {
        std::string fromtype = boost::python::extract<std::string>(obj.attr("__class__").attr("__name__"));
        throw PythonConvertException("Cannot convert from python to C++", fromtype, typeid(T).name(), "Conversion failed"); 
    }
}


template<typename T>
boost::python::object ConvertToPy(const T & obj)
{
    try {
       return boost::python::object(obj); 
    }
    catch(...)
    {
        throw PythonConvertException("Cannot convert from C++ to python", typeid(T).name(), "boost::python::object", "Conversion failed"); 
    }
}

} // close namespace python_helper
} // close namespace bpmodule

#endif
