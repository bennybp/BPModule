/*! \file
 *
 * \brief Python exports for testing the core library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/testing/TestConvert.hpp"
#include "bpmodule/testing/TestDatastore.hpp"
#include "bpmodule/testing/TestOptions.hpp"
#include "bpmodule/testing/TestingBase.hpp"
#include "bpmodule/testing/TestModuleInfo.hpp"

using namespace boost::python;


namespace bpmodule {
namespace testing {
namespace export_python {

BOOST_PYTHON_MODULE(testing)
{
    // From-python conversions
    def("TestConvertToCpp_sshort", TestConvertToCpp<signed short>);
    def("TestConvertToCpp_ushort", TestConvertToCpp<unsigned short>);
    def("TestConvertToCpp_sint", TestConvertToCpp<signed int>);
    def("TestConvertToCpp_uint", TestConvertToCpp<unsigned int>);
    def("TestConvertToCpp_slong", TestConvertToCpp<signed long>);
    def("TestConvertToCpp_ulong", TestConvertToCpp<unsigned long>);
    def("TestConvertToCpp_slonglong", TestConvertToCpp<signed long long>);
    def("TestConvertToCpp_ulonglong", TestConvertToCpp<unsigned long long>);
    def("TestConvertToCpp_float", TestConvertToCpp<float>);
    def("TestConvertToCpp_double", TestConvertToCpp<double>);
    def("TestConvertToCpp_string", TestConvertToCpp<std::string>);

    def("TestConvertToCpp_vector_sshort", TestConvertToCpp<std::vector<signed short>>);
    def("TestConvertToCpp_vector_ushort", TestConvertToCpp<std::vector<unsigned short>>);
    def("TestConvertToCpp_vector_sint", TestConvertToCpp<std::vector<signed int>>);
    def("TestConvertToCpp_vector_uint", TestConvertToCpp<std::vector<unsigned int>>);
    def("TestConvertToCpp_vector_slong", TestConvertToCpp<std::vector<signed long>>);
    def("TestConvertToCpp_vector_ulong", TestConvertToCpp<std::vector<unsigned long>>);
    def("TestConvertToCpp_vector_slonglong", TestConvertToCpp<std::vector<signed long long>>);
    def("TestConvertToCpp_vector_ulonglong", TestConvertToCpp<std::vector<unsigned long long>>);
    def("TestConvertToCpp_vector_float", TestConvertToCpp<std::vector<float>>);
    def("TestConvertToCpp_vector_double", TestConvertToCpp<std::vector<double>>);
    def("TestConvertToCpp_vector_string", TestConvertToCpp<std::vector<std::string>>);



    // ModuleInfo construction
    def("TestModuleInfo_Construct", TestModuleInfo_Construct);



    

    // Options
    def("Limits_sshort", Limits<signed short>);
    def("Limits_ushort", Limits<unsigned short>);
    def("Limits_sint", Limits<signed int>);
    def("Limits_uint", Limits<unsigned int>);
    def("Limits_slong", Limits<signed long>);
    def("Limits_ulong", Limits<unsigned long>);
    def("Limits_slonglong", Limits<signed long long>);
    def("Limits_ulonglong", Limits<unsigned long long>);

}


} // close namespace export_python
} // close namespace testing 
} // close namespace bpmodule

