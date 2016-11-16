/*! \file
 *
 * \brief  Add me
 * \author Ryan Richard (ryanmrichard1<at>gmail.com)
 */

#ifndef TESTER_HPP
#define TESTER_HPP

#include<string>

namespace pulsar {

/** \brief Class designed to assist in testing Pulsar's core
 *
 
 */
class Tester{
private:
    size_t nfailed_=0,///<The number of failed tests
           ntest_=1;///<The number tests run starting from 1
public:
    ///Initializes a tester for a set of tests described by \p desc
    Tester(const std::string& desc);
    
    /**\brief Ends our series of tests.
     *
     * Prints the results of the series of tests and raises an exception
     * if any test fails.  This exception is the sign to CTest that the
     * test failed, please don't catch it.
     * 
     * \throw GeneralException If one or more tests fails
     */
    void print_results()const;
    
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
    template<typename return_t,typename fxn_t,typename...Args>
    void test(const std::string& desc,
              bool should_pass,
              const return_t& expected,
              fxn_t fxn,
              Args&&...args)
    {
        bool success=false;
        try{success=(expected==fxn(std::forward<Args>(args)...) && should_pass);}
        catch(...){success=!should_pass;}
        test(desc,success);
    }
    
    
    ///Test that compares two, non-double, values
    template<typename T1,typename T2>
    void test(const std::string& desc,T1 v1, T2 v2){
        test(desc,v1==v2);
    }
    
    ///Tests if boolean is true (all test commands reduce to this test)
    void test(const std::string& desc,bool success);
    
    ///Overload of test for comparing doubles to within a tolerance
    void test(const std::string& desc,double v1,double v2,double tol=0.0001);
};
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
tester.test(msg,true,success);} while(0)
    

}//End namespace pulsar

#endif /* TESTER_HPP */

