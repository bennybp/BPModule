/*! \file
 *
 * \brief Python exports for output and printing functions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <vector>
#include "bpmodule/output/OutputStream.hpp"
#include "bpmodule/output/GlobalOutput.hpp"
#include "bpmodule/python/Pybind11.hpp"


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


    // Outputstream type
    pybind11::class_<std::ostream> os(m, "OSTREAM_INTERNAL");

    pybind11::class_<OutputStream>(m, "OutputStream", os)
    .def("GeneralOutput",  &OutputStream::Output<>)
    .def("Output",         &OutputStream::Output<>)
    .def("Changed",        &OutputStream::Changed<>)
    .def("Error",          &OutputStream::Error<>)
    .def("Warning",        &OutputStream::Warning<>)
    .def("Success",        &OutputStream::Success<>)
    .def("Debug",          &OutputStream::Debug<>)
    .def("DebugEnabled",   &OutputStream::DebugEnabled)
    .def("EnableDebug",    &OutputStream::EnableDebug)
    ;
    

    m.def("ColorEnabled", ColorEnabled);
    m.def("EnableColor", EnableColor);

    m.def("GetGlobalOut", GetGlobalOut, pybind11::return_value_policy::reference);
    m.def("SetGlobalOut_Stdout", SetGlobalOut_Stdout);
    //m.def("SetOut_File", SetOut_File);

    m.def("GlobalGeneralOutput",  GlobalGeneralOutput<>);
    m.def("GlobalOutput",  GlobalOutput<>);
    m.def("GlobalChanged", GlobalChanged<>);
    m.def("GlobalError",   GlobalError<>);
    m.def("GlobalWarning", GlobalWarning<>);
    m.def("GlobalSuccess", GlobalSuccess<>);
    m.def("GlobalDebug",   GlobalDebug<>);

    return m.ptr();
}


} // close namespace export_python
} // close namespace output
} // close namespace bpmodule

