/*! \file
 *
 * \brief Handling of python errors (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/exception/GeneralException.hpp"
#include "bpmodule/python/Errors.hpp"
#include "bpmodule/python/Types.hpp"

#include "bpmodule/python/Pybind11.hpp"



using bpmodule::exception::GeneralException;


namespace bpmodule {
namespace python {
namespace detail {


GeneralException GetPyException(void)
{
    try {
        //! \todo Get traceback info?
        //! \todo Only handles the built-in string exceptions and GeneralException. May need to do isinstance, blah blah
        if(PyErr_Occurred() != NULL)
        {
            PyObject *type, *value, *traceback;
            PyErr_Fetch(&type, &value, &traceback);

            pybind11::object type_obj(type, false); // false = take ownership
            pybind11::object value_obj(value, false);
            pybind11::object traceback_obj(traceback, false);

            // the type of the exception
            std::string extype = GetPyClass(value_obj);

            //! \todo runtime error

            if(extype == "str")  // type error, etc
                return value_obj.cast<std::string>();
            else if(extype == "GeneralException") // python version of GeneralException
                return static_cast<pybind11::object>(value_obj.attr("gex")).cast<GeneralException>();
            else
                return GeneralException("Unknown python exception type", "type", GetPyClass(value_obj));
        }
        else
            return GeneralException("(no error?)");
    }
    catch(...)
    {
        return GeneralException("DEVELOPER ERROR: EXCEPTION THROWN IN GETTING PYTHON EXCEPTION");
    }
}


} // close namespace detail
} // close namespace python
} // close namespace bpmodule
