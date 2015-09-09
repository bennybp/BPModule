/*! \file
 *
 * \brief Python exports for exceptions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/exception/GeneralException.hpp"

#include <boost/python.hpp>
using namespace boost::python;


namespace bpmodule {
namespace exception {
namespace export_python {

// the main exception translator
void TranslateException(const GeneralException & ex)
{
    PyErr_SetString(PyExc_RuntimeError, ex.ExceptionString().c_str());
}



BOOST_PYTHON_MODULE(exception)
{
    // set the translator for exceptions
    register_exception_translator<GeneralException>(&TranslateException);
}


} // close namespace export_python
} // close namespace exception
} // close namespace bpmodule

