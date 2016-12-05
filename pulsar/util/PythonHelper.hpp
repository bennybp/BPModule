/*! \file
 *
 * \brief Miscellaneous helpers for python/pybind11 (header)
 */


#pragma once

#include "pulsar/util/Pybind11.hpp"

#include "pulsar/exception/PulsarException.hpp"
#include "pulsar/exception/Assert.hpp"
#include "pulsar/util/Mangle.hpp"


namespace pulsar {


/*! \brief Get the class of a python object as a string
 */
inline std::string get_py_class(const pybind11::object & obj)
{
    psr_assert(obj.ptr() != nullptr, "Python object pointer is null");

    pybind11::object cls = obj.attr("__class__");
    pybind11::object name = cls.attr("__name__");
    return name.cast<std::string>();
}


/*! \brief Check if a python object has a given attribute
 */
inline bool has_attr(const pybind11::object & obj, const char * attr)
{
    psr_assert(obj.ptr() != nullptr, "Python object pointer is null");
    return PyObject_HasAttrString(obj.ptr(), attr);
}


/*! \brief Check if a python object is callable
 */ 
inline bool is_callable(const pybind11::object & obj)
{
    psr_assert(obj.ptr() != nullptr, "Python object pointer is null");
    return PyCallable_Check(obj.ptr());
}


/*! \brief Check if a python object has a given callable attribute
 */
inline bool has_callable_attr(const pybind11::object & obj, const char * attr)
{
    psr_assert(obj.ptr() != nullptr, "Python object pointer is null");
    return has_attr(obj, attr) && is_callable(obj.attr(attr));
}



/*! \brief Convert a python object to a C++ object
 *
 * \throw pulsar::PulsarException on error
 *
 * \tparam T The C++ type to convert to
 * \param [in] obj The python object to convert
 */
template<typename T>
T convert_to_cpp(const pybind11::object & obj)
{
    psr_assert(obj.ptr() != nullptr, "Python object pointer is null");

    try {
        return obj.cast<T>();
    }
    catch(const std::exception & ex)
    {
        throw PulsarException("Cannot convert from python to C++: Conversion failed",
                                     "fromtype", get_py_class(obj), "totype", demangle_cpp_type<T>(),
                                     "what", ex.what());
    }
    catch(...)
    {
        throw PulsarException("Cannot convert from python to C++: Conversion failed",
                                     "fromtype", get_py_class(obj), "totype", demangle_cpp_type<T>(),
                                     "what", "unknown exception type");
    }
}



/*! \brief Convert a C++ object to a python object
 *
 * \throw pulsar::PulsarException on error
 *
 * \tparam T The C++ type to convert from
 * \param [in] obj The python object to convert
 * \param [in] pol The policy to use for the resulting cast
 */
template<typename T>
pybind11::object convert_to_py(const T & obj,
                               pybind11::return_value_policy pol = pybind11::return_value_policy::copy)
{
    // may NOT throw if there is an issue
    try {
        pybind11::object pyobj = pybind11::cast(obj, pol);

        //! \todo fix if this pybind11 is changed to throw an exception
        if(!pyobj)
        {
            throw PulsarException("Cannot convert from C++ to Python",
                                         "fromtype", demangle_cpp_type<T>(),
                                         "info", "Resulting object pointer is null");
        }
        else
            return pyobj;
        
    }
    catch (const std::exception & ex)
    {
        throw PulsarException(ex.what(), 
                                     "fromtype", demangle_cpp_type<T>(),
                                     "when", "in converting from C++ to python");
    }
    catch(...)
    {
        throw PulsarException("Caught unknown exception in converting from C++ to python object", 
                                     "from type", demangle_cpp_type<T>());
    }
}


/*! \brief Calls a python object
 *
 * \throw pulsar::PulsarException if there is an error (including conversion error)
 *
 * \param [in] obj The python object to call
 * \param [in] Fargs Arguments to call the function with
 */
template<typename Ret, typename... Targs>
Ret call_py_func(const pybind11::object & obj, Targs &&... Fargs)
{
    using pulsar::PulsarException;
    using pulsar::PulsarException;
    using pulsar::PulsarException;
    using pulsar::psr_assert;

    psr_assert(obj.ptr() != nullptr, "Python object pointer is null");

    int nargs = static_cast<int>(sizeof...(Fargs));

    // don't check for nargs, since this may be a class method
    // and Fargs... doesn't include self
    if(!is_callable(obj))  
        throw PulsarException("Object is not callable!", "nargs", nargs);

    pybind11::object ret;


    try {
        ret = obj(std::forward<Targs>(Fargs)...);
    }
    catch(const std::exception & ex) // may include pybind11::error_already_set
    {
        throw PulsarException(ex.what());
    }
    catch(...)
    {
        throw PulsarException("Caught unknown exception when calling a python function");
    }



    try {
        return convert_to_cpp<Ret>(ret);
    }
    catch(const std::exception & ex)
    {
        throw PulsarException(ex,
                                  "desc", "Unable to convert return value from python function");
    }
}





/*! \brief Calls a function that is an attribute a python object
 *
 * \throw pulsar::PulsarException if there is an error, including
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
    using pulsar::PulsarException;
    using pulsar::PulsarException;
    using pulsar::psr_assert;

    psr_assert(obj.ptr() != nullptr, "Python object pointer is null");

    int nargs = static_cast<int>(sizeof...(Fargs));

    if(!has_callable_attr(obj, attribute))
        throw PulsarException("Python object does not have callable attribute!",
                                  "function", attribute,
                                  "nargs", nargs);
    
    pybind11::object objattr = obj.attr(attribute);
    return call_py_func<Ret>(objattr, std::forward<Targs>(Fargs)...); 
}

} // close namespace pulsar

