#include <iostream>
#include <vector>

#include "BPModule/core/ModuleStore.h"
#include "BPModule/core/Test_Base.h"

using namespace bpmodule;


class TestModule1 : public Test_Base
{
public:
    TestModule1(long id, ModuleStore * mstore, const OptionMap & options)
                   : Test_Base(id, mstore, options)
    {
    }

    void RunTest(void)
    {
        std::cout << "I am TestModule 1. Running tests...\n";
    }

    virtual ~TestModule1()
    {
        std::cout << "I am TestModule 1. Destructing\n";
    }

};


extern "C" {

ModuleBase * CreateModule(const std::string name, long id, ModuleStore * mstore, const OptionMap & options)
{
    if(name == "TESTMOD1")
        return new TestModule1(id, mstore, options);
    else
        throw std::runtime_error("No module by that name");
}



}

