/*! \file
 *
 * \brief  Add me
 * \author Ryan Richard (ryanmrichard1<at>gmail.com)
 */

#pragma once

#include "pulsar/testing/TesterBase.hpp"
#include <pybind11/pybind11.h>
#include <cmath>

namespace pulsar {

/** \brief Class designed to assist in testing Pulsar's core
 *
 
 */
class CppTester : public TesterBase
{
public:
    using TesterBase::TesterBase;
    
    /**\brief The main testing function.
     * 
     * This function is capable of both testing a function for failure and
     * for success.  In the latter, likely more common scenario, this function
     * will ensure that the return value of the function matches the expected
     * value and that the function does not throw.  In the former case your
     * function should throw when it fails and the throw will be caught and
     * your test marked as successful.
     * 
     * \param[in] desc description of the test we are running 
     * \param[in] should_pass True if we expect the test to match expected
     * \param[in] expected the expected return of func
     * \param[in] func A callable object to test
     * \param[in] args The arguments to pass to func
     */
    template<typename R, typename Func, typename...Args>
    void test_function(const std::string & desc,
                       bool should_pass,
                       const R & expected,
                       Func func,
                       Args&&...args)
    {
        bool success=false;
        try{
            R r = func(std::forward<Args>(args)...);
            success = (expected == r) && should_pass;
        }
        catch(...)
        {
            success = !should_pass;
        }

        test(desc, success);
    }
    
    ///Test that compares two, non-double, values
    template<typename T1, typename T2>
    void test_equal(const std::string& desc, const T1 & v1, const T2 & v2)
    {
        test(desc, v1 == v2);
    }

    ///Test that compares two, non-double, values
    template<typename T1, typename T2>
    void test_not_equal(const std::string& desc, const T1 & v1, const T2 & v2)
    {
        test(desc, !(v1 == v2));
    }
    
    ///Overload of test for comparing doubles to within a tolerance
    void test_double(const std::string& desc,double v1,double v2,double tol=0.0001)
    {
        test(desc,std::fabs(v1-v2)<tol);
    }
};

}

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
