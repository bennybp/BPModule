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
namespace testing {
namespace export_python {

void export_pybind11(pybind11::module & mtop)
{
    pybind11::module m = mtop.def_submodule("testing", "Some helpers for testing"); 
    
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
    
    
    
    // Limits for various types
    m.def("Limits_sshort", Limits<signed short>);
    m.def("Limits_ushort", Limits<unsigned short>);
    m.def("Limits_sint", Limits<signed int>);
    m.def("Limits_uint", Limits<unsigned int>);
    m.def("Limits_slong", Limits<signed long>);
    m.def("Limits_ulong", Limits<unsigned long>);
    m.def("Limits_slonglong", Limits<signed long long>);
    m.def("Limits_ulonglong", Limits<unsigned long long>);


    /////////////////////////////////
    // Py-Cpp-Py round trip tests
    // Arithmetic types
    /////////////////////////////////
    m.def("TestPyCppPy_sshort",            TestPyCppPy<signed short>);
    m.def("TestPyCppPy_ushort",            TestPyCppPy<unsigned short>);
    m.def("TestPyCppPy_sint",              TestPyCppPy<signed int>);
    m.def("TestPyCppPy_uint",              TestPyCppPy<unsigned int>);
    m.def("TestPyCppPy_slong",             TestPyCppPy<signed long>);
    m.def("TestPyCppPy_ulong",             TestPyCppPy<unsigned long>);
    m.def("TestPyCppPy_slonglong",         TestPyCppPy<signed long long>);
    m.def("TestPyCppPy_ulonglong",         TestPyCppPy<unsigned long long>);
    m.def("TestPyCppPy_float",             TestPyCppPy<float>);
    m.def("TestPyCppPy_double",            TestPyCppPy<double>);
    m.def("TestPyCppPy_longdouble",        TestPyCppPy<long double>);
    m.def("TestPyCppPy_string",            TestPyCppPy<std::string>);

    // Vector types
    m.def("TestPyCppPy_vector_sshort",     TestPyCppPy<std::vector<signed short>>);
    m.def("TestPyCppPy_vector_ushort",     TestPyCppPy<std::vector<unsigned short>>);
    m.def("TestPyCppPy_vector_sint",       TestPyCppPy<std::vector<signed int>>);
    m.def("TestPyCppPy_vector_uint",       TestPyCppPy<std::vector<unsigned int>>);
    m.def("TestPyCppPy_vector_slong",      TestPyCppPy<std::vector<signed long>>);
    m.def("TestPyCppPy_vector_ulong",      TestPyCppPy<std::vector<unsigned long>>);
    m.def("TestPyCppPy_vector_slonglong",  TestPyCppPy<std::vector<signed long long>>);
    m.def("TestPyCppPy_vector_ulonglong",  TestPyCppPy<std::vector<unsigned long long>>);
    m.def("TestPyCppPy_vector_float",      TestPyCppPy<std::vector<float>>);
    m.def("TestPyCppPy_vector_double",     TestPyCppPy<std::vector<double>>);
    m.def("TestPyCppPy_vector_longdouble", TestPyCppPy<std::vector<long double>>);
    m.def("TestPyCppPy_vector_string",     TestPyCppPy<std::vector<std::string>>);



    // Py-Cpp-Py round trip tests that should
    // fail on the cpp-to-py step
    // Arithmetic types
    m.def("TestPyCppPy_Fail_sshort",            TestPyCppPy_Fail<signed short>);
    m.def("TestPyCppPy_Fail_ushort",            TestPyCppPy_Fail<unsigned short>);
    m.def("TestPyCppPy_Fail_sint",              TestPyCppPy_Fail<signed int>);
    m.def("TestPyCppPy_Fail_uint",              TestPyCppPy_Fail<unsigned int>);
    m.def("TestPyCppPy_Fail_slong",             TestPyCppPy_Fail<signed long>);
    m.def("TestPyCppPy_Fail_ulong",             TestPyCppPy_Fail<unsigned long>);
    m.def("TestPyCppPy_Fail_slonglong",         TestPyCppPy_Fail<signed long long>);
    m.def("TestPyCppPy_Fail_ulonglong",         TestPyCppPy_Fail<unsigned long long>);
    m.def("TestPyCppPy_Fail_float",             TestPyCppPy_Fail<float>);
    m.def("TestPyCppPy_Fail_double",            TestPyCppPy_Fail<double>);
    m.def("TestPyCppPy_Fail_longdouble",        TestPyCppPy_Fail<long double>);
    m.def("TestPyCppPy_Fail_string",            TestPyCppPy_Fail<std::string>);

    // Vector types
    m.def("TestPyCppPy_Fail_vector_sshort",     TestPyCppPy_Fail<std::vector<signed short>>);
    m.def("TestPyCppPy_Fail_vector_ushort",     TestPyCppPy_Fail<std::vector<unsigned short>>);
    m.def("TestPyCppPy_Fail_vector_sint",       TestPyCppPy_Fail<std::vector<signed int>>);
    m.def("TestPyCppPy_Fail_vector_uint",       TestPyCppPy_Fail<std::vector<unsigned int>>);
    m.def("TestPyCppPy_Fail_vector_slong",      TestPyCppPy_Fail<std::vector<signed long>>);
    m.def("TestPyCppPy_Fail_vector_ulong",      TestPyCppPy_Fail<std::vector<unsigned long>>);
    m.def("TestPyCppPy_Fail_vector_slonglong",  TestPyCppPy_Fail<std::vector<signed long long>>);
    m.def("TestPyCppPy_Fail_vector_ulonglong",  TestPyCppPy_Fail<std::vector<unsigned long long>>);
    m.def("TestPyCppPy_Fail_vector_float",      TestPyCppPy_Fail<std::vector<float>>);
    m.def("TestPyCppPy_Fail_vector_double",     TestPyCppPy_Fail<std::vector<double>>);
    m.def("TestPyCppPy_Fail_vector_longdouble", TestPyCppPy_Fail<std::vector<long double>>);
    m.def("TestPyCppPy_Fail_vector_string",     TestPyCppPy_Fail<std::vector<std::string>>);



    ////////////////
    // Options
    ////////////////
    // Getting an option from a map
    m.def("TestOptionMapGet_bool",              &TestOptionMap_Get<bool>);
    m.def("TestOptionMapGet_sshort",            &TestOptionMap_Get<signed short>);
    m.def("TestOptionMapGet_ushort",            &TestOptionMap_Get<unsigned short>);
    m.def("TestOptionMapGet_sint",              &TestOptionMap_Get<signed int>);
    m.def("TestOptionMapGet_uint",              &TestOptionMap_Get<unsigned int>);
    m.def("TestOptionMapGet_slong",             &TestOptionMap_Get<signed long>);
    m.def("TestOptionMapGet_ulong",             &TestOptionMap_Get<unsigned long>);
    m.def("TestOptionMapGet_slonglong",         &TestOptionMap_Get<signed long long>);
    m.def("TestOptionMapGet_ulonglong",         &TestOptionMap_Get<unsigned long long>);
    m.def("TestOptionMapGet_float",             &TestOptionMap_Get<float>);
    m.def("TestOptionMapGet_double",            &TestOptionMap_Get<double>);
    m.def("TestOptionMapGet_longdouble",        &TestOptionMap_Get<long double>);
    m.def("TestOptionMapGet_string",            &TestOptionMap_Get<std::string>);

    m.def("TestOptionMapGet_vector_bool",       &TestOptionMap_Get<std::vector<bool>>);
    m.def("TestOptionMapGet_vector_sshort",     &TestOptionMap_Get<std::vector<signed short>>);
    m.def("TestOptionMapGet_vector_ushort",     &TestOptionMap_Get<std::vector<unsigned short>>);
    m.def("TestOptionMapGet_vector_sint",       &TestOptionMap_Get<std::vector<signed int>>);
    m.def("TestOptionMapGet_vector_uint",       &TestOptionMap_Get<std::vector<unsigned int>>);
    m.def("TestOptionMapGet_vector_slong",      &TestOptionMap_Get<std::vector<signed long>>);
    m.def("TestOptionMapGet_vector_ulong",      &TestOptionMap_Get<std::vector<unsigned long>>);
    m.def("TestOptionMapGet_vector_slonglong",  &TestOptionMap_Get<std::vector<signed long long>>);
    m.def("TestOptionMapGet_vector_ulonglong",  &TestOptionMap_Get<std::vector<unsigned long long>>);
    m.def("TestOptionMapGet_vector_float",      &TestOptionMap_Get<std::vector<float>>);
    m.def("TestOptionMapGet_vector_double",     &TestOptionMap_Get<std::vector<double>>);
    m.def("TestOptionMapGet_vector_longdouble", &TestOptionMap_Get<std::vector<long double>>);
    m.def("TestOptionMapGet_vector_string",     &TestOptionMap_Get<std::vector<std::string>>);

    // Changing an option in the map
    m.def("TestOptionMapChange_bool",              &TestOptionMap_Change<bool>);
    m.def("TestOptionMapChange_sshort",            &TestOptionMap_Change<signed short>);
    m.def("TestOptionMapChange_ushort",            &TestOptionMap_Change<unsigned short>);
    m.def("TestOptionMapChange_sint",              &TestOptionMap_Change<signed int>);
    m.def("TestOptionMapChange_uint",              &TestOptionMap_Change<unsigned int>);
    m.def("TestOptionMapChange_slong",             &TestOptionMap_Change<signed long>);
    m.def("TestOptionMapChange_ulong",             &TestOptionMap_Change<unsigned long>);
    m.def("TestOptionMapChange_slonglong",         &TestOptionMap_Change<signed long long>);
    m.def("TestOptionMapChange_ulonglong",         &TestOptionMap_Change<unsigned long long>);
    m.def("TestOptionMapChange_float",             &TestOptionMap_Change<float>);
    m.def("TestOptionMapChange_double",            &TestOptionMap_Change<double>);
    m.def("TestOptionMapChange_longdouble",        &TestOptionMap_Change<long double>);
    m.def("TestOptionMapChange_string",            &TestOptionMap_Change<std::string>);

    m.def("TestOptionMapChange_vector_bool",       &TestOptionMap_Change<std::vector<bool>>);
    m.def("TestOptionMapChange_vector_sshort",     &TestOptionMap_Change<std::vector<signed short>>);
    m.def("TestOptionMapChange_vector_ushort",     &TestOptionMap_Change<std::vector<unsigned short>>);
    m.def("TestOptionMapChange_vector_sint",       &TestOptionMap_Change<std::vector<signed int>>);
    m.def("TestOptionMapChange_vector_uint",       &TestOptionMap_Change<std::vector<unsigned int>>);
    m.def("TestOptionMapChange_vector_slong",      &TestOptionMap_Change<std::vector<signed long>>);
    m.def("TestOptionMapChange_vector_ulong",      &TestOptionMap_Change<std::vector<unsigned long>>);
    m.def("TestOptionMapChange_vector_slonglong",  &TestOptionMap_Change<std::vector<signed long long>>);
    m.def("TestOptionMapChange_vector_ulonglong",  &TestOptionMap_Change<std::vector<unsigned long long>>);
    m.def("TestOptionMapChange_vector_float",      &TestOptionMap_Change<std::vector<float>>);
    m.def("TestOptionMapChange_vector_double",     &TestOptionMap_Change<std::vector<double>>);
    m.def("TestOptionMapChange_vector_longdouble", &TestOptionMap_Change<std::vector<long double>>);
    m.def("TestOptionMapChange_vector_string",     &TestOptionMap_Change<std::vector<std::string>>);
}


} // close namespace export_python
} // close namespace testing 
} // close namespace pulsar

