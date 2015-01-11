#include "BPModule/base/ModuleBase.h"

namespace bpmodule {

ModuleBase::ModuleBase(const std::string & filepath,
                       ModuleClass mclass, ModuleType mtype,
                       const std::string & name, const std::string & authors, 
                       const std::string & version, const std::string & description)
    : filepath_(filepath), mclass_(mclass), mtype_(mtype), name_(name), authors_(authors), 
      version_(version), description_(description)
{
}


} // close namespace bpmodule
