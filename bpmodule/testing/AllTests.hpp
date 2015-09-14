/*! \file
 *
 * \brief Various test for the core (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_ALLTESTS_HPP_
#define _GUARD_ALLTESTS_HPP_

#include <string>
#include "bpmodule/python_helper/BoostPython_fwd.hpp"

namespace bpmodule {
namespace testing {

// TestModuleInfo.cpp
int TestModuleInfo(int itest, const std::string & desc, bool expected, const boost::python::dict & mi);


} // close namespace testing
} // close namespace bpmodule



#endif
