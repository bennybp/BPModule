#include "BPModule/base/ModuleBase.h"

namespace bpmodule {

ModuleBase::ModuleBase(ModuleClass mclass, ModuleType mtype,
                       const std::string & name, const std::string & authors, 
                       const std::string & version, const std::string & description)
    : mclass_(mclass), mtype_(mtype), name_(name), authors_(authors), 
      version_(version), description_(description)
{
}


} // close namespace bpmodule
