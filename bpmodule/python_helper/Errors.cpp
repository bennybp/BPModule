/*! \file
 *
 * \brief Handling of python errors (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bpmodule/python_helper/Errors.hpp"

namespace bpmodule {
namespace python_helper {
namespace detail {


std::string GetPyExceptionString(void)
{
    try {
        //! \todo leaking memory?
        if(PyErr_Occurred() != NULL)
        {
            PyObject *e, *v, *t;
            PyErr_Fetch(&e, &v, &t);

            boost::python::object e_obj(boost::python::handle<>(boost::python::allow_null(e)));
            boost::python::object v_obj(boost::python::handle<>(boost::python::allow_null(v)));
            boost::python::object t_obj(boost::python::handle<>(boost::python::allow_null(t)));

            std::string errstr = boost::python::extract<std::string>(e_obj);
            errstr += " : ";
            errstr += boost::python::extract<std::string>(v_obj);
            return errstr;
        }
        else
            return "(no error?)";
    }
    catch(...)
    {
        return "(EXCEPTION THROWN IN GETTING EXCEPTION STRING)";
    }
}


} // close namespace detail
} // close namespace python_helper
} // close namespace bpmodule
