/*! \file
 *
 * \brief Various test for the ModuleInfo class (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include "bpmodule/testing/TestingBase.hpp"
#include "bpmodule/testing/TestModuleInfo.hpp"
#include "bpmodule/modulelocator/ModuleInfo.hpp"


using bpmodule::modulelocator::ModuleInfo;

namespace bpmodule {
namespace testing {


int TestModuleInfo_Construct(pybind11::dict mi)
{
    return TestConstruct<ModuleInfo>(mi);
}


} // close namespace testing
} // close namespace bpmodule
