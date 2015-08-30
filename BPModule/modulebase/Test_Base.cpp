#include "BPModule/modulebase/Test_Base.hpp"

namespace bpmodule {
namespace modulebase {

Test_Base::Test_Base(unsigned long id, ModuleStore & mstore, const ModuleInfo & minfo)
    : ModuleBase(id, mstore, minfo)
{
}


} // close namespace modulebase
} // close namespace bpmodule
