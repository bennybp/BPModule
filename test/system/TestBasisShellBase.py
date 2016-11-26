import pulsar as psr
import time

def run_test():
    BSI = psr.BasisShellInfo
    BSB = psr.BasisShellBase
    cGTO = psr.ShellType.CartesianGaussian
    sGTO = psr.ShellType.SphericalGaussian
    tester = psr.PyTester("Testing BasisShellBase (BSB)")

    alpha =[3.42525091, 0.62391373, 0.16885540]
    c=[0.15432897, 0.53532814, 0.44463454]
    c2=[]

    FakeD=BSI(cGTO, 2, 3, 1, alpha, c)
    FakeD2=BSI(sGTO, 2, 3, 1, alpha, c)

    tester.test_return("get type works", True, cGTO, FakeD.get_type)
    tester.test_return("get am works", True, 2, FakeD2.am)
    tester.test_return("number of prims", True, 3, FakeD2.n_primitives)
    tester.test_return("number of ceofs", True, 3, FakeD.n_coefficients)
    tester.test_return("number of gen contract", True, 1, FakeD2.n_general_contractions)
    tester.test_return("number of functions Cart", True, 6, FakeD.n_functions)
    tester.test_return("number of functions Spherical", True, 5, FakeD2.n_functions)
    tester.test_return("is combined am", True, False, FakeD2.is_combined_am)
    tester.test_return("get valid general am", True, 2, FakeD2.general_am, 0)
    tester.test_return("get invalid general am", False, 3, FakeD2.general_am, 99)
    tester.test_return("general n fxns valid", True, 6, FakeD.general_n_functions, 0)
    tester.test_return("general n fxns invalid", False, 6, FakeD.general_n_functions, 99)
    tester.test_return("get valid alpha", True, alpha[1], FakeD.alpha, 1)
    tester.test_return("get invalid alpha", False, 0.4, FakeD.alpha, 99)
    tester.test_return("get valid coef", True, c[1], FakeD2.coef, 0, 1)
    tester.test_return("get invalid coef", False, 0.2, FakeD2.coef, 0, 99)
    tester.test_return("get invalid gen coef", False, 0.2, FakeD2.coef, 99, 1)
    tester.test_return("get valid alpha safe", True, alpha[1], FakeD2.get_alpha, 1)
    tester.test_return("get invalid alpha safe", False, 0.0, FakeD2.get_alpha, 99)
    tester.test_return("set valid alpha safe",True,None,FakeD.set_alpha,0, 3.14)
    tester.test_return("set valid alpha safe", True, 3.14, FakeD.get_alpha,0)
    tester.test_return("set invalid alpha safe", False, None, FakeD.set_alpha,99,3.14)
    tester.test_return("get valid coef safe", True, c[1], FakeD2.get_coef, 0, 1)
    tester.test_return("get invalid gen coef safe", False, 0.0, FakeD2.get_coef, 99, 1)
    tester.test_return("get invalid coef safe", False, 0.0, FakeD2.get_coef, 0, 99)
    tester.test_return("set valid coef safe",True,None,FakeD.set_coef,0, 0, 3.14)
    tester.test_return("set valid coef safe", True, 3.14, FakeD.get_coef,0,0)
    tester.test_return("set invalid gen coef safe",False,None,FakeD.set_coef,99,1,3.14)
    tester.test_return("set invalid coef safe",False,None,FakeD.set_coef,0,99,3.14)
    tester.test_return("set all alpha",True,None,FakeD.set_alphas,alpha)
    tester.test_return("safe get all alpha",True,alpha,FakeD.get_alphas)
    tester.test_return("safe set all coef",True,None,FakeD.set_coefs,0,c)
    tester.test_return("safe get/set all coef",True,c,FakeD.get_coefs,0)
    tester.test_return("safe set all invalid coefs",False,None,FakeD.set_coefs,99,c)
    tester.test_return("safe set all gen coefs",True,None,FakeD2.set_all_coefs,c)
    tester.test_return("safe get all gen coefs",True,c,FakeD2.get_all_coefs);
    tester.test_return("safe set all invalid coefs",False,None,FakeD.set_all_coefs,c2)
    tester.test_return("set primitive works",True,None,FakeD2.set_primitive,0,3.14,3.14)
    tester.test_return("set primitive works alpha",True,3.14,FakeD2.get_alpha,0)
    tester.test_return("set primitive works coef",True,3.14,FakeD2.get_coef,0,0)
    tester.test_return("safe invalid primitive",False, None, FakeD.set_primitive,99,3.14,3.14)
    pi=[3.14]
    tester.test_return("set gen primitive works",True,None,FakeD2.set_primitive,0,3.14,pi)
    tester.test_return("set gen primitive works alpha", True,3.14, FakeD2.get_alpha,0)
    tester.test_return("set gen primitive works coef",True, 3.14, FakeD2.get_coef,0,0)
    tester.test_return("safe gen primitive",False,None,FakeD.set_primitive,99,3.14,pi)
    tester.test_return("safe invalid gen primitive",False,None,FakeD.set_primitive,0,3.14,c2)
    FakeD2=FakeD
    tester.test_return("hash", True, FakeD.my_hash(), FakeD2.my_hash)



    tester.print_results()
    return tester.nfailed()
