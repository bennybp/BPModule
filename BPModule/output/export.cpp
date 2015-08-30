#include "BPModule/output/Output.hpp"

#include <boost/python.hpp>

using namespace boost::python;
namespace bpy = boost::python;



namespace bpmodule {
namespace output {
namespace export_python {


////////////////////////////
// Main boost python part
////////////////////////////

BOOST_PYTHON_MODULE(bpmodule_output)
{
    def("SetOut_Stdout", SetOut_Stdout);
    def("SetOut_Stderr", SetOut_Stderr);
    def("SetOut_File", SetOut_File);
    def("SetColor", SetColor);
    def("SetDebug", SetDebug);
    def("Valid", Valid);
    def("Flush", Flush);

    // printing to output
    def("Output", Output_Wrap_Output);
    def("Success", Output_Wrap_Success);
    def("Warning", Output_Wrap_Warning);
    def("Error", Output_Wrap_Error);
    def("Changed", Output_Wrap_Changed);
    def("Debug", Output_Wrap_Debug);
}


} // close namespace export_python
} // close namespace output
} // close namespace bpmodule

