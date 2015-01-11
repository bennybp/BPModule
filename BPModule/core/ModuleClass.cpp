#include "BPModule/core/ModuleClass.h"

namespace bpmodule {

const char * MClassToString(ModuleClass mc)
{
    switch(mc)
    {
        case ModuleClass::TEST:
          return "Test";
        case ModuleClass::PARALLEL:
          return "PARALLEL";
        default:
          return "???";
    }
}

const char * MTypeToString(ModuleType mt)
{
    switch(mt)
    {
        case ModuleType::TEST:
          return "Test";
        case ModuleType::SERIAL:
          return "SERIAL";
        case ModuleType::LOCAL:
          return "LOCAL";
        case ModuleType::DISTRIBUTED:
          return "DISTRIBUTED";
        default:
          return "???";
    }
}

}
