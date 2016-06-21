/*! \file
 *
 * \brief Handling of python errors (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/python/Errors.hpp"
#include "pulsar/python/Types.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include <pybind11/pybind11.h>

using pulsar::exception::GeneralException;


namespace pulsar{
namespace python {
namespace detail {


std::string get_py_exception(void)
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
            std::string extype = get_py_class(value_obj);

            //! \todo runtime error
            //! \todo check for stuff deriving from std::exception

            if(extype == "str")  // type error, etc
                return value_obj.cast<std::string>();
            else if(extype == "GeneralException") // python version of GeneralException
                return static_cast<pybind11::object>(value_obj.attr("gex")).cast<GeneralException>().what();
            else
                return std::string("Unknown python exception type: ") + get_py_class(value_obj);
        }
        else
            return "(no python error)";
    }
    catch(...)
    {
        return "DEVELOPER ERROR: EXCEPTION THROWN IN GETTING PYTHON EXCEPTION";
    }
}


} // close namespace detail
} // close namespace python
} // close namespace pulsar
