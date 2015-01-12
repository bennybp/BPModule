#include <iostream>
#include <dlfcn.h>

#include "BPModule/core/ModuleStore.h"
#include "BPModule/modules/base/Test_Base.h"
#include "BPModule/modules/base/Parallel_Base.h"

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

void RunTest(const char * modname)
{
    std::cout << "Test: " << modname << "\n";
    std::unique_ptr<Test_Base> tptr = mstore_.GetModule<Test_Base>(modname);
    tptr->RunTest();
}

void InitParallel(void)
{
}

void FinalizeParallel(void)
{
}

} // close extern C
