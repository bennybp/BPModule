#include <iostream>
#include <vector>

#include "bpmodule/modulelocator/ModuleLocator.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/modulebase/Test_Base.hpp"


// From this module
#include "staticlib.hpp"
#include "dynlib.hpp"

using namespace bpmodule::output;
using namespace bpmodule::modulebase;
using namespace bpmodule::modulelocator;
using namespace bpmodule::datastore;
using bpmodule::exception::GeneralException;
using bpmodule::exception::ModuleCreateException;


class TestExtLib : public Test_Base
{
public:
    TestExtLib(unsigned long id)
                   : Test_Base(id)
    {
    }

    virtual void RunTest(void)
    {
        Output("+++ In TestExtLib: RunTest. Info: (%1%) %2% %3% v%4%\n", ID(), Key(), Name(), Version());
        Output("*** Testing calling other libraries\n");
        std::string sstr = Static_GetString();
        std::string dstr = Dynamic_GetString();

        Output("       From static: %1%\n", sstr);
        Output("      From dynamic: %1%\n", dstr);
    }

    virtual void CallRunTest(const std::string & other)
    {
    }


    virtual void Throw(void)
    {
    }



    virtual void CallThrow(const std::string & other)
    {
    }



    virtual CalcData CalcTest(CalcData inputs)
    {
        CalcData output;
        return output;
    }


    virtual ~TestExtLib()
    {
    }

};


extern "C" {

ModuleBase * CreateModule(const std::string & name, unsigned long id)
{
    if(name == "testextlib")
        return new TestExtLib(id);
    else
        throw ModuleCreateException("Unknown module", name);
}



}

