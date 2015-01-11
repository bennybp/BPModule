#include "BPModule/core/ModuleClass.h"

namespace bpmodule {

const char * MClassToString(ModuleClass mc)
{
    switch(mc)
    {
        case ModuleClass::MCLASS_TEST:
          return "Test";
        default:
          return "???";
    }
}

const char * MTypeToString(ModuleType mt)
{
    switch(mt)
    {
        case ModuleType::MTYPE_TEST:
          return "Test";
        default:
          return "???";
    }
}

}
