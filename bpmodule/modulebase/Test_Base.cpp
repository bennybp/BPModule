/*! \file
 *
 * \brief Base class for a test module (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/modulebase/Test_Base.hpp"

using bpmodule::modulelocator::ModuleLocator;
using bpmodule::modulelocator::ModuleInfo;

namespace bpmodule {
namespace modulebase {

Test_Base::Test_Base(unsigned long id)
    : ModuleBase(id)
{ }


} // close namespace modulebase
} // close namespace bpmodule
