from TestFxns import *
import numpy as np

def run_test():
    tester = PyTester("Testing Eigen Matrix Vector Python Interface")
    
    Mat1=np.array([[1.0,2.0,3.0,4.0],[5.0,6.0,7.0,8.0]])
    Mat2=np.array([[2.0,3.0,4.0,5.0],[6.0,7.0,8.0,9.0]])
    M1=psr.EigenMatrixImpl(Mat1)
    tester.test_function("Matrix constructor works",True,True,np.array_equal,Mat1,M1.get_matrix())
    M2=psr.EigenMatrixImpl(Mat2)
    tester.test_function("Matrix sizes works",True,[2,4],M1.sizes)
    tester.test_function("Matrix get_value works",True,3.0,M1.get_value,[0,2])
    M1.set_value([0,2],55.0)
    tester.test_function("Matrix set_value works",True,55.0,M1.get_value,[0,2])
    tester.test_function("Matrix != works",True,True,M1.__ne__,M2)
    Mat4=M1.get_matrix()
    tester.test_function("Not an alias of numpy array",True,False,np.array_equal,Mat1,Mat4)
    M2=psr.EigenMatrixImpl(M1)
    tester.test_function("Matrix Hash Value",True,M1.my_hash(),M2.my_hash)
    
    
    Vec1=np.array([1.0,2.0,3.0,4.0])
    Vec2=np.array([2.0,3.0,4.0,5.0])
    V1=psr.EigenVectorImpl(Vec1)
    tester.test_function("Vector constructor works",True,True,np.array_equal,Vec1,V1.get_matrix())
    V2=psr.EigenVectorImpl(Vec2)
    tester.test_function("Vector sizes works",True,[4],V1.sizes)
    tester.test_function("Vector get_value works",True,3.0,V1.get_value,[2])
    V1.set_value([2],55.0)
    tester.test_function("Vector set_value works",True,55.0,V1.get_value,[2])
    tester.test_function("Vector != works",True,True,V1.__ne__,V2)
    Vec4=V1.get_matrix()
    tester.test_function("Not an alias of numpy array",True,False,np.array_equal,Vec1,Vec4)
    V2=psr.EigenVectorImpl(V1)
    tester.test_function("Vector Hash Value",True,V1.my_hash(),V2.my_hash)
    
    tester.print_results();
    return tester.nfailed()
