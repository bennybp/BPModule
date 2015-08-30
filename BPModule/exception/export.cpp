#include "BPModule/exception/Exception.hpp"

#include <boost/python.hpp>
using namespace boost::python;


namespace bpmodule {
namespace export_python {

BOOST_PYTHON_MODULE(bpmodule_exception)
{
    // set the translator for exceptions
    register_exception_translator<BPModuleException>(&TranslateException);
}


} // close namespace export_python
} // close namespace bpmodule

