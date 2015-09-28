/*! \file
 *
 * \brief Various test for the ModuleInfo class (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_TESTMODULEINFO_HPP_
#define _GUARD_TESTMODULEINFO_HPP_

#include <string>

#include "bpmodule/python_helper/BoostPython_fwd.hpp"



namespace bpmodule {
namespace testing {



/*! \brief Test construction of a ModuleInfo object
 *
 * \param [in] itest A test number
 * \param [in] desc Some description
 * \param [in] expected True if this is supoosed to succeed, false if it should
 *                      throw an exception
 * \param [in] mi A dictionary to construct from
 *
 * \return 0 if the test succeeds, 1 if it fails
 */
int TestModuleInfo_Construct(int itest, const std::string & desc, bool expected, const boost::python::dict & mi);




} // close namespace testing
} // close namespace bpmodule



#endif
