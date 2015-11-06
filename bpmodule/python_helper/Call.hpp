/*! \file
 *
 * \brief Calling of python functions (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_CALL_HPP_
#define _GUARD_CALL_HPP_

#include <string>

#include "bpmodule/exception/PythonCallException.hpp"
#include "bpmodule/python_helper/Types.hpp"
#include "bpmodule/python_helper/Errors.hpp"
#include "bpmodule/python_helper/Convert.hpp"


namespace bpmodule {
namespace python_helper {


/*! \brief Calls a python object
 *
 * Wraps exceptions in bmodule exceptions
 */
template<typename Ret, typename... Targs>
Ret CallPyFunc(const boost::python::object & obj, Targs... Fargs)
{
    int nargs = static_cast<int>(sizeof...(Fargs));

    // don't check for nargs, since this may be a class method
    // and Fargs... doesn't include self
    if(!IsCallable(obj))  
        throw exception::PythonCallException("Object is not callable!", "(none)",
                                             "nargs", nargs);

    boost::python::object ret;

    try {
        ret = obj(Fargs...);
    }
    catch(...)
    {
        throw exception::PythonCallException("Unable to call python function",
                                             detail::GetPyExceptionString(),
                                             "nargs", nargs);
    }

    try {
        return ConvertToCpp<Ret>(ret);
    }
    catch(const bpmodule::exception::PythonConvertException & ex)
    {
        // change to a PythonCallException
        throw bpmodule::exception::PythonCallException(ex, "(none)",
                                                       "desc", "Unable to convert return value from python function");
    }
}


} // close namespace python_helper
} // close namespace bpmodule

#endif
