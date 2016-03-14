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
    .def("SetType",   &OutputStream::SetType)
    .def("ResetType", &OutputStream::ResetType)
    .def("Output_",   &OutputStream::Output<std::string>)
    .def("Changed_",  &OutputStream::Changed<std::string>)
    .def("Error_",    &OutputStream::Error<std::string>)
    .def("Warning_",  &OutputStream::Warning<std::string>)
    .def("Success_",  &OutputStream::Success<std::string>)
    .def("Debug_",    &OutputStream::Debug<std::string>)
    ;
    


    m.def("SetOut_Stdout", SetOut_Stdout);
    m.def("SetOut_Stderr", SetOut_Stderr);
    m.def("SetOut_File", SetOut_File);
    m.def("SetColor", SetColor);
    m.def("SetDebug", SetDebug);
    m.def("Valid", Valid);
    m.def("Flush", Flush);

    // printing to output
    // Python is expected to just give us a string, which
    // would correspond to the "format". Therefore, the
    // variadic template is empty (hence, <>)
    m.def("GlobalOutput",   output::GlobalOutput<>); 
    m.def("GlobalChanged",  output::GlobalChanged<>); 
    m.def("GlobalError",    output::GlobalError<>); 
    m.def("GlobalWarning",  output::GlobalWarning<>); 
    m.def("GlobalSuccess",  output::GlobalSuccess<>); 
    m.def("GlobalDebug",    output::GlobalDebug<>); 
    ;

    return m.ptr();
}


} // close namespace export_python
} // close namespace output
} // close namespace bpmodule

