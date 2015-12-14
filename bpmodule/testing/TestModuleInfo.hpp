/*! \file
 *
 * \brief Various test for the ModuleInfo class (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_TESTMODULEINFO_HPP_
#define _GUARD_TESTMODULEINFO_HPP_

#include <string>

#include "bpmodule/python_helper/Pybind11.hpp"



namespace bpmodule {
namespace testing {



/*! \brief Test construction of a ModuleInfo object
 *
 * \param [in] mi A dictionary to construct from
 *
 * \return 0 if the test succeeds, 1 if it fails
 */
int TestModuleInfo_Construct(pybind11::dict mi);




} // close namespace testing
} // close namespace bpmodule



#endif
