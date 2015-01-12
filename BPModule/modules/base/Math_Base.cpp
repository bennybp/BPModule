#include "BPModule/modules/base/Math_Base.h"

namespace bpmodule {

Math_Base::Math_Base(ModuleStore * mstore,
                     const std::string & filepath,
                     ModuleType mtype,
                     const std::string & name, const std::string & authors, 
                     const std::string & version, const std::string & description)
    : ModuleBase(mstore, filepath, ModuleClass::MATH, mtype, name, authors, version, description)
{
}


} // close namespace bpmodule
