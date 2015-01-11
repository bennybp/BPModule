#include <iostream>


#include "BPModule/store/ModuleStore.h"

using namespace bpmodule;


namespace {
  ModuleStore mstore_;
}

extern "C" {


bool LoadModule(const char * modulepath, const char * components)
{
    return mstore_.LoadModule(modulepath, components);
}

void CloseAll(void)
{
    mstore_.CloseAll();
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
