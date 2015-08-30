#include "BPModule/exception/GeneralException.hpp"

#include <boost/python.hpp>
using namespace boost::python;


namespace bpmodule {
namespace exception {
namespace export_python {

BOOST_PYTHON_MODULE(bpmodule_exception)
{
    // set the translator for exceptions
    register_exception_translator<GeneralException>(&TranslateException);
}


} // close namespace export_python
} // close namespace exception
} // close namespace bpmodule

