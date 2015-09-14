/*! \file
 *
 * \brief Base class for a test module (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/modulebase/Test_Base.hpp"

using bpmodule::modulestore::ModuleStore;
using bpmodule::modulestore::ModuleInfo;

namespace bpmodule {
namespace modulebase {

Test_Base::Test_Base(unsigned long id,
                     ModuleStore & mstore, 
                     const ModuleInfo & minfo)
    : ModuleBase(id, mstore, minfo)
{
}


} // close namespace modulebase
} // close namespace bpmodule
