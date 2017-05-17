import pulsar as psr
import numpy as np

Mat1 = [psr.EigenMatrixImpl(np.array([[1.0,0.0,0.0],[0.0,1.0,0.0],[0.0,0.0,1.0]]))]

class TestMatrix(psr.MatrixBuilder):
    def __init__(self,id):
        super(TestMatrix,self).__init__(id)

    def calculate_(self,key,deriv,wf,bs1,bs2):
        return Mat1
    def my_hash_(self,key,deriv,wf,bs1,bs2):
        return ""

def run_test():
    tester=psr.PyTester("Testing MatrixBuilder Python Bindings")
    mm=psr.ModuleManager()
    mm.load_lambda_module(TestMatrix,"TestBuilder","test_builder")
    builder=mm.get_module("test_builder",0)
    key="a string"
    deriv=1
    wf=psr.Wavefunction()
    bs=psr.BasisSet(1,3,3,3)
    tester.test_return("Can call calculate",True,Mat1,builder.calculate,key,deriv,wf,bs,bs)
    tester.print_results()
    return tester.nfailed()
