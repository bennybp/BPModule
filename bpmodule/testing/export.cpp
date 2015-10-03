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
    // Arithmetic types
    def("TestPyCppPy_sshort",            TestPyCppPy<signed short>);
    def("TestPyCppPy_ushort",            TestPyCppPy<unsigned short>);
    def("TestPyCppPy_sint",              TestPyCppPy<signed int>);
    def("TestPyCppPy_uint",              TestPyCppPy<unsigned int>);
    def("TestPyCppPy_slong",             TestPyCppPy<signed long>);
    def("TestPyCppPy_ulong",             TestPyCppPy<unsigned long>);
    def("TestPyCppPy_slonglong",         TestPyCppPy<signed long long>);
    def("TestPyCppPy_ulonglong",         TestPyCppPy<unsigned long long>);

    def("TestPyCppPy_float",             TestPyCppPy<float>);
    def("TestPyCppPy_double",            TestPyCppPy<double>);
    def("TestPyCppPy_longdouble",        TestPyCppPy<long double>);

    def("TestPyCppPy_string",            TestPyCppPy<std::string>);

    // Vector types
    def("TestPyCppPy_vector_sshort",     TestPyCppPy<std::vector<signed short>>);
    def("TestPyCppPy_vector_ushort",     TestPyCppPy<std::vector<unsigned short>>);
    def("TestPyCppPy_vector_sint",       TestPyCppPy<std::vector<signed int>>);
    def("TestPyCppPy_vector_uint",       TestPyCppPy<std::vector<unsigned int>>);
    def("TestPyCppPy_vector_slong",      TestPyCppPy<std::vector<signed long>>);
    def("TestPyCppPy_vector_ulong",      TestPyCppPy<std::vector<unsigned long>>);
    def("TestPyCppPy_vector_slonglong",  TestPyCppPy<std::vector<signed long long>>);
    def("TestPyCppPy_vector_ulonglong",  TestPyCppPy<std::vector<unsigned long long>>);

    def("TestPyCppPy_vector_float",      TestPyCppPy<std::vector<float>>);
    def("TestPyCppPy_vector_double",     TestPyCppPy<std::vector<double>>);
    def("TestPyCppPy_vector_longdouble", TestPyCppPy<std::vector<long double>>);

    def("TestPyCppPy_vector_string",     TestPyCppPy<std::vector<std::string>>);



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

