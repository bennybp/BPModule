/*! \file
 *
 * \brief Python exports for output and printing functions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <vector>
#include "pulsar/output/OutputStream.hpp"
#include "pulsar/output/GlobalOutput.hpp"
#include <pybind11/pybind11.h>


namespace pulsar{
namespace output {
namespace export_python {


void export_pybind11(pybind11::module & mtop)
{
    pybind11::module m = mtop.def_submodule("output", "Output functionality");

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
    .def("general_output", &OutputStream::output<>)
    .def("output",         &OutputStream::output<>)
    .def("changed",        &OutputStream::changed<>)
    .def("error",          &OutputStream::error<>)
    .def("warning",        &OutputStream::warning<>)
    .def("success",        &OutputStream::success<>)
    .def("debug",          &OutputStream::debug<>)
    .def("debug_enabled",  &OutputStream::debug_enabled)
    .def("enable_debug",   &OutputStream::enable_debug)
    ;
    

    m.def("color_enabled", color_enabled);
    m.def("enable_color", enable_color);

    m.def("get_global_output", get_global_output, pybind11::return_value_policy::reference);
    m.def("create_global_output", create_global_output);

    m.def("print_general_output",  print_general_output<>);
    m.def("print_global_output",  print_global_output<>);
    m.def("print_global_changed", print_global_changed<>);
    m.def("print_global_error",   print_global_error<>);
    m.def("print_global_warning", print_global_warning<>);
    m.def("print_global_success", print_global_success<>);
    m.def("print_global_debug",   print_global_debug<>);
}


} // close namespace export_python
} // close namespace output
} // close namespace pulsar

