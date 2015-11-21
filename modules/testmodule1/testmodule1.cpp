#include <iostream>
#include <vector>

#include "bpmodule/modulelocator/ModuleLocator.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/modulebase/Test_Base.hpp"
#include "bpmodule/tensor/Matrix.hpp"

using namespace bpmodule::output;
using namespace bpmodule::modulebase;
using namespace bpmodule::modulelocator;
using namespace bpmodule::datastore;
using namespace bpmodule::tensor;
using bpmodule::exception::GeneralException;
using bpmodule::exception::ModuleCreateException;


class TestModule1 : public Test_Base
{
public:
    TestModule1(unsigned long id)
                   : Test_Base(id)
    {
    }

    virtual void RunTest(void)
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

    virtual void CallRunTest(const std::string & other)
    {
        Output("+++ In TestModule1: CallRunTest with %1%\n", other);

        ScopedModule<Test_Base> tb2 = MLocator().GetModule<Test_Base>(other);
        Output("  + Obtained scoped module ID %1%\n", tb2->ID());
        tb2->RunTest();
        Output("  + Finished with scoped module %1%. Deleting automatically\n", tb2->ID());

        Output("+++Done\n");
    }


    virtual void Throw(void)
    {
        Warning("+++ In TestModule1: Throwing an exception!\n");
        throw GeneralException("This is a test exception",
                               "Data1", "Hi",
                               "Data 2", "Hello");
    }



    virtual void CallThrow(const std::string & other)
    {
        Output("+++ In TestModule1: CallThrowTest with %1%\n", other);

        ScopedModule<Test_Base> tb2 = MLocator().GetModule<Test_Base>(other);
        Output("  + Obtained scoped module ID %1%\n", tb2->ID());
        tb2->Throw();

        // shouldn't be called
        Output("+++Done\n");
    }



    virtual CalcData CalcTest(CalcData inputs)
    {
        CalcData output;
        Output("HERE: %1%\n", output.HasType<double>("Hello"));

        MatrixD testmat(3,3);
        Output("Matrix dimensions: %1% x %2%\n", testmat.Height(), testmat.Width());

        DistMatrixD testdmat(100,100);
        Output("DistMatrix dimensions: %1% x %2%\n", testdmat.Height(), testdmat.Width());

        return output;
    }


    virtual ~TestModule1()
    {
    }

};


extern "C" {

ModuleBase * CreateModule(const std::string & name, unsigned long id)
{
    if(name == "testmodule1")
        return new TestModule1(id);
    else
        throw ModuleCreateException("Unknown module", name);
}



}

