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

    def("TestConvertToCpp_vector_int", TestConvertToCpp<std::vector<int>>);
    def("TestConvertToCpp_vector_long", TestConvertToCpp<std::vector<long>>);
    def("TestConvertToCpp_vector_float", TestConvertToCpp<std::vector<float>>);
    def("TestConvertToCpp_vector_double", TestConvertToCpp<std::vector<double>>);
    def("TestConvertToCpp_vector_string", TestConvertToCpp<std::vector<std::string>>);



    // ModuleInfo construction
    def("TestModuleInfo", TestModuleInfo);

    // OptionMap construction
    def("TestOptionMap", TestOptionMap);
}


} // close namespace export_python
} // close namespace testing 
} // close namespace bpmodule

