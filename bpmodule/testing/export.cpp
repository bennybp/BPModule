/*! \file
 *
 * \brief Python exports for testing the core library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/testing/AllTests.hpp"

using namespace boost::python;


namespace bpmodule {
namespace testing {
namespace export_python {

BOOST_PYTHON_MODULE(testing)
{
    // From-python conversions
    def("TestConvertToCpp_int", TestConvertToCpp<int>);
    def("TestConvertToCpp_long", TestConvertToCpp<long>);
    def("TestConvertToCpp_float", TestConvertToCpp<float>);
    def("TestConvertToCpp_double", TestConvertToCpp<double>);
    def("TestConvertToCpp_string", TestConvertToCpp<std::string>);

    def("TestConvertToCppVector_int", TestConvertToCppVector<int>);
    def("TestConvertToCppVector_long", TestConvertToCppVector<long>);
    def("TestConvertToCppVector_float", TestConvertToCppVector<float>);
    def("TestConvertToCppVector_double", TestConvertToCppVector<double>);
    def("TestConvertToCppVector_string", TestConvertToCppVector<std::string>);



    // ModuleInfo construction
    def("TestModuleInfo", TestModuleInfo);
}


} // close namespace export_python
} // close namespace testing 
} // close namespace bpmodule

