#include <pulsar/testing/CppTester.hpp>
#include <pulsar/modulemanager/ModuleManager.hpp>
#include <pulsar/modulebase/SystemFragmenter.hpp>


using namespace std;
using namespace pulsar;

class TestFragger:public SystemFragmenter{
public:
    TestFragger(ID_t id):SystemFragmenter(id){}
private:
    NMerSetType fragmentize_(const System& sys){
        NMerSetType temp({{{0},{{0},sys,1.0}}});
        return temp;
    }
};

TEST_SIMPLE(TestSystemFragmenter){
    CppTester tester("Testing SystemFragmenter base class");

    auto mm=make_shared<ModuleManager>();
    mm->load_lambda_module<TestFragger>("TestFragger","Test_fragger");

    auto fragger=mm->get_module<SystemFragmenter>("Test_fragger",0);
    AtomSetUniverse MyU;
    Atom H=create_atom({0.0,0.0,0.0},1),H1=create_atom({0.0,0.0,0.89},1);
    MyU.insert(H);
    MyU.insert(H1);
    System H2(MyU,true);
    NMerSetType corr_answer={{{0},{{0},H2,1.0}}};
    tester.test_member_return("Can call fragmentize",true,corr_answer,
        &SystemFragmenter::fragmentize,&(*fragger),H2);
    tester.print_results();
    return tester.nfailed();
}
