/*! \file
 *
 * \brief  Contains the macro definition of a dummy class for testing Pulsar
 *         C++ side
 */

#pragma once

#include<pulsar/testing/CppTester.hpp>
#include<pybind11/pybind11.h>


using namespace pulsar;

#define TEST_SIMPLE(test_name) \
    static size_t run_test(void); \
    \
    PYBIND11_PLUGIN(test_name)\
    {\
        pybind11::module m(#test_name, "Pulsar Simple Test"); \
        m.def("run_test", &run_test); \
        return m.ptr();\
    }\
    \
    static size_t run_test() \





#define TEST_FXN(desc,should_pass,expected, fxn)\
 do{bool success=false;\
 try{success=(expected==fxn && should_pass);}\
 catch(...){success=!should_pass;}\
 tester.test(desc,success);}while(0)

//Specializes TEST_FXN to case where fxn doesn't return
#define TEST_VOID(msg,should_pass,fxn)\
do{bool success=false;\
try{fxn;success=should_pass;}\
catch(...){success= !should_pass;}\
tester.test(msg,success);} while(0)
