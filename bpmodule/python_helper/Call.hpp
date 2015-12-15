/*! \file
 *
 * \brief Calling of python functions (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_CALL_HPP_
#define _GUARD_CALL_HPP_

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
Ret CallPyFunc2(pybind11::object & obj, Targs... Fargs)
{
    int nargs = static_cast<int>(sizeof...(Fargs));

    // don't check for nargs, since this may be a class method
    // and Fargs... doesn't include self
    if(!IsCallable2(obj))  
        throw exception::PythonCallException("Object is not callable!", "(none)",
                                             "nargs", nargs);

    pybind11::object ret;

    try {
        ret = obj.call(Fargs...);
    }
    catch(const pybind11::error_already_set & ex)
    {
        throw bpmodule::exception::PythonCallException("Error calling python function", "(none)",
                                                       "what", ex.what());
    }
    catch(...)
    {
        throw exception::GeneralException("TODO");
    }

    try {
        return ConvertToCpp2<Ret>(ret);
    }
    catch(const bpmodule::exception::PythonConvertException & ex)
    {
        // change to a PythonCallException
        throw bpmodule::exception::PythonCallException(ex, "(none)",
                                                       "desc", "Unable to convert return value from python function");
    }
}





/*! \brief Calls a function that is part of a python object
 *
 * Wraps exceptions in bmodule exceptions
 */
template<typename Ret, typename... Targs>
Ret CallPyFuncAttr2(pybind11::object obj, const char * fname, Targs... Fargs)
{

    int nargs = static_cast<int>(sizeof...(Fargs));

    if(!HasCallableAttr2(obj, fname))
        throw exception::PythonCallException("Object does not have callable attribute!", "(none)",
                                             "function", fname,
                                             "nargs", nargs);

    return CallPyFunc2<Ret>(obj.attr(fname), Fargs...); 
}




} // close namespace python_helper
} // close namespace bpmodule

#endif
