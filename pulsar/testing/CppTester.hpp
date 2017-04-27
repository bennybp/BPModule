/*! \file
 * \brief Class for testing C++ classes in pulsar (header)
 */

#pragma once

#include "pulsar/testing/TesterBase.hpp"
#include "pulsar/output/GlobalOutput.hpp"
#include "pulsar/util/Pybind11.hpp"
#include <cmath>

namespace pulsar {

/** \brief Class designed to assist in testing Pulsar's core
 *
 * This is the C++ version of the Pulsar tester class. It implements
 * methods for testing C++ functions and other functionality.
 *
 * \threadunsafe
 */
class CppTester : public TesterBase
{
public:
    using TesterBase::TesterBase;

    /*!\brief Tests the return value of a function
     *
     * This function is capable of both testing a function for failure and
     * for success.  In the latter, likely more common scenario, this function
     * will ensure that the return value of the function matches the expected
     * value and that the function does not throw. The former can be used
     * to make sure values or objects change when they are expected to, although
     * this kind of test is expected to be uncommon.
     *
     * If the function throws an exception, it will be marked as a failure.
     * If you are testing for proper exception behavior, use `test_call` instead.
     *
     * \exsafe_par If an exception is thrown anywhere, the
     *             tester object remains in its original state. However,
     *             the safety of this function with respect to the arguments
     *             depends on the safety of those arguments.
     *
     * \param[in] desc description of the test we are running
     * \param[in] should_pass True if we expect the test to match expected
     * \param[in] expected the expected return of func
     * \param[in] func A callable object to test
     * \param[in] args The arguments to pass to \p func
     */
    template<typename R, typename Func, typename...Targs>
    void test_return(const std::string & desc,
                     bool should_pass,
                     const R & expected,
                     Func func,
                     Targs&&...args)
    {
        bool success;

        try {
            R r = func(std::forward<Targs>(args)...);
            success = (expected == r) && should_pass;
        }
        catch(std::exception & ex)
        {
            print_global_debug("Caught exception in test\n");
            print_global_debug(ex.what());
            success = false;
        }
        catch(...)
        {
            print_global_debug("Caught unknown exception in test\n");
            success = false;
        }

        test(desc, success);
    }


    /*! \brief Tests the return value of a class member function call
     *
     * This is similar to test_return, however can be used to test
     * a member function of a class instead.
     *
     * \exsafe_par If an exception is thrown anywhere, the
     *             tester object remains in its original state. However,
     *             the safety of this function with respect to the arguments
     *             depends on the safety of those arguments.
     *
     * \note This version is an overload for non-const member functions
     *
     * \note If you are going to be testing the same member function
     *       multiple times, consider wrapping the function via
     *       std::bind and calling test_return directly. It will
     *       probably be easier.
     *
     * \param[in] desc description of the test we are running
     * \param[in] should_pass True if we expect the test to match expected
     * \param[in] expected the expected return of func
     * \param[in] func Pointer to a class member function
     * \param[in] objptr Pointer to a class object (with which to call \p func)
     * \param[in] args The arguments to pass to \p func
     */
    template<typename R1, typename R2, typename Class,
             typename... Targs1, typename... Targs2>
    void test_member_return(const std::string & desc,
                            bool should_pass,
                            const R2 & expected,
                            R1 (Class::*func)(Targs1...),
                            Class * objptr,
                            Targs2 &&... args)
    {
        auto f = std::bind(func, objptr, std::forward<Targs1>(args)...);
        test_return(desc, should_pass, expected, f);
    }

    
    /*! \brief Tests the return value of a class member function call
     *
     * This is similar to test_return, however can be used to test
     * a member function of a class instead.
     *
     * \exsafe_par If an exception is thrown anywhere, the
     *             tester object remains in its original state. However,
     *             the safety of this function with respect to the arguments
     *             depends on the safety of those arguments.
     *
     * \note This version is an overload for const member functions
     *
     * \note If you are going to be testing the same member function
     *       multiple times, consider wrapping the function via
     *       std::bind and calling test_return directly. It will
     *       probably be easier.
     *
     * \param[in] desc description of the test we are running
     * \param[in] should_pass True if we expect the test to match expected
     * \param[in] expected the expected return of func
     * \param[in] func Pointer to a class member function
     * \param[in] objptr Pointer to a class object (with which to call \p func)
     * \param[in] args The arguments to pass to \p func
     */
    template<typename R1, typename R2, typename Class,
             typename... Targs1, typename... Targs2>
    void test_member_return(const std::string & desc,
                            bool should_pass,
                            const R2 & expected,
                            R1 (Class::*func)(Targs1...) const,
                            const Class * objptr,
                            Targs2 &&... args)
    {
        auto f = std::bind(func, objptr, std::forward<Targs1>(args)...);
        test_return(desc, should_pass, expected, f);
    }



    /**\brief Tests if a function throws an exception
     *
     * Success is marked when the given function is successfully called and
     * does not throw an exception.
     *
     * \exsafe_par If an exception is thrown anywhere, the
     *             tester object remains in its original state. However,
     *             the safety of this function with respect to the arguments
     *             depends on the safety of those arguments.
     *
     * \param[in] desc description of the test we are running
     * \param[in] func A callable object to test
     * \param[in] args The arguments to pass to func
     */
    template<typename Func, typename...Targs>
    void test_call(const std::string & desc,
                   bool should_pass,
                   Func func,
                   Targs&&...args)
    {
        bool success;

        try {
            func(std::forward<Targs>(args)...);
            success = should_pass;
        }
        catch(std::exception & ex)
        {
            print_global_debug("Caught exception in test\n");
            print_global_debug(ex.what());
            success = !should_pass;
        }
        catch(...)
        {
            print_global_debug("Caught unknown exception in test\n");
            success = !should_pass;
        }

        test(desc, success);
    }


    /*! \brief Tests calling a class member function
     *
     * This is similar to test_call, however can be used to test
     * a member function of a class instead.
     *
     * \exsafe_par If an exception is thrown anywhere, the
     *             tester object remains in its original state. However,
     *             the safety of this function with respect to the arguments
     *             depends on the safety of those arguments.
     *
     * \note This version is an overload for non-const member functions
     *
     * \note If you are going to be testing the same member function
     *       multiple times, consider wrapping the function via
     *       std::bind and calling test_call directly. It will
     *       probably be easier.
     *
     * \param[in] desc description of the test we are running
     * \param[in] should_pass True if we expect the test to match expected
     * \param[in] func Pointer to a class member function
     * \param[in] objptr Pointer to a class object (with which to call \p func)
     * \param[in] args The arguments to pass to \p func
     */
    template<typename Class, typename R, typename... Targs1, typename... Targs2>
    void test_member_call(const std::string & desc,
                          bool should_pass,
                          R (Class::*func)(Targs1...),
                          Class * objptr,
                          Targs2 &&... args)
    {
        auto f = std::bind(func, objptr, std::forward<Targs1>(args)...);
        test_call(desc, should_pass, f);
    }


    /*! \brief Tests calling a class member function
     *
     * This is similar to test_call, however can be used to test
     * a member function of a class instead.
     *
     * \exsafe_par If an exception is thrown anywhere, the
     *             tester object remains in its original state. However,
     *             the safety of this function with respect to the arguments
     *             depends on the safety of those arguments.
     *
     * \note This version is an overload for const member functions
     *
     * \note If you are going to be testing the same member function
     *       multiple times, consider wrapping the function via
     *       std::bind and calling test_call directly. It will
     *       probably be easier.
     *
     * \param[in] desc description of the test we are running
     * \param[in] should_pass True if we expect the test to match expected
     * \param[in] func Pointer to a class member function
     * \param[in] objptr Pointer to a class object (with which to call \p func)
     * \param[in] args The arguments to pass to \p func
     */
    template<typename Class, typename R, typename... Targs1, typename... Targs2>
    void test_member_call(const std::string & desc,
                          bool should_pass,
                          R (Class::*func)(Targs1...) const,
                          Class * objptr,
                          Targs2 &&... args)
    {
        auto f = std::bind(func, objptr, std::forward<Targs1>(args)...);
        test_call(desc, should_pass, f);
    }


    /*! \brief Compares two, non-double, values for equality
     *
     * \exstrong
     */
    template<typename T1, typename T2>
    void test_equal(const std::string& desc, const T1 & v1, const T2 & v2)
    {
        test(desc, v1 == v2);
    }

    /*! \brief Test that compares two, non-double, values for inequality
     *
     * \exstrong
     */
    template<typename T1, typename T2>
    void test_not_equal(const std::string& desc, const T1 & v1, const T2 & v2)
    {
        test(desc, !(v1 == v2));
    }

    /*! \brief Overload of test for comparing doubles to within a tolerance
     *
     * \exstrong
     */
    void test_double(const std::string& desc,double v1,double v2,double tol=0.0001)
    {
        test(desc,std::fabs(v1-v2)<tol);
    }

    /*! \brief Overload of test that compares containers of doubles to a tolerance
     *
     * \exstrong
     */
    template<typename T1,typename T2>
    void test_double_vector(const std::string& desc,const T1& v1, const T2& v2, double tol=0.0001)
    {
        bool is_good=true;
        is_good = std::inner_product(v1.begin(),v1.end(),v2.begin(),is_good,
                     std::logical_and<double>(),
                     [=](double v1x,double v2x){return std::fabs(v1x-v2x)<tol;});
        test(desc,is_good);
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



/*! \todo Remove these eventually? */
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
