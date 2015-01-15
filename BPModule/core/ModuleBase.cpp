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
 
void ModuleBase::Info(void) const
{
    ModuleInfo minfo = MyInfo();
    
    bpmodule::Output("---------------------------------\n");
    bpmodule::Output("%1% [ID %2%]\n", MyKey(), id_);
    bpmodule::Output("---------------------------------\n");
    minfo.Info();
}
  
void ModuleBase::Help(void) const
{
    ModuleInfo minfo = MyInfo();
    bpmodule::Output("---------------------------------\n");
    bpmodule::Output("%1% [ID %2%]\n", MyKey(), id_);
    bpmodule::Output("---------------------------------\n");
    minfo.Help(); 
}


} // close namespace bpmodule
