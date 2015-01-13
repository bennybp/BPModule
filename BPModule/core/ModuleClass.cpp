#include <stdexcept>
#include "BPModule/core/ModuleClass.h"

namespace bpmodule {

const char * MClassToString(ModuleClass mc)
{
    switch(mc)
    {
        case ModuleClass::TEST:
          return "TEST";
        case ModuleClass::MATH:
          return "MATH";
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
        case ModuleType::GENERAL:
          return "GENERAL";
        default:
          return "???";
    }
}

ModuleClass StringToMClass(const char * str)
{
    std::string s(str);

    if(s == "TEST")
        return ModuleClass::TEST;
    else if(s == "MATH")
        return ModuleClass::MATH;
    else
        throw std::runtime_error("Unknown class");

}

ModuleType StringToMType(const char * str)
{
    std::string s(str);

    if(s == "TEST")
        return ModuleType::TEST;
    else if(s == "GENERAL")
        return ModuleType::GENERAL;
    else
        throw std::runtime_error("Unknown class");

}

} // close namespace bpmodule
