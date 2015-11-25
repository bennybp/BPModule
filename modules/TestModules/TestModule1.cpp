#include <bpmodule/output/Output.hpp>
#include <bpmodule/modulelocator/ModuleLocator.hpp>
#include <bpmodule/exception/GeneralException.hpp>

#include "TestModule1.hpp"


using namespace bpmodule::output;
using namespace bpmodule::modulelocator;
using namespace bpmodule::exception;



TestModule1::TestModule1(unsigned long id)
    : Test_Base(id)
{
}



TestModule1::~TestModule1()
{
}



void TestModule1::RunTest(void)
{
    Output("+++ In TestModule1: RunTest. Info: (%1%) %2% %3% v%4%\n", ID(), Key(), Name(), Version());
    Output("   double_opt_def:    %1%\n", Options().Get<double>("double_opt_def"));
    Output("      int_opt_def:    %1%\n", Options().Get<int>("int_opt_def"));
    Output("     bool_opt_def:    %1%\n", Options().Get<bool>("bool_opt_def"));
    Output("      str_opt_def:    %1%\n", Options().Get<std::string>("str_opt_def"));
    Output("\n");

    if(Options().Has("double_opt"))
        Output("       double_opt:    %1%\n", Options().Get<double>("double_opt"));

    if(Options().Has("int_opt"))
        Output("          int_opt:    %1%\n", Options().Get<int>("int_opt"));

    if(Options().Has("bool_opt"))
        Output("         bool_opt:    %1%\n", Options().Get<bool>("bool_opt"));

    if(Options().Has("str_opt"))
        Output("          str_opt:    %1%\n", Options().Get<std::string>("str_opt"));
}



void TestModule1::CallRunTest(const std::string & other)
{
    Output("+++ In TestModule1: CallRunTest with %1%\n", other);

    ScopedModule<Test_Base> tb2 = MLocator().GetModule<Test_Base>(other);
    Output("  + Obtained scoped module ID %1%\n", tb2->ID());
    tb2->RunTest();
    Output("  + Finished with scoped module %1%. Deleting automatically\n", tb2->ID());

    Output("+++Done\n");
}



void TestModule1::Throw(void)
{
    Warning("+++ In TestModule1: Throwing an exception!\n");
    throw GeneralException("This is a test exception",
                           "Data1", "Hi",
                           "Data 2", "Hello");
}



void TestModule1::CallThrow(const std::string & other)
{
    Output("+++ In TestModule1: CallThrowTest with %1%\n", other);

    ScopedModule<Test_Base> tb2 = MLocator().GetModule<Test_Base>(other);
    Output("  + Obtained scoped module ID %1%\n", tb2->ID());
    tb2->Throw();

    // shouldn't be called
    Output("+++Done\n");
}
