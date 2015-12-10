/*! \file
 *
 * \brief Python exports for exceptions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/exception_translator.hpp>

#include "bpmodule/exception/GeneralException.hpp"

using namespace boost::python;
using namespace bpmodule::exception;

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

    class_<GeneralException>("GeneralException", init<const std::string &>())
    .def("AppendInfo", static_cast<void(GeneralException::*)(const std::string &, const std::string &)>(&GeneralException::AppendInfo))
    .def("ExceptionString", &GeneralException::ExceptionString)
    ;
}


} // close namespace export_python
} // close namespace exception
} // close namespace bpmodule

