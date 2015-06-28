#include <iostream>
#include <vector>

#include "BPModule/core/ModuleStore.hpp"
#include "BPModule/core/Output.hpp"

#include "BPModule/modulebase/Test_Base.hpp"

using namespace bpmodule;
namespace out = bpmodule::output;

class TestModule1 : public Test_Base
{
public:
    TestModule1(unsigned long id, ModuleStore & mstore, const ModuleInfo & minfo)
                   : Test_Base(id, mstore, minfo)
    {
    }

    virtual void RunTest(void)
    {
        out::Output("+++ In TestModule1: RunTest. Info: (%1%) %2% %3% v%4%\n", ID(), Key(), Name(), Version());
        /*
        out::Output("\nIn TestModule1: Testing different outputs\n");
        out::Debug("This is debugging output: %1% %2% %3%\n", 5, 2.4, "Hello");
        out::Output("This is regular output %1% %2% %3%\n", 5, 2.4, "Hello");
        out::Success("This is success output %1% %2% %3%\n", 5, 2.4, "Hello");
        out::Warning("This is warning output %1% %2% %3%\n", 5, 2.4, "Hello");
        out::Error("This is error output %1% %2% %3%\n", 5, 2.4, "Hello");
        out::Output("Done testing outputs\n\n");
        */
    }

    virtual void RunCallTest(const std::string & other)
    {
        out::Output("+++ In TestModule1: RunCallTest with %1%\n", other);

        ScopedModule<Test_Base> tb2 = MStore().GetScopedModule<Test_Base>(other);
        out::Output("  + Obtained scoped module ID %1%\n", tb2->ID());
        tb2->RunTest();
        out::Output("  + Finished with scoped module %1%. Deleting automatically\n", tb2->ID());

        Test_Base & tb = MStore().GetModule<Test_Base>(other);
        out::Output("  + Obtained module ID %1%\n", tb.ID());
        tb.RunTest();
        out::Output("  + Finished with module %1%. Deleting\n", tb.ID());
        MStore().Delete(tb.ID());

        out::Output("+++Done\n");


    }

    virtual ~TestModule1()
    {
    }

};


extern "C" {

ModuleBase * CreateModule(const std::string & key, unsigned long id, ModuleStore & mstore, const ModuleInfo & minfo)
{
    if(key == "TESTMOD1")
        return new TestModule1(id, mstore, minfo);
    else
        throw BPModuleException(
                                 "Unknown key",
                                 {
                                     { "Module", minfo.name },
                                     { "Path", minfo.path },
                                     { "Key", key }
                                 }
                               );
}



}

