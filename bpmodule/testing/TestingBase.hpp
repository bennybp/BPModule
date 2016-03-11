/*! \file
 *
 * \brief Convenience functions for testing the core library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_TESTING__TESTINGBASE_HPP_
#define BPMODULE_GUARD_TESTING__TESTINGBASE_HPP_

#include <functional>

#include "bpmodule/output/Output.hpp"


using bpmodule::output::GlobalOutput;
using bpmodule::output::GlobalError;
using bpmodule::output::GlobalDebug;

namespace bpmodule {
namespace testing {


/*! \brief Test a function call
 *
 * \tparam T A callable object type
 * \tparam Targs Types of the object's arguments
 *
 * \param [in] func A callable object
 * \param [in] Fargs Arguments for that callable object
 *
 * \return 1 If the function fails (throws), 0 if it succeeds
 */
template<typename T, typename... Targs>
int TestFunc(T func, Targs... Fargs)
{
    try {
       func(Fargs...); 
    }
    catch(std::exception & ex)
    {
        GlobalDebug(ex.what());
        GlobalDebug("\n");
        return 1;
    }   
    catch(...)
    {
        GlobalDebug("Caught unknown exception\n");
        return 1;
    }   



    // here if no exceptions are thrown
    return 0;
}



/*! \brief Test a function call (that returns a bool)
 *
 * \tparam T A callable object type that returns a bool
 * \tparam Targs Types of the object's arguments
 *
 * \param [in] func A callable object
 * \param [in] Fargs Arguments for that callable object
 *
 * \return 1 If the function fails (returns false or throws), 0 if it succeeds
 */
template<typename T, typename... Targs>
int TestBoolFunc(T func, Targs... Fargs)
{
    try {
        if(func(Fargs...))
            return 0;
        else
            return 1;
    }
    catch(std::exception & ex)
    {
        GlobalDebug(ex.what());
        GlobalDebug("\n");
        return 1;
    }   
    catch(...)
    {
        GlobalDebug("Caught unknown exception\n");
        return 1;
    }   

    return 0;
}




/*! \brief Test construction of an object
 *
 * \tparam T An object to construct
 * \tparam Targs Types of the object's constructor's arguments
 *
 * \param [in] Fargs Arguments for the object's constructor
 *
 * \return 1 If the construction fails, 0 if it succeeds
 */
template<typename T, typename... Targs>
int TestConstruct(Targs... Fargs)
{
    try {
       T obj(Fargs...); 
    }
    catch(std::exception & ex)
    {
        GlobalDebug(ex.what());
        GlobalDebug("\n");
        return 1;
    }   
    catch(...)
    {
        GlobalDebug("Caught unknown exception\n");
        return 1;
    }   



    // here if no exceptions are thrown
    return 0;
}



} // close namespace testing
} // close namespace bpmodule

#endif
