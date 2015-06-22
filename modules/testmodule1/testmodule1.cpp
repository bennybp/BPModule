#include <iostream>
#include <vector>

#include "BPModule/core/ModuleStore.h"
#include "BPModule/core/Output.h"

#include "BPModule/modulebase/Test_Base.h"

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
        Output("\nIn TestModule1: Testing different outputs\n");
        Debug("This is debugging output: %1% %2% %3%\n", 5, 2.4, "Hello");
        Output("This is regular output %1% %2% %3%\n", 5, 2.4, "Hello");
        Warning("This is warning output %1% %2% %3%\n", 5, 2.4, "Hello");
        Error("This is error output %1% %2% %3%\n", 5, 2.4, "Hello");
        Output("Done testing outputs\n\n");
    }

    virtual ~TestModule1()
    {
    }

};


extern "C" {

ModuleBase * CreateModule(const std::string & name, long id, ModuleStore * mstore, const OptionMap & options)
{
    if(name == "TESTMOD1")
        return new TestModule1(id, mstore, options);
    else
        throw ModuleException(ModuleException::Type::BadKey, __FILE__, name);
}



}

