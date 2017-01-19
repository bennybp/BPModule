#include <pulsar/testing/CppTester.hpp>
#include <pulsar/modulemanager/Checkpoint.hpp>
#include <pulsar/modulemanager/checkpoint_backends/BDBCheckpointIO.hpp>
#include <pulsar/modulebase/TestModule.hpp>

using namespace std;
using namespace pulsar;
/* Small module for testing checkpoint.  On the first call to run_test it puts
 * a double into the cache.  On the second call
 */
struct CXXModule:public TestModule{
    CXXModule(ID_t id):TestModule(id){}
    static bool was_called;
    void run_test_(){
        auto data=cache().get<double>("Some data",false);
        if(data)
        {
            if(!was_called)
                throw PulsarException("Data shouldn't be there");
            else if(std::fabs(*data-2.1)>1e-5)
                throw PulsarException("Data isn't right");
        }
        else if(was_called)
            throw PulsarException("Data should be here");

        cache().set("Some data",2.1,CacheData::CachePolicy::CheckpointLocal);
        was_called=true;
    }
};

bool CXXModule::was_called=false;

TEST_SIMPLE(TestCheckpoint){
    CppTester tester("Testing the Checkpoint class");
    for(size_t i=0;i<2;++i)
    {
        Checkpoint mychk(make_shared<BDBCheckpointIO>("local"),
                         make_shared<BDBCheckpointIO>("global"));
        auto pmm=make_shared<ModuleManager>();
        ModuleManager& mm=*pmm;
        ModuleInfo minfo;
        minfo.name="C++ Module";

        ModuleCreationFuncs mcf;
        mcf.add_cpp_creator<CXXModule>(minfo.name);
        mm.load_module_from_mcf(minfo,"Module",mcf);
        if(i==1)mychk.load_local_cache(mm);
        auto my_mod=mm.get_module<TestModule>("Module",0);
        tester.test_member_call("call "+to_string(i),true,
                                &TestModule::run_test,my_mod.operator->());
        if(i==0)mychk.save_local_cache(mm);
    }
    tester.print_results();
    return tester.nfailed();
}

