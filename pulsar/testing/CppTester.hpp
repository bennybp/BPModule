/*! \file
 *
 * \brief  Add me
 * \author Ryan Richard (ryanmrichard1<at>gmail.com)
 */

#pragma once

#include "pulsar/testing/TesterBase.hpp"
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

