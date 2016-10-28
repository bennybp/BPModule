from TestFxns import *
import numpy as np

tester = Tester("Testing Blocked Eigen Matrix Python Interface")

Mat1_=np.array([[1.0,2.0,3.0],[4.0,5.0,6.0],[7.0,8.0,9.0]])
Mat2_=np.array([[2.0,4.0,6.0],[8.0,10.0,12.0],[14.0,16.0,18.0]])
iA, iB=psr.Irrep.A , psr.Irrep.B
sa, sb =psr.Spin.alpha, psr.Spin.beta

Mat1=psr.EigenMatrixImpl(Mat1_)
Mat2=psr.EigenMatrixImpl(Mat2_)
M1=psr.BlockedEigenMatrix()
M4=psr.BlockedEigenMatrix()
M1.set(iA,sa,Mat1)
M4.set(iB,sb,Mat2)
tester.test("Set/Get elements works",True,Mat1,M1.get,iA,sa)
M2=psr.BlockedEigenMatrix(M1)
tester.test("Copy constructor works",True,True,M1.__eq__,M2)
tester.test("Not equal works",True,True,M1.__ne__,M4)
tester.test("Has works true",True,True,M4.has,iB,sb)
tester.test("Has works false",True,False,M4.has,iA,sb)
tester.test("Get valid irreps works",True,{iA},M1.get_irreps)
tester.test("Get valid spin works",True,{sb},M4.get_spins,iB)
tester.test("Get invalid spin works",True,set(),M4.get_spins,iA)
tester.test("Get invalid element throws",False,Mat1,M4.get,iA,sb)
tester.test("Same structure different",True,False,M4.same_structure,M1)
tester.test("Same structure same",True,True,M4.same_structure,M4)
M5=psr.BlockedEigenMatrix(M4)
tester.test("Hash works",True,M4.my_hash(),M5.my_hash)

tester.print_results()
