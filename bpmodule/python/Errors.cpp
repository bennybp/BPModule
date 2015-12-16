/*! \file
 *
 * \brief Handling of python errors (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/pragma.h"

#include "bpmodule/exception/GeneralException.hpp"
#include "bpmodule/python/Errors.hpp"
#include "bpmodule/python/Types.hpp"


using bpmodule::exception::GeneralException;


namespace bpmodule {
namespace python {
namespace detail {


/*
GeneralException GetPyException(void)
{
    try {
        //! \todo leaking memory?
        //! \todo Get traceback info?
        //! \todo Only handles the built-in string exceptions and GeneralException. May need to do isinstance, blah blah
        if(PyErr_Occurred() != NULL)
        {
            PyObject *type, *value, *traceback;
            PyErr_Fetch(&type, &value, &traceback);

            // careful of most vexing parse
            boost::python::object type_obj = boost::python::object(boost::python::handle<>(type));
            boost::python::object value_obj = boost::python::object(boost::python::handle<>(value));

            //boost::python::object traceback_obj(boost::python::handle<>(traceback));
            //std::string errstr = GetPyClass(value_obj);
            std::string extype = GetPyClass(value_obj);

            if(extype == "str")
                return GeneralException(boost::python::extract<std::string>(value_obj));
            else if(extype == "GeneralException") // python version of GeneralException
            {
                return boost::python::extract<GeneralException>(value_obj.attr("gex"));
            }
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
*/

} // close namespace detail
} // close namespace python
} // close namespace bpmodule
