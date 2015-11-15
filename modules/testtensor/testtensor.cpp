#include <iostream>
#include <vector>

#include "bpmodule/modulelocator/ModuleLocator.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/modulebase/Test_Base.hpp"

using namespace bpmodule::output;
using namespace bpmodule::modulebase;
using namespace bpmodule::modulelocator;
using namespace bpmodule::datastore;
using bpmodule::exception::GeneralException;
using bpmodule::exception::ModuleCreateException;


class TestTensor : public Test_Base
{
public:
    TestTensor(unsigned long id)
                   : Test_Base(id)
    {
    }

    virtual void RunTest(void)
    {
        Output("+++ In TestTensor: RunTest. Info: (%1%) %2% %3% v%4%\n", ID(), Key(), Name(), Version());
    }

    virtual void CallRunTest(const std::string & other)
    {
        Output("+++ In TestTensor: CallRunTest with %1%\n", other);

        ScopedModule<Test_Base> tb2 = MLocator().GetModule<Test_Base>(other);
        Output("  + Obtained scoped module ID %1%\n", tb2->ID());
        tb2->RunTest();
        Output("  + Finished with scoped module %1%. Deleting automatically\n", tb2->ID());

        Output("+++Done\n");
    }


    virtual void Throw(void)
    {
        Warning("+++ In TestTensor: Throwing an exception!\n");
        throw GeneralException("This is a test exception",
                               "Data1", "Hi",
                               "Data 2", "Hello");
    }



    virtual void CallThrow(const std::string & other)
    {
        Output("+++ In TestTensor: CallThrowTest with %1%\n", other);

        ScopedModule<Test_Base> tb2 = MLocator().GetModule<Test_Base>(other);
        Output("  + Obtained scoped module ID %1%\n", tb2->ID());
        tb2->Throw();

        // shouldn't be called
        Output("+++Done\n");
    }



    virtual CalcData CalcTest(CalcData inputs)
    {
        CalcData output;

        // generate some data
        long i = 4;
        double d = Options().Get<double>("double_opt_def");
        double d2 = d * double(i);

        if(inputs.HasKey("TEST_INPUT_1"))
            d2 *= inputs.GetRef<double>("TEST_INPUT_1");

        return output;
    }


    virtual ~TestTensor()
    {
    }

};


extern "C" {

ModuleBase * CreateModule(const std::string & name, unsigned long id)
{
    if(name == "testtensor")
        return new TestTensor(id);
    else
        throw ModuleCreateException("Unknown module", name);
}



}

