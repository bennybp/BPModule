#include "BPModule/core/ModuleBase.h"
#include "BPModule/core/ModuleStore.h"
#include "BPModule/core/Output.h"

namespace bpmodule {

ModuleBase::ModuleBase(unsigned long id, ModuleStore & mstore, const OptionMap & options)
    : id_(id), mstore_(mstore), options_(options)
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
