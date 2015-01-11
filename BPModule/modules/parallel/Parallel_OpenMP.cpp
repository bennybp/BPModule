#include <vector>

#include "BPModule/core/ModuleStore.h"
#include "BPModule/modules/parallel/Parallel_OpenMP.h"


///////////////////////////
// Static variable storage
///////////////////////////
int bpmodule::Parallel_OpenMP::nthreads_ = 0;

using namespace bpmodule;


ModuleBase * NewOpenMPModule(ModuleStore * mstore, const std::string & filepath) { return new Parallel_OpenMP(mstore, filepath); }


extern "C" {

StoreType GetComponents(void)
{
    StoreType st;
    st["PARALLEL"] = NewOpenMPModule;
    return st;
}

}

