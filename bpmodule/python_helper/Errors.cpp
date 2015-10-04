/*! \file
 *
 * \brief Handling of python errors (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bpmodule/python_helper/Errors.hpp"

namespace bpmodule {
namespace python_helper {
namespace detail {


//! \todo what is throwing exceptions?
std::string GetPyExceptionString(void)
{
    try {
        //! \todo leaking memory?
        //! \todo Get traceback info?
        if(PyErr_Occurred() != NULL)
        {
            PyObject *type, *value, *traceback;
            PyErr_Fetch(&type, &value, &traceback);

            /*
            boost::python::object type_obj(boost::python::handle<>(type));
            boost::python::object value_obj(boost::python::handle<>(value));
            boost::python::object traceback_obj(boost::python::handle<>(traceback));
            */
            std::string errstr = boost::python::extract<std::string>(value);

            /*
            errstr = boost::python::extract<std::string>(type);
            //errstr += " : ";
            errstr += boost::python::extract<std::string>(value);
            */

            return errstr;
        }
        else
            return "(no error?)";
    }
    catch(...)
    {
        throw;
        //return "(EXCEPTION THROWN IN GETTING EXCEPTION STRING)";
    }
}


} // close namespace detail
} // close namespace python_helper
} // close namespace bpmodule
