#include <pulsar/testing/CppTester.hpp>
#include <pulsar/modulemanager/ModuleManager.hpp>
#include <pulsar/modulebase/TestModule.hpp>
#include <unistd.h>//For getcwd switch to filesystem when C++17 is more widespread


using namespace std;
using namespace pulsar;

class CXXModule2:public pulsar::TestModule{
public:
    CXXModule2(ID_t id):TestModule(id){}
private:
    void run_test_(){

    }
};

TEST_SIMPLE(TestModuleManager){
    CppTester tester("Testing ModuleManager class");

    auto pmm=make_shared<ModuleManager>();
    ModuleManager& mm=*pmm;
    ModuleInfo minfo;
    string mod_name="my module",not_name="Not a module",opt_key="Option1";
    double opt=2.0;
    void add_option(std::string key, OptionType opttype, bool required,
                    const pybind11::object & validator, std::string help,
                    const pybind11::object & def);
    minfo.options.add_option(opt_key,OptionType::Float,true,pybind11::none(),
                             "",pybind11::none());
    auto load_module=&ModuleManager::load_module_from_minfo;
    tester.test_member_return("has non-existent module",true,false,
                              &ModuleManager::has_key,&mm,mod_name);
    tester.test_member_call("No path causes failure",false,load_module,
                            &mm,minfo,mod_name);
    minfo.path="a/fake/path";
    tester.test_member_call("No handler",false,load_module,&mm,minfo,mod_name);
    minfo.type="c_module";
    tester.test_member_call("Not a path",false,load_module,&mm,minfo,mod_name);
    char cwd[1026];
    getcwd(cwd, sizeof(cwd));
    minfo.path=string(cwd)+"/test_stage/modulemanager/CXXModule.so";
    tester.test_member_call("No name",false,load_module,&mm,minfo,mod_name);
    minfo.name="C++ Module";
    tester.test_member_call("load_module",true,load_module,&mm,minfo,mod_name);
    tester.test_member_return("has module",true,true,&ModuleManager::has_key,
                              &mm,mod_name);
    tester.test_member_return("size",true,1,&ModuleManager::size,&mm);
    tester.test_member_return("module key info",true,minfo,
                              &ModuleManager::module_key_info,&mm,mod_name);
    tester.test_member_call("module key info non-existent key",false,
                                  &ModuleManager::module_key_info,&mm,not_name);
    tester.test_member_call("test all no required opt",false,
                            &ModuleManager::test_all,&mm);
    tester.test_member_call("set required opt bad key",false,
                &ModuleManager::change_option<double>,&mm,not_name,opt_key,opt);
    tester.test_member_call("set required opt bad opt key",false,
        &ModuleManager::change_option<double>,&mm,mod_name,not_name,opt);
    tester.test_member_call("get non-existent module",false,
                         &ModuleManager::get_module<TestModule>,&mm,not_name,0);
    tester.test_member_call("get bad parent ID",false,
                         &ModuleManager::get_module<TestModule>,&mm,mod_name,9);
    tester.test_member_call("get not required options ",false,
                      &ModuleManager::get_module<TestModule>,&mm,mod_name,0);
    tester.test_member_call("set required opt",true,
                &ModuleManager::change_option<double>,&mm,mod_name,opt_key,opt);
    tester.test_member_call("get bad type",false,
                      &ModuleManager::get_module<ModuleManager>,&mm,mod_name,0);
    tester.test_member_call("get module",true,
                 &ModuleManager::get_module<TestModule>,&mm,mod_name,0);
    tester.test_member_call("can't change option for used key",false,
                &ModuleManager::change_option<double>,&mm,mod_name,opt_key,opt);
    tester.test_member_call("can't duplicate to existing key",false,
                           &ModuleManager::duplicate_key,&mm,not_name,mod_name);
    tester.test_member_call("can't duplicate non-existing key",false,
                           &ModuleManager::duplicate_key,&mm,not_name,not_name);
    string new_name="new mod name";
    //I don't understand why, but mod_name and not_name are both null now
    mod_name="my module";
    tester.test_member_call("duplicate key",true,
                           &ModuleManager::duplicate_key,&mm,mod_name,new_name);
    string unq_key=mm.generate_unique_key();
    tester.test_member_return("unique key",true,false,
                          &ModuleManager::has_key,&mm,unq_key);
    ModuleCreationFuncs cf;
    cf.add_cpp_creator<CXXModule2>("C++ Module2");
    minfo.name="C++ Module2";
    tester.test_member_call("add module from creation funcs",true,
                     &ModuleManager::load_module_from_mcf,&mm,minfo,unq_key,cf);

    tester.print_results();
    return tester.nfailed();
}
