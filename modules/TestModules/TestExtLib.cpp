#include <bpmodule/output/Output.hpp>
#include <bpmodule/modulelocator/ModuleLocator.hpp>
#include <bpmodule/exception/GeneralException.hpp>

#include "TestExtLib.hpp"

// From this module
#include "staticlib.hpp"
#include "dynlib.hpp"

using namespace bpmodule::output;
using namespace bpmodule::modulelocator;
using namespace bpmodule::exception;



TestExtLib::TestExtLib(unsigned long id)
               : Test_Base(id)
{
}



TestExtLib::~TestExtLib()
{
}



void TestExtLib::RunTest(void)
{
    Output("+++ In TestExtLib: RunTest. Info: (%1%) %2% %3% v%4%\n", ID(), Key(), Name(), Version());
    Output("*** Testing calling other libraries\n");
    std::string sstr = Static_GetString();
    std::string dstr = Dynamic_GetString();

    Output("       From static: %1%\n", sstr);
    Output("      From dynamic: %1%\n", dstr);
}



void TestExtLib::CallRunTest(const std::string & other)
{
}



void TestExtLib::Throw(void)
{
}



void TestExtLib::CallThrow(const std::string & other)
{
}

