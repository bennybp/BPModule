#include "BPModule/modulebase/Test_Base.h"

namespace bpmodule {

Test_Base::Test_Base(unsigned long id, ModuleStore & mstore, const ModuleInfo & minfo)
    : ModuleBase(id, mstore, minfo)
{
}


} // close namespace bpmodule
