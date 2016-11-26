/*! \file
 * \brief Base class for testing of Pulsar (header)
 */

#pragma once

#include<string>

namespace pulsar {

/** \brief Class designed to assist in testing of Pulsar
 *
 * Individual tests are run via member functions of the derived
 * classes, and the the results are accumulated internally. 
 *
 * The results of individual tests are printed as they are run.
 * When finished, the final results can then be queried or printed.
 *
 * \threadunsafe
 */
class TesterBase
{
private:
    size_t nfailed_=0; ///< The number of failed tests
    size_t ntest_=0;///<The number of tests run

public:
    ///Initializes a tester for a set of tests described by \p desc
    TesterBase(const std::string& desc);
    
    /**\brief Prints the final results of the series of tests
     * 
     * \exstrong
     */
    void print_results()const;

    /*! \brief Obtain the total number of tests that we ran
     * 
     * \exnothrow
     */
    size_t ntest() const noexcept;

    /*! \brief Obtain the number of tests that failed
     * 
     * \exnothrow
     */
    size_t nfailed() const noexcept;
    
    /*! \brief Tests if boolean is true and increments the appropriate counters
     * 
     * Also prints a line with the result of the test.
     *
     * \exstrong
     *
     * \note All test commands should reduce to this test
     *
     * \param [in] desc A description of the test
     * \param [in] success True if the test passed, false if it failed
     *
     */
    void test(const std::string& desc, bool success);
};

}

