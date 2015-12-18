#include "TestModule1.hpp"
#include "TestExtLib.hpp"

using bpmodule::modulelocator::CreatorFunctions;


extern "C" {

CreatorFunctions CreateModule(void)
{
    CreatorFunctions cf;
    cf.AddCppCreator<TestModule1>("TestModule1");
    cf.AddCppCreator<TestExtLib>("TestExtLib");
    return cf;
}



}

