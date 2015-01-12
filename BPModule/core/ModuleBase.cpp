#include "BPModule/core/ModuleBase.h"

namespace bpmodule {

ModuleBase::ModuleBase(ModuleStore * mstore, const OptionMap & options)
    : mstore_(*mstore), options_(options)
{
}


} // close namespace bpmodule
