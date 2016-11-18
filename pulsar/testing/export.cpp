/*! \file
 *
 * \brief Python exports for testing the core library
 */ 

#include "pulsar/testing/TesterBase.hpp"
#include <pybind11/pybind11.h>


namespace pulsar {

void export_testing(pybind11::module & m)
{   
    using test1 = void (TesterBase::*)(const std::string&,bool);

    pybind11::class_<TesterBase>(m,"TesterBase")
    .def(pybind11::init<const std::string&>())
    .def("print_results",&TesterBase::print_results)
    .def("test",static_cast<test1>(&TesterBase::test))
    .def("nfailed",&TesterBase::nfailed)
    .def("ntest",&TesterBase::ntest)
    ;
    
}

} // close namespace pulsar

