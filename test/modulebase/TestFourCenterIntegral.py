import pulsar as psr
import numpy as np

Mat1 = np.array([1.1,2.2,3.3])

class Test4CInt(psr.FourCenterIntegral):
    def __init__(self,id):
        super(Test4CInt,self).__init__(id)

    def initialize_(self,deriv,wf,bs1,bs2,bs3,bs4):
        return

    def calculate_(self,shell1,shell2,shell3,shell4):
        return Mat1

def run_test():
    tester=psr.PyTester("Testing FourCenterIntegral Python Bindings")
    mm=psr.ModuleManager()
    mm.load_lambda_module(Test4CInt,"IntBuilder","test_builder")
    builder=mm.get_module("test_builder",0)
    deriv=1
    wf=psr.Wavefunction()
    bs=psr.BasisSet(1,3,3,3)
    tester.test_call("Can call initialize",True,builder.initialize,deriv,wf,bs,bs,bs,bs)
    temp = builder.calculate(deriv,deriv,deriv,deriv).tolist()
    tester.test_double_vector("Can call calculate",Mat1,temp)

    tester.print_results()
    return tester.nfailed()
