/*! \file
 *
 * \brief Calling of python functions (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_PYTHON__CALL_HPP_
#define PULSAR_GUARD_PYTHON__CALL_HPP_

#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/python/Convert.hpp"

namespace pulsar{
namespace python {


/*! \brief Calls a python object
 *
 * \throw pulsar::exception::PythonCallException if there is an error (including conversion error)
 *
 * \param [in] obj The python object to call
 * \param [in] Fargs Arguments to call the function with
 */
template<typename Ret, typename... Targs>
Ret CallPyFunc(const pybind11::object & obj, Targs &&... Fargs)
{
    using pulsar::exception::PythonCallException;
    using pulsar::exception::PythonConvertException;
    using pulsar::exception::GeneralException;
    using pulsar::exception::Assert;

    Assert<GeneralException>(obj.ptr() != nullptr, "Python object pointer is null");

    int nargs = static_cast<int>(sizeof...(Fargs));

    // don't check for nargs, since this may be a class method
    // and Fargs... doesn't include self
    if(!IsCallable(obj))  
        throw PythonCallException("Object is not callable!", "nargs", nargs);

    pybind11::object ret;


    try {
        ret = obj.call(std::forward<Targs>(Fargs)...);
    }
    catch(const std::exception &)
    {
        std::string what = detail::GetPyException();
        throw PythonCallException(what, "from", "within a python function");
    }
    catch(...)
    {
        throw PythonCallException("Caught unknown exception when calling a python function");
    }



    try {
        return ConvertToCpp<Ret>(ret);
    }
    catch(const std::exception & ex)
    {
        throw PythonCallException(ex,
                                  "desc", "Unable to convert return value from python function");
    }
}





/*! \brief Calls a function that is an attribute a python object
 *
 * \throw pulsar::exception::PythonCallException if there is an error, including
 *        if the object does not have the given attribute or if there is a conversion
 *        error
 *
 * \param [in] obj The object with the attribute
 * \param [in] attribute The attribute to call
 * \param [in] Fargs Arguments to call the function with
 */
template<typename Ret, typename... Targs>
Ret CallPyFuncAttr(const pybind11::object & obj, const char * attribute, Targs &&... Fargs)
{
    using pulsar::exception::PythonCallException;
    using pulsar::exception::GeneralException;
    using pulsar::exception::Assert;

    Assert<GeneralException>(obj.ptr() != nullptr, "Python object pointer is null");

    int nargs = static_cast<int>(sizeof...(Fargs));

    if(!HasCallableAttr(obj, attribute))
        throw PythonCallException("Python object does not have callable attribute!",
                                  "function", attribute,
                                  "nargs", nargs);
    
    pybind11::object objattr = obj.attr(attribute);
    return CallPyFunc<Ret>(objattr, std::forward<Targs>(Fargs)...); 
}




} // close namespace python
} // close namespace pulsar

#endif
