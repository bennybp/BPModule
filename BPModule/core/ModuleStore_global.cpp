#include <iostream>


#include "BPModule/core/ModuleStore.h"

using namespace bpmodule;


namespace {
  ModuleStore mstore_;
}

extern "C" {


bool LoadSO(const char * modulepath, const char * components)
{
    return mstore_.LoadSO(modulepath, components);
}

int Count(void)
{
    return static_cast<int>(mstore_.Count());
}


void DumpInfo(void)
{
    mstore_.DumpInfo();
}


} // close extern C
