/*! \file
 *
 * \brief  Add me
 * \author Ryan Richard (ryanmrichard1<at>gmail.com)
 */

#pragma once

#include<string>

namespace pulsar {

/** \brief Class designed to assist in testing Pulsar's core
 *
 
 */
class TesterBase{
private:
    size_t nfailed_=0; ///< The number of failed tests
    size_t ntest_=0;///<The number of tests run

public:
    ///Initializes a tester for a set of tests described by \p desc
    TesterBase(const std::string& desc);
    
    /**\brief Ends our series of tests.
     *
     * Prints the results of the series of tests and raises an exception
     * if any test fails.  This exception is the sign to CTest that the
     * test failed, please don't catch it.
     * 
     * \throw PulsarException If one or more tests fails
     */
    void print_results()const;

    size_t nfailed() const noexcept;

    size_t ntest() const noexcept;
    
    ///Tests if boolean is true (all test commands should reduce to this test)
    void test(const std::string& desc, bool success);
};

}

