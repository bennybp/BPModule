#ifndef EXCEPTION_PYTHON_HPP
#define EXCEPTION_PYTHON_HPP

#include "BPModule/core/Exception.hpp"

#include <boost/python.hpp>

using namespace boost::python;

namespace bpmodule {
namespace export_python {

void TranslateException(const BPModuleException & ex);

std::string ExceptionString(const BPModuleException & ex);

} // close namespace export_python
} // close namespace bpmodule

#endif
