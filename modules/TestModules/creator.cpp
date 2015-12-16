#include <bpmodule/modulebase/ModuleBase.hpp>
#include <bpmodule/exception/ModuleCreateException.hpp>

#include "TestModule1.hpp"
#include "TestExtLib.hpp"

using bpmodule::modulebase::ModuleBase;
using bpmodule::exception::ModuleCreateException;
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

