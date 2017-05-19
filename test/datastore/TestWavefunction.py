import pulsar as psr
import numpy as np
import sys
import os
sys.path.append(os.path.dirname(__file__))

def run_test():
    tester=psr.PyTester("Testing Wavefunction Python Bindings")

    #Make a system
    MyU = psr.AtomSetUniverse()
    H=psr.create_atom([0.0,0.0,0.0],1)
    H1=psr.create_atom([0.0,0.0,0.89],1)
    MyU.insert(H)
    MyU.insert(H1)
    H2=psr.System(MyU,True)
    Ca_=np.array([[1.0,1.0],[1.0,-1.0]])
    Cb_=Ca_
    rhoa_=np.array([[1.0,0.0],[0.0,0.0]])
    rhob_=np.array([[0.0,0.0],[0.0,1.0]])
    occa_=np.array([1.0,0.0])
    occb_=np.array([0.0,1.0])
    epsa_=np.array([-1.0,1.0])
    epsb_=epsa_
    Ca=psr.EigenMatrixImpl(Ca_)
    Cb=psr.EigenMatrixImpl(Cb_)
    rhoa=psr.EigenMatrixImpl(rhoa_)
    rhob=psr.EigenMatrixImpl(rhob_)
    epa=psr.EigenVectorImpl(epsa_)
    epb=psr.EigenVectorImpl(epsb_)
    occa=psr.EigenVectorImpl(occa_)
    occb=psr.EigenVectorImpl(occb_)

    C,rho = psr.BlockedEigenMatrix(),psr.BlockedEigenMatrix()
    iA = psr.Irrep.A
    sa,sb = psr.Spin.alpha,psr.Spin.beta
    C.set(iA,sa,Ca)
    C.set(iA,sb,Cb)
    rho.set(iA,sa,rhoa)
    rho.set(iA,sb,rhob)

    epsilons,occs = psr.BlockedEigenVector(),psr.BlockedEigenVector()
    epsilons.set(iA,sa,epa)
    epsilons.set(iA,sb,epb)
    occs.set(iA,sa,occa)
    occs.set(iA,sb,occb)

    wf1,wf3 = psr.Wavefunction(),psr.Wavefunction()
    wf1.system=H2
    wf1.cmat=C
    wf1.opdm=rho
    wf1.epsilon=epsilons
    wf1.occupations=occs

    wf2 = psr.Wavefunction(wf1)
    tester.test_equal("Wavefunction copy constructor and equal works",wf1,wf2)
    tester.test_call("valid_check works",True,wf1.valid_check)
    empty_mat=psr.BlockedEigenMatrix()
    empty_vec=psr.BlockedEigenVector()
    wf3.cmat=wf2.cmat
    wf3.opdm=empty_mat
    tester.test_call("valid check inconsistent cmat and opdm",False,wf3.valid_check)
    wf3.opdm=wf2.cmat
    wf3.epsilon=empty_vec
    tester.test_call("valid check inconsistent cmat and epsilon",False,wf3.valid_check)
    wf3.epsilon=wf2.epsilon
    wf3.occupations=empty_vec
    tester.test_call("valid check inconsistent cmat and occupations",False,wf3.valid_check)
    wf3.opdm=empty_mat
    wf3.epsilon=empty_vec;
    tester.test_call("valid check inconsistent opdm and epsilon",False,wf3.valid_check)
    wf3.epsilon=None
    tester.test_call("valid check inconsistent opdm and occupations",False,wf3.valid_check)
    wf3.epsilon=wf2.epsilon
    tester.test_call("valid check inconsistent epsilons and occupations",False,wf3.valid_check)
    tester.test_equal("Hash works",wf1.my_hash(),wf2.my_hash())
    tester.test_not_equal("Wavefunctions are not equal",wf1,wf3)

    tester.print_results()
    return tester.nfailed()

