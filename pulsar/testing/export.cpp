/*! \file
 *
 * \brief Python exports for testing the core library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include "pulsar/testing/TestConvert.hpp"
#include "pulsar/testing/TestOptions.hpp"
#include "pulsar/testing/TestingBase.hpp"
#include "pulsar/testing/Tester.hpp"


/*! \brief Returns the limits of a type as a python tuple
 */ 
template<typename T>
pybind11::tuple Limits(void)
{
    pybind11::tuple tup(2);
    tup[0] = pybind11::cast(std::numeric_limits<T>::lowest());
    tup[1] = pybind11::cast(std::numeric_limits<T>::max());
    return tup;
}


namespace pulsar{

void export_testing(pybind11::module & m)
{   
    using test1=void (Tester::*)(const std::string&,bool);
    using test2=void (Tester::*)(const std::string&,double,double,double);
    pybind11::class_<Tester>(m,"TesterBase")
    .def(pybind11::init<const std::string&>())
    .def("print_results",&Tester::print_results)
    .def("test_bool",static_cast<test1>(&Tester::test))
    .def("test_float",static_cast<test2>(&Tester::test),
         pybind11::arg("desc"),pybind11::arg("v1"),
         pybind11::arg("v2"),pybind11::arg("tol")=0.0001)
    ;
    
}

} // close namespace pulsar

