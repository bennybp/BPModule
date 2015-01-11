#include <vector>

#include "BPModule/core/ModuleStore.h"
#include "BPModule/modules/parallel/Parallel_Serial.h"

using namespace bpmodule;


ModuleBase * NewSerialModule(ModuleStore * mstore, const std::string & filepath) { return new Parallel_Serial(mstore, filepath); }


extern "C" {

StoreType GetComponents(void)
{
    StoreType st;
    st["PARALLEL"] = NewSerialModule;
    return st;
}


}

