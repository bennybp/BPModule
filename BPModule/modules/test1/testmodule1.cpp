#include <iostream>
#include <vector>

#include "BPModule/core/ModuleStore.h"
#include "BPModule/core/base/Test_Base.h"

using namespace bpmodule;


class TestModule1 : public Test_Base
{
public:
    TestModule1(ModuleStore * mstore, const OptionMap & options)
                   : Test_Base(mstore, options)
    {
    }

    void RunTest(void)
    {
        std::cout << "I am TestModule 1. Running tests...\n";
    }

};


extern "C" {

ModuleBase * CreateModule(const std::string name, ModuleStore * mstore, const OptionMap & options)
{
    if(name == "TESTMOD1")
        return new TestModule1(mstore, options);
    else
        throw std::runtime_error("No module by that name");
}



}

