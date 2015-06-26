#include "BPModule/core/ModuleBase.h"
#include "BPModule/core/ModuleStore.h"
#include "BPModule/core/Output.h"

namespace bpmodule {

ModuleBase::ModuleBase(unsigned long id, ModuleStore & mstore, const ModuleInfo & minfo)
    : id_(id), key_(minfo.key), name_(minfo.name), version_(minfo.version), options_(minfo.options),
      mstore_(mstore)
{
}

ModuleBase::~ModuleBase()
{
    output::Debug("Destructor called for module %1%\n", id_);
}

unsigned long ModuleBase::ID(void) const
{
    return id_;
}

const std::string & ModuleBase::Key(void) const
{
    return key_;
}

const std::string & ModuleBase::Name(void) const
{
    return name_;
}

const std::string & ModuleBase::Version(void) const
{
    return version_;
}

OptionMap ModuleBase::Traits(void) const
{
    return traits_;
}
 
OptionMap ModuleBase::Options(void) const
{
    return options_;
}

ModuleStore & ModuleBase::MStore(void)
{
    return mstore_;
}
 
} // close namespace bpmodule
