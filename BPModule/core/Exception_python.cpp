#include "BPModule/core/Exception.hpp"

#include <boost/python.hpp>

using namespace boost::python;

namespace bpmodule {
namespace export_python {

// the main exception translator
void TranslateException(const BPModuleException & ex)
{
    PyErr_SetString(PyExc_RuntimeError, ex.ExceptionString().c_str());
}



} // close namespace export_python
} // close namespace bpmodule

