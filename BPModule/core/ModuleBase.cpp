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
 
void ModuleBase::PrintInfo(void) const
{
    ModuleInfo minfo = MyInfo();
    
    Output(Line('-'));
    Output("KEY: %1% [ID %2%]\n", MyKey(), id_);
    Output(Line('-'));
    minfo.PrintInfo();
}
  
void ModuleBase::PrintHelp(void) const
{
    ModuleInfo minfo = MyInfo();
    Output(Line('-'));
    Output("KEY: %1% [ID %2%]\n", MyKey(), id_);
    Output(Line('-'));
    minfo.PrintHelp(); 
}


} // close namespace bpmodule
