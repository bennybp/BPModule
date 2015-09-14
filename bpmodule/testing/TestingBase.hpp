/*! \file
 *
 * \brief Convenience functions for testing the core library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_TESTINGBASE_HPP_
#define _GUARD_TESTINGBASE_HPP_

#include <functional>

#include "bpmodule/output/Output.hpp"

using bpmodule::output::Output;
using bpmodule::output::Error;

namespace bpmodule {
namespace testing {


/*! \brief Test a function call
 *
 * \tparam T A callable object type
 * \tparam Targs Types of the object's arguments
 *
 * \param [in] itest A test number
 * \param [in] desc Some description
 * \param [in] expected True if this is supoosed to succeed, false if it should
 *                      throw an exception
 * \param [in] func A callable object
 * \param [in] Fargs Arguments for that callable object
 *
 * \return 1 If the test fails, 0 if it succeeds
 */
template<typename T, typename... Targs>
int TestFunc(int itest, const std::string & desc, bool expected, T func, Targs... Fargs)
{
    std::string fmt("%|1$5| : %|2$-9|  : %3%\n");

    try {
       func(Fargs...); 
    }
    catch(...)
    {
        if(expected == false)
        {
            Output(fmt, itest, "Success", desc);
            return 0;
        }
        else
        {
            Error(fmt, itest, "FAILED", desc);
            return 1;
        }
        
    }

    if(expected == true)
    {
        Output(fmt, itest, "Success", desc);
        return 0;
    }
    else
    {
        Error(fmt, itest, "FAILED", desc);
        return 1;
    }
}



/*! \brief Test construction of an object
 *
 * \tparam T An object to construct
 * \tparam Targs Types of the object's constructor's arguments
 *
 * \param [in] itest A test number
 * \param [in] desc Some description
 * \param [in] expected True if this is supoosed to succeed, false if it should
 *                      throw an exception
 * \param [in] Fargs Arguments for the object's constructor
 *
 * \return 1 If the test fails, 0 if it succeeds
 */
template<typename T, typename... Targs>
int TestConstruct(int itest, const std::string & desc, bool expected, Targs... Fargs)
{
    std::string fmt("%|1$5| : %|2$-9|  : %3%\n");

    try {
       T obj(Fargs...); 
    }
    catch(...)
    {
        if(expected == false)
        {
            Output(fmt, itest, "Success", desc);
            return 0;
        }
        else
        {
            Error(fmt, itest, "FAILED", desc);
            return 1;
        }
        
    }

    if(expected == true)
    {
        Output(fmt, itest, "Success", desc);
        return 0;
    }
    else
    {
        Error(fmt, itest, "FAILED", desc);
        return 1;
    }
}



} // close namespace testing
} // close namespace bpmodule

#endif
