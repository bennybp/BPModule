#include <pulsar/modulebase/TestModule.hpp>

class CXXModule:public pulsar::TestModule{
public:
    CXXModule(ID_t id):TestModule(id){}
private:
    void run_test_(){

    }
};

extern "C" {

pulsar::ModuleCreationFuncs insert_supermodule(void){
    pulsar::ModuleCreationFuncs cf;
    cf.add_cpp_creator<CXXModule>("C++ Module");
    return cf;
}

}
