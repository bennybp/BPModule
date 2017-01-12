#include <pulsar/testing/CppTester.hpp>
#include <pulsar/modulemanager/ModuleCreationFuncs.hpp>
#include <pulsar/modulebase/ModuleBase.hpp>

using namespace std;
using namespace pulsar;

struct MyCPPModule:public ModuleBase{
    using BaseType=MyCPPModule;
    MyCPPModule(ID_t id):ModuleBase(id,"MyCPPModule"){}
};

using MCF=ModuleCreationFuncs;

TEST_SIMPLE(TestModuleCreationFuncs){
    CppTester tester("Testing ModuleCreationFunctions class");

    MCF mcf;
    string module_type="my module type",not_type="not a type";
    tester.test_member_call("Can add C++ module",true,
                          &MCF::add_cpp_creator<MyCPPModule>,&mcf,module_type);
    tester.test_member_return("C++ module is registered",true,true,
                              &MCF::has_creator,&mcf,module_type);
    tester.test_member_return("Handles non-registered type",true,false,
                              &MCF::has_creator,&mcf,not_type);
    tester.test_member_call("Get creator crashes for non-registered type",
        false,&MCF::get_creator,&mcf,not_type);

    tester.test_member_call("Get creator",true,&MCF::get_creator,&mcf,
                            module_type);
    tester.test_member_call("clear",true,&MCF::clear,&mcf);
    tester.test_member_return("clear worked",true,false,&MCF::has_creator,&mcf,
                                                                   module_type);
    tester.print_results();
    return tester.nfailed();
}
