#include "TestCppModules.hpp"

using pulsar::ModuleCreationFuncs;

extern "C" {

ModuleCreationFuncs insert_supermodule(void)
{
    ModuleCreationFuncs cf;
    cf.add_cpp_creator<TestCppModule1>("TestCppModule1");
    return cf;
}



}

