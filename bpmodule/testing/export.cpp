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


/*! \brief Returns the limits of a type as a python tuple
 */ 
template<typename T>
boost::python::tuple Limits(void)
{ 
    return boost::python::make_tuple( 
                                      std::numeric_limits<T>::lowest(),
                                      std::numeric_limits<T>::max()
                                    );
}


namespace bpmodule {
namespace testing {
namespace export_python {

BOOST_PYTHON_MODULE(testing)
{
    // Limits for various types
    def("Limits_sshort", Limits<signed short>);
    def("Limits_ushort", Limits<unsigned short>);
    def("Limits_sint", Limits<signed int>);
    def("Limits_uint", Limits<unsigned int>);
    def("Limits_slong", Limits<signed long>);
    def("Limits_ulong", Limits<unsigned long>);
    def("Limits_slonglong", Limits<signed long long>);
    def("Limits_ulonglong", Limits<unsigned long long>);


    /////////////////////////////////
    // Py-Cpp-Py round trip tests
    // Arithmetic types
    /////////////////////////////////
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



    // Py-Cpp-Py round trip tests that should
    // fail on the cpp-to-py step
    // Arithmetic types
    def("TestPyCppPy_Fail_sshort",            TestPyCppPy_Fail<signed short>);
    def("TestPyCppPy_Fail_ushort",            TestPyCppPy_Fail<unsigned short>);
    def("TestPyCppPy_Fail_sint",              TestPyCppPy_Fail<signed int>);
    def("TestPyCppPy_Fail_uint",              TestPyCppPy_Fail<unsigned int>);
    def("TestPyCppPy_Fail_slong",             TestPyCppPy_Fail<signed long>);
    def("TestPyCppPy_Fail_ulong",             TestPyCppPy_Fail<unsigned long>);
    def("TestPyCppPy_Fail_slonglong",         TestPyCppPy_Fail<signed long long>);
    def("TestPyCppPy_Fail_ulonglong",         TestPyCppPy_Fail<unsigned long long>);
    def("TestPyCppPy_Fail_float",             TestPyCppPy_Fail<float>);
    def("TestPyCppPy_Fail_double",            TestPyCppPy_Fail<double>);
    def("TestPyCppPy_Fail_longdouble",        TestPyCppPy_Fail<long double>);
    def("TestPyCppPy_Fail_string",            TestPyCppPy_Fail<std::string>);

    // Vector types
    def("TestPyCppPy_Fail_vector_sshort",     TestPyCppPy_Fail<std::vector<signed short>>);
    def("TestPyCppPy_Fail_vector_ushort",     TestPyCppPy_Fail<std::vector<unsigned short>>);
    def("TestPyCppPy_Fail_vector_sint",       TestPyCppPy_Fail<std::vector<signed int>>);
    def("TestPyCppPy_Fail_vector_uint",       TestPyCppPy_Fail<std::vector<unsigned int>>);
    def("TestPyCppPy_Fail_vector_slong",      TestPyCppPy_Fail<std::vector<signed long>>);
    def("TestPyCppPy_Fail_vector_ulong",      TestPyCppPy_Fail<std::vector<unsigned long>>);
    def("TestPyCppPy_Fail_vector_slonglong",  TestPyCppPy_Fail<std::vector<signed long long>>);
    def("TestPyCppPy_Fail_vector_ulonglong",  TestPyCppPy_Fail<std::vector<unsigned long long>>);
    def("TestPyCppPy_Fail_vector_float",      TestPyCppPy_Fail<std::vector<float>>);
    def("TestPyCppPy_Fail_vector_double",     TestPyCppPy_Fail<std::vector<double>>);
    def("TestPyCppPy_Fail_vector_longdouble", TestPyCppPy_Fail<std::vector<long double>>);
    def("TestPyCppPy_Fail_vector_string",     TestPyCppPy_Fail<std::vector<std::string>>);



    ////////////////
    // Options
    ////////////////
    // Getting an option from a map
    def("TestOptionMapGet_bool",              &TestOptionMap_Get<bool>);
    def("TestOptionMapGet_sshort",            &TestOptionMap_Get<signed short>);
    def("TestOptionMapGet_ushort",            &TestOptionMap_Get<unsigned short>);
    def("TestOptionMapGet_sint",              &TestOptionMap_Get<signed int>);
    def("TestOptionMapGet_uint",              &TestOptionMap_Get<unsigned int>);
    def("TestOptionMapGet_slong",             &TestOptionMap_Get<signed long>);
    def("TestOptionMapGet_ulong",             &TestOptionMap_Get<unsigned long>);
    def("TestOptionMapGet_slonglong",         &TestOptionMap_Get<signed long long>);
    def("TestOptionMapGet_ulonglong",         &TestOptionMap_Get<unsigned long long>);
    def("TestOptionMapGet_float",             &TestOptionMap_Get<float>);
    def("TestOptionMapGet_double",            &TestOptionMap_Get<double>);
    def("TestOptionMapGet_longdouble",        &TestOptionMap_Get<long double>);
    def("TestOptionMapGet_string",            &TestOptionMap_Get<std::string>);

    def("TestOptionMapGet_vector_bool",       &TestOptionMap_Get<std::vector<bool>>);
    def("TestOptionMapGet_vector_sshort",     &TestOptionMap_Get<std::vector<signed short>>);
    def("TestOptionMapGet_vector_ushort",     &TestOptionMap_Get<std::vector<unsigned short>>);
    def("TestOptionMapGet_vector_sint",       &TestOptionMap_Get<std::vector<signed int>>);
    def("TestOptionMapGet_vector_uint",       &TestOptionMap_Get<std::vector<unsigned int>>);
    def("TestOptionMapGet_vector_slong",      &TestOptionMap_Get<std::vector<signed long>>);
    def("TestOptionMapGet_vector_ulong",      &TestOptionMap_Get<std::vector<unsigned long>>);
    def("TestOptionMapGet_vector_slonglong",  &TestOptionMap_Get<std::vector<signed long long>>);
    def("TestOptionMapGet_vector_ulonglong",  &TestOptionMap_Get<std::vector<unsigned long long>>);
    def("TestOptionMapGet_vector_float",      &TestOptionMap_Get<std::vector<float>>);
    def("TestOptionMapGet_vector_double",     &TestOptionMap_Get<std::vector<double>>);
    def("TestOptionMapGet_vector_longdouble", &TestOptionMap_Get<std::vector<long double>>);
    def("TestOptionMapGet_vector_string",     &TestOptionMap_Get<std::vector<std::string>>);

    // Changing an option in the map
    def("TestOptionMapChange_bool",              &TestOptionMap_Change<bool>);
    def("TestOptionMapChange_sshort",            &TestOptionMap_Change<signed short>);
    def("TestOptionMapChange_ushort",            &TestOptionMap_Change<unsigned short>);
    def("TestOptionMapChange_sint",              &TestOptionMap_Change<signed int>);
    def("TestOptionMapChange_uint",              &TestOptionMap_Change<unsigned int>);
    def("TestOptionMapChange_slong",             &TestOptionMap_Change<signed long>);
    def("TestOptionMapChange_ulong",             &TestOptionMap_Change<unsigned long>);
    def("TestOptionMapChange_slonglong",         &TestOptionMap_Change<signed long long>);
    def("TestOptionMapChange_ulonglong",         &TestOptionMap_Change<unsigned long long>);
    def("TestOptionMapChange_float",             &TestOptionMap_Change<float>);
    def("TestOptionMapChange_double",            &TestOptionMap_Change<double>);
    def("TestOptionMapChange_longdouble",        &TestOptionMap_Change<long double>);
    def("TestOptionMapChange_string",            &TestOptionMap_Change<std::string>);

    def("TestOptionMapChange_vector_bool",       &TestOptionMap_Change<std::vector<bool>>);
    def("TestOptionMapChange_vector_sshort",     &TestOptionMap_Change<std::vector<signed short>>);
    def("TestOptionMapChange_vector_ushort",     &TestOptionMap_Change<std::vector<unsigned short>>);
    def("TestOptionMapChange_vector_sint",       &TestOptionMap_Change<std::vector<signed int>>);
    def("TestOptionMapChange_vector_uint",       &TestOptionMap_Change<std::vector<unsigned int>>);
    def("TestOptionMapChange_vector_slong",      &TestOptionMap_Change<std::vector<signed long>>);
    def("TestOptionMapChange_vector_ulong",      &TestOptionMap_Change<std::vector<unsigned long>>);
    def("TestOptionMapChange_vector_slonglong",  &TestOptionMap_Change<std::vector<signed long long>>);
    def("TestOptionMapChange_vector_ulonglong",  &TestOptionMap_Change<std::vector<unsigned long long>>);
    def("TestOptionMapChange_vector_float",      &TestOptionMap_Change<std::vector<float>>);
    def("TestOptionMapChange_vector_double",     &TestOptionMap_Change<std::vector<double>>);
    def("TestOptionMapChange_vector_longdouble", &TestOptionMap_Change<std::vector<long double>>);
    def("TestOptionMapChange_vector_string",     &TestOptionMap_Change<std::vector<std::string>>);




    ////////////////////////////
    // ModuleInfo construction
    ////////////////////////////
    def("TestModuleInfo_Construct", TestModuleInfo_Construct);
}


} // close namespace export_python
} // close namespace testing 
} // close namespace bpmodule

