/*! \file
 *
 * \brief  Contains the macro definition of a dummy class for testing Pulsar
 *         C++ side
 */

#pragma once

#include<pulsar/testing/Tester.hpp>
#include<pybind11/pybind11.h>


using namespace pulsar;

#define TEST_SIMPLE(test_name) \
    static void run_test(void); \
    \
    PYBIND11_PLUGIN(test_name)\
    {\
        pybind11::module m(#test_name, "Pulsar Simple Test"); \
        m.def("run_test", &run_test); \
        return m.ptr();\
    }\
    \
    static void run_test() \

