#include "BPModule/modules/base/Test_Base.h"

namespace bpmodule {

Test_Base::Test_Base(ModuleStore * mstore,
                     const std::string & filepath,
                     ModuleType mtype,
                     const std::string & name, const std::string & authors, 
                     const std::string & version, const std::string & description)
    : ModuleBase(mstore, filepath, ModuleClass::TEST, mtype, name, authors, version, description)
{
}


} // close namespace bpmodule
