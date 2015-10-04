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


namespace bpmodule {
namespace python_helper {


/*! \brief Calls a python object
 *
 * Wraps exceptions in bmodule exceptions
 */
template<typename... Targs>
boost::python::object CallPyFunc(const boost::python::object & obj, Targs... Fargs)
{
    int nargs = static_cast<int>(sizeof...(Fargs));

    // don't check for nargs, since this may be a class method
    // and Fargs... doesn't include self
    if(!IsCallable(obj))  
        throw exception::PythonCallException("Object is not callable!", "(none)",
                                             "nargs", std::to_string(nargs));

    try {
        return obj(Fargs...);
    }
    catch(...)
    {
        throw exception::PythonCallException("Unable to call python function",
                                             detail::GetPyExceptionString(),
                                             "nargs", std::to_string(nargs));
    }
}


} // close namespace python_helper
} // close namespace bpmodule

#endif
