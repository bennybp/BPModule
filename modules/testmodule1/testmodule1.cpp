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
        out::Output("   double_opt_def:    %1%\n", GetOption<double>("double_opt_def"));
        out::Output("      int_opt_def:    %1%\n", GetOption<long>("int_opt_def"));
        out::Output("     bool_opt_def:    %1%\n", GetOption<bool>("bool_opt_def"));
        out::Output("      str_opt_def:    %1%\n", GetOption<std::string>("str_opt_def"));
        out::Output("\n");
        if(HasOption("double_opt"))
            out::Output("       double_opt:    %1%\n", GetOption<double>("double_opt"));
        if(HasOption("int_opt"))
            out::Output("          int_opt:    %1%\n", GetOption<long>("int_opt"));
        if(HasOption("bool_opt"))
            out::Output("         bool_opt:    %1%\n", GetOption<bool>("bool_opt"));
        if(HasOption("str_opt"))
            out::Output("          str_opt:    %1%\n", GetOption<std::string>("str_opt"));
    }

    virtual void CallRunTest(const std::string & other)
    {
        out::Output("+++ In TestModule1: CallRunTest with %1%\n", other);

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


    virtual void Throw(void)
    {
        out::Warning("+++ In TestModule1: Throwing an exception!\n");
        ThrowException(
                        "This is a test exception", 
                        {
                           { "Data 1", "Hi" },
                           { "Data 2", "Hello" }
                        }
                      );
                       
    }



    virtual void CallThrow(const std::string & other)
    {
        out::Output("+++ In TestModule1: CallThrowTest with %1%\n", other);

        ScopedModule<Test_Base> tb2 = MStore().GetScopedModule<Test_Base>(other);
        out::Output("  + Obtained scoped module ID %1%\n", tb2->ID());
        tb2->Throw();

        // shouldn't be called
        out::Output("+++Done\n");
    }



    virtual CalcData CalcTest(CalcData inputs)
    {
        CalcData output;

        // generate some data
        long i = GetOption<long>("int_opt_def");
        double d = GetOption<double>("double_opt_def");
        double d2 = d * double(i);

        if(inputs.Has("TEST_INPUT_1"))
            d2 *= inputs.GetRef<double>("TEST_INPUT_1");

        output.Set("TEST_RESULT_1", d2);

        return output;
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

