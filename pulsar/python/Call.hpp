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

/*! \brief Calls a python object
 *
 * \throw pulsar::PythonCallException if there is an error (including conversion error)
 *
 * \param [in] obj The python object to call
 * \param [in] Fargs Arguments to call the function with
 */
template<typename Ret, typename... Targs>
Ret call_py_func(const pybind11::object & obj, Targs &&... Fargs)
{
    using pulsar::PythonCallException;
    using pulsar::PythonConvertException;
    using pulsar::GeneralException;
    using pulsar::psr_assert;

    psr_assert<GeneralException>(obj.ptr() != nullptr, "Python object pointer is null");

    int nargs = static_cast<int>(sizeof...(Fargs));

    // don't check for nargs, since this may be a class method
    // and Fargs... doesn't include self
    if(!is_callable(obj))  
        throw PythonCallException("Object is not callable!", "nargs", nargs);

    pybind11::object ret;


    try {
        ret = obj(std::forward<Targs>(Fargs)...);
    }
    catch(const std::exception & ex) // may include pybind11::error_already_set
    {
        throw PythonCallException(ex.what());
    }
    catch(...)
    {
        throw PythonCallException("Caught unknown exception when calling a python function");
    }



    try {
        return convert_to_cpp<Ret>(ret);
    }
    catch(const std::exception & ex)
    {
        throw PythonCallException(ex,
                                  "desc", "Unable to convert return value from python function");
    }
}





/*! \brief Calls a function that is an attribute a python object
 *
 * \throw pulsar::PythonCallException if there is an error, including
 *        if the object does not have the given attribute or if there is a conversion
 *        error
 *
 * \param [in] obj The object with the attribute
 * \param [in] attribute The attribute to call
 * \param [in] Fargs Arguments to call the function with
 */
template<typename Ret, typename... Targs>
Ret call_py_func_attr(const pybind11::object & obj, const char * attribute, Targs &&... Fargs)
{
    using pulsar::PythonCallException;
    using pulsar::GeneralException;
    using pulsar::psr_assert;

    psr_assert<GeneralException>(obj.ptr() != nullptr, "Python object pointer is null");

    int nargs = static_cast<int>(sizeof...(Fargs));

    if(!has_callable_attr(obj, attribute))
        throw PythonCallException("Python object does not have callable attribute!",
                                  "function", attribute,
                                  "nargs", nargs);
    
    pybind11::object objattr = obj.attr(attribute);
    return call_py_func<Ret>(objattr, std::forward<Targs>(Fargs)...); 
}

} // close namespace pulsar

#endif
