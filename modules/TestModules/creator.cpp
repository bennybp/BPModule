#include "TestModule1.hpp"
#include "TestExtLib.hpp"

using bpmodule::modulelocator::ModuleCreationFuncs;


extern "C" {

ModuleCreationFuncs CreateModule(void)
{
    ModuleCreationFuncs cf;
    cf.AddCppCreator<TestModule1>("TestModule1");
    cf.AddCppCreator<TestExtLib>("TestExtLib");
    return cf;
}



}

