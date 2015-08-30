#include "bpmodule/modulebase/Test_Base.hpp"

namespace bpmodule {
namespace modulebase {

Test_Base::Test_Base(unsigned long id,
                     modulestore::ModuleStore & mstore, 
                     const modulestore::ModuleInfo & minfo)
    : ModuleBase(id, mstore, minfo)
{
}


} // close namespace modulebase
} // close namespace bpmodule
