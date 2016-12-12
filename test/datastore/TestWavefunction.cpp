#include <pulsar/testing/CppTester.hpp>
#include <pulsar/datastore/Wavefunction.hpp>

using namespace pulsar;

TEST_SIMPLE(TestWavefunction){
    CppTester tester("Testing Wavefunction class");

    //Make a system
    AtomSetUniverse MyU;
    Atom H=create_atom({0.0,0.0,0.0},1), H1=create_atom({0.0,0.0,0.89},1);
    MyU.insert(H);
    MyU.insert(H1);
    System H2(MyU,true);

    Eigen::MatrixXd Ca_(2,2),Cb_(2,2),rhoa_(2,2),rhob_(2,2);
    Eigen::VectorXd occa_(2),occb_(2),epsa_(2),epsb_(2);
    //The below should be consistent with using the actual 1s orbitals
    Ca_<<1.0,1.0,
            1.0,-1.0;
    Ca_/=sqrt(2.0);
    Cb_=Ca_;
    rhoa_<<1.0,0.0,
             0.0,0.0;
    rhob_<<0.0,0.0,
             0.0,1.0;
    occa_<<1.0,0.0;
    occb_=occa_;
    epsa_<<-1.0,1.0;//I don't know the real answer...
    epsb_=epsa_;

    auto Ca=std::make_shared<EigenMatrixImpl>(Ca_);
    auto Cb=std::make_shared<EigenMatrixImpl>(Cb_);
    auto rhoa=std::make_shared<EigenMatrixImpl>(rhoa_);
    auto rhob=std::make_shared<EigenMatrixImpl>(rhob_);
    auto epa=std::make_shared<EigenVectorImpl>(epsa_);
    auto epb=std::make_shared<EigenVectorImpl>(epsb_);
    auto occa=std::make_shared<EigenVectorImpl>(occa_);
    auto occb=std::make_shared<EigenVectorImpl>(occb_);

    IrrepSpinMatrixD C,rho;
    Irrep iA=Irrep::A;
    Spin sa=Spin::alpha,sb=Spin::beta;
    C.set(iA,sa,Ca);C.set(iA,sb,Cb);
    rho.set(iA,sa,rhoa);rho.set(iA,sb,rhob);

    IrrepSpinVectorD epsilons,occs;
    epsilons.set(iA,sa,epa);epsilons.set(iA,sb,epb);
    occs.set(iA,sa,occa);occs.set(iA,sb,occb);

    Wavefunction wf1,wf3;
    wf1.system=std::make_shared<const System>(H2);
    wf1.cmat=std::make_shared<const IrrepSpinMatrixD>(C);
    wf1.opdm=std::make_shared<const IrrepSpinMatrixD>(rho);
    wf1.epsilon=std::make_shared<const IrrepSpinVectorD>(epsilons);
    wf1.occupations=std::make_shared<const IrrepSpinVectorD>(occs);

    Wavefunction wf2(wf1);
    tester.test_equal("Wavefunction copy constructor and equal works",wf1,wf2);
    tester.test_member_call("valid_check works",true,&Wavefunction::valid_check,&wf1);
    auto empty_mat=std::make_shared<const IrrepSpinMatrixD>();
    auto empty_vec=std::make_shared<const IrrepSpinVectorD>();
    wf3.cmat=wf2.cmat;
    wf3.opdm=empty_mat;
    tester.test_member_call("valid check inconsistent cmat and opdm",false,&Wavefunction::valid_check,&wf3);
    wf3.opdm=nullptr;
    wf3.epsilon=empty_vec;
    tester.test_member_call("valid check inconsistent cmat and epsilon",false,&Wavefunction::valid_check,&wf3);
    wf3.epsilon=nullptr;
    wf3.occupations=empty_vec;
    tester.test_member_call("valid check inconsistent cmat and occupations",false,&Wavefunction::valid_check,&wf3);
    wf3.opdm=wf2.opdm;
    wf3.epsilon=empty_vec;
    tester.test_member_call("valid check inconsistent opdm and epsilon",false,&Wavefunction::valid_check,&wf3);
    wf3.epsilon=nullptr;
    tester.test_member_call("valid check inconsistent opdm and occupations",false,&Wavefunction::valid_check,&wf3);
    wf3.epsilon=wf2.epsilon;
    tester.test_member_call("valid check inconsistent epsilons and occupations",false,&Wavefunction::valid_check,&wf3);
    tester.test_equal("Hash works",wf1.my_hash(),wf2.my_hash());
    tester.test_not_equal("Wavefunctions are not equal",wf1,wf3);

    tester.print_results();
    return tester.nfailed();
}
