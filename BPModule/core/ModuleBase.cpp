#include "BPModule/core/ModuleBase.h"
#include "BPModule/core/ModuleStore.h"
#include "BPModule/core/Output.h"

namespace bpmodule {

ModuleBase::ModuleBase(long id, ModuleStore * mstore, const OptionMap & options)
    : mstore_(*mstore), id_(id), options_(options)
{
}

std::string ModuleBase::MyKey(void) const
{
    return mstore_.KeyFromID(id_);
}

ModuleInfo ModuleBase::MyInfo(void) const
{
    return mstore_.ModuleInfoFromID(id_);
}

OptionMap ModuleBase::Traits(void) const
{
    return traits_;
}
 
OptionMap ModuleBase::Options(void) const
{
    return options_;
}
 
} // close namespace bpmodule
