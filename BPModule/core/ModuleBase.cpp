#include "BPModule/core/ModuleBase.h"
#include "BPModule/core/ModuleStore.h"

namespace bpmodule {

ModuleBase::ModuleBase(long id, ModuleStore * mstore, const OptionMap & options)
    : mstore_(*mstore), id_(id), options_(options)
{
}
  
ModuleInfo ModuleBase::MyInfo(void) const
{
  return mstore_.ModuleInfoFromID(id_);
}


} // close namespace bpmodule
