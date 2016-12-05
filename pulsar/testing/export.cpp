/*! \file
 *
 * \brief Python exports for testing the core library
 */ 

#include "pulsar/testing/TesterBase.hpp"
#include "pulsar/util/Pybind11.hpp"


namespace pulsar {

void export_testing(pybind11::module & m)
{   
    pybind11::class_<TesterBase>(m,"TesterBase")
    .def(pybind11::init<const std::string&>())
    .def("print_results",&TesterBase::print_results)
    .def("test",&TesterBase::test)
    .def("nfailed",&TesterBase::nfailed)
    .def("ntest",&TesterBase::ntest)
    ;
    
}

} // close namespace pulsar

