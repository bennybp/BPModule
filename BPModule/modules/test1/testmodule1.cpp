#include <iostream>
#include <vector>

#include "BPModule/core/ModuleStore.h"
#include "BPModule/core/Test_Base.h"
#include "BPModule/core/Output.h"

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
        Output("This is regular output\n");
        Warning("This is warning output\n");
        Error("This is error output\n");
    }

    virtual ~TestModule1()
    {
    }

};


extern "C" {

ModuleBase * CreateModule(const std::string name, long id, ModuleStore * mstore, const OptionMap & options)
{
    if(name == "TESTMOD1")
        return new TestModule1(id, mstore, options);
    else
        throw ModuleException(ModuleException::Type::BadKey, __FILE__, name);
}



}

