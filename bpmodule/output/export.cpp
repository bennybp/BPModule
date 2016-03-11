/*! \file
 *
 * \brief Python exports for output and printing functions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <vector>
#include "bpmodule/output/Output.hpp"
#include "bpmodule/python/Pybind11.hpp"
#include "bpmodule/python/Pybind11_stl.hpp"


namespace bpmodule {
namespace output {
namespace export_python {


////////////////////////////
// Main boost python part
////////////////////////////

PYBIND11_PLUGIN(output)
{
    pybind11::module m("output", "Output functionality");

    // Enumeration for output types
    pybind11::enum_<OutputType>(m, "OutputType")
    .value("Output", OutputType::Output)
    .value("Changed", OutputType::Changed)
    .value("Error", OutputType::Error)
    .value("Warning", OutputType::Warning)
    .value("Success", OutputType::Success)
    .value("Debug", OutputType::Debug)
    ;


    pybind11::class_<OutputStream>(m, "OutputStream")
    .def("Output_",  &OutputStream::Output<std::vector<std::string>>)
    .def("Changed_", &OutputStream::Changed<std::vector<std::string>>)
    .def("Error_",   &OutputStream::Error<std::vector<std::string>>)
    .def("Warning_", &OutputStream::Warning<std::vector<std::string>>)
    .def("Success_", &OutputStream::Success<std::vector<std::string>>)
    .def("Debug_",   &OutputStream::Debug<std::vector<std::string>>)
    ;
    


    m.def("SetOut_Stdout", SetOut_Stdout);
    m.def("SetOut_Stderr", SetOut_Stderr);
    m.def("SetOut_File", SetOut_File);
    m.def("SetColor", SetColor);
    m.def("SetDebug", SetDebug);
    m.def("Valid", Valid);
    m.def("Flush", Flush);

    // printing to output
    m.def("GlobalOutput_",   output::GlobalOutput<std::vector<std::string>>); 
    m.def("GlobalChanged_",  output::GlobalChanged<std::vector<std::string>>); 
    m.def("GlobalError_",    output::GlobalError<std::vector<std::string>>); 
    m.def("GlobalWarning_",  output::GlobalWarning<std::vector<std::string>>); 
    m.def("GlobalSuccess_",  output::GlobalSuccess<std::vector<std::string>>); 
    m.def("GlobalDebug_",    output::GlobalDebug<std::vector<std::string>>); 
    ;

    return m.ptr();
}


} // close namespace export_python
} // close namespace output
} // close namespace bpmodule

