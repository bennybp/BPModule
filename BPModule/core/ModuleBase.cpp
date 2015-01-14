#include "BPModule/core/ModuleBase.h"
#include "BPModule/core/ModuleStore.h"

namespace bpmodule {

ModuleBase::ModuleBase(long id, ModuleStore * mstore, const OptionMap & options)
    : mstore_(*mstore), out_(mstore->GetOutput()), id_(id), options_(options)
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
 
void ModuleBase::Dump(void) const
{
    ModuleInfo minfo = MyInfo();
    
    out_ << "---------------------------------\n"
         << MyKey() << "  [ID " << id_ << "]\n"
         << "---------------------------------\n";
    minfo.Dump(out_);
}
  
void ModuleBase::Help(void) const
{
    ModuleInfo minfo = MyInfo();
    out_ << "---------------------------------\n"
         << MyKey() << "  [ID " << id_ << "]\n"
         << "---------------------------------\n";
    minfo.Help(out_); 
}


} // close namespace bpmodule
