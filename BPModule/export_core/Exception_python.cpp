#include "BPModule/core/Exception.hpp"
#include "BPModule/core/Output.hpp"

#include <boost/python.hpp>

using namespace boost::python;

namespace bpmodule {
namespace export_python {

std::string ExceptionString(const BPModuleException & ex)
{
    BPModuleException::ExceptionInfo exinfo = ex.GetInfo();
    std::stringstream ss;
    bpmodule::output::Output(ss, bpmodule::output::Line('*'));
    bpmodule::output::Output(ss, "Exception thrown!\n");
    bpmodule::output::Output(ss, "what() = %1%\n", ex.what());
    for(auto & it : exinfo)
        if(it.second.size())
            bpmodule::output::Output(ss, "%|24| : %|-|\n", it.first, it.second);
    return ss.str();
}

// the main exception translator
void TranslateException(const BPModuleException & ex)
{
    PyErr_SetString(PyExc_RuntimeError, ExceptionString(ex).c_str());
}



} // close namespace export_python
} // close namespace bpmodule

