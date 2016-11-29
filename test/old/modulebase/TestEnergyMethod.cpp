#include<pulsar/testing/CppTester.hpp>
#include<pulsar/modulebase/Test_Base.hpp>
#include <pulsar/modulebase/EnergyMethod.hpp>

using namespace pulsar;
using std::to_string;

//A simple harmonic 3*Natoms dimensional harmonic oscillator with k=1
class FakeEnergyMethod : public EnergyMethod{
    public:
        //Uses constructor of base class
        using EnergyMethod::EnergyMethod;
        ///Returns the \p Order -th derivative of the MBE of system in \p wfn
        DerivReturnType deriv_(size_t Order,const Wavefunction& wfn){
            const System& Mol=*wfn.system;
            std::vector<double> egy({0.0});
            for(const pulsar::Atom& Ai: Mol)
                egy[0]+=0.5*(Ai[0]*Ai[0]+Ai[1]*Ai[1]+Ai[2]*Ai[2]);
            return {wfn,egy};
        }

};

class TestEnergyMethod : public Test_CXX_Base {
protected:
        virtual size_t run_test_(void);
public:
    TestEnergyMethod(ID_t id) : Test_CXX_Base(id) { }
};

size_t TestEnergyMethod::run_test_(){
    CppTester tester("Testing EnergyMethod module base type");
 
    ModuleManager& mm=module_manager();
    OptionMap om;
    const auto& none=static_cast<pybind11::object>(pybind11::none());
    om.add_option("MAX_DERIV",OptionType::Int,false,none,"",pybind11::cast(0));
    om.add_option("FDIFF_DISPLACEMENT",OptionType::Float,false,none,"",pybind11::cast(0.005));
    om.add_option("FDIFF_STENCIL_SIZE",OptionType::Int,false,none,"",pybind11::cast(3));
    
    ModuleInfo minfo={"FakeEnergyMethod","c_module","EnergyMethod",
        "./TestEnergyMethod.so","1.0","A 3*Natoms H.O.",{""},{""}, om
    };
    mm.load_module_from_minfo(minfo,"Test the H.O.");
    auto egy_mod=create_child<EnergyMethod>("Test the H.O.");
    
    Wavefunction wfn;
    Atom H=create_atom({0.0,0.0,0.0},1),H1=create_atom({0.0,0.0,0.89},1);
    AtomSetUniverse MyU;
    MyU.insert(H);
    MyU.insert(H1);
    wfn.system=std::make_shared<const System>(System(MyU,true));
    
    //The right answers
    const std::vector<double> egy({0.39605});
    std::vector<double> grad(6,0.0);grad[5]=0.89;
    std::vector<double> hess(36,0.0);
    hess[0]=hess[7]=hess[14]=hess[21]=hess[28]=hess[35]=1.0;
    
    auto deriv=egy_mod->deriv(0,wfn);
    tester.test_equal("Energy works",egy,deriv.second);
    
    deriv=egy_mod->deriv(1,wfn);
    tester.test_equal("Grad has right dimensions",grad.size(),deriv.second.size());
    for(size_t i=0;i<grad.size();++i)
        tester.test_equal("FDiff grad comp "+to_string(i),grad[i],deriv.second[i]);
    
    deriv=egy_mod->deriv(2,wfn);
    tester.test_equal("Hessian has right dimensions",hess.size(),deriv.second.size());
    for(size_t i=0;i<hess.size();++i)
        tester.test_equal("FDiff Hessian comp "+to_string(i),hess[i],deriv.second[i]);
    
    tester.print_results();
    return tester.nfailed();
}

extern "C" {
ModuleCreationFuncs insert_supermodule(void){
    ModuleCreationFuncs cf;
    cf.add_cpp_creator<TestEnergyMethod>("TestEnergyMethod");
    cf.add_cpp_creator<FakeEnergyMethod>("FakeEnergyMethod");
    return cf;
}}
