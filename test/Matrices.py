#!/usr/bin/env python3

import os
import sys
import traceback 
import operator

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, psrpath)

import pulsar as psr
from pulsar.output import *
from pulsar.testing import *
from pulsar.math import *

from helper.SetOperations import TestSetOperations


def TestMatrix(tester, mattype, pytype, testserial):
    mat = [ pytype(1), pytype(2), pytype(3),
            pytype(4), pytype(5), pytype(6) ]


    sf = mattype(2, 3, mat)

    idx = 0 
    for i in range(0, 2):
      for j in range(0, 3):
        tester.TestValue("Getting values - {} {}".format(i, j), sf[i, j], mat[idx])
        sf[i,j] = 10*idx
        tester.TestValue("Setting values - {} {}".format(i, j), sf[i, j], 10*idx)
        idx += 1

    # Comparison
    sf2 = mattype(sf)
    tester.TestValue("Comparison of copied matrices", True, sf == sf2)
    sf = mattype(2, 3, mat)
    sf2 = mattype(2, 3, mat)
    tester.TestValue("Comparison of equal matrices", True, sf == sf2)
    sf2[0, 0] += 1
    tester.TestValue("Comparison after changing", False, sf == sf2)
    

    # Creating with invalid values
    mat = [ 1, 2, 3 ]
    tester.Test("Creating with invalid length", False, mattype, 2, 3, mat)


    # Serialization
    tester.Test("Serialization", True, testserial, sf)


def TestIrrepSpinMatrix(tester, mattype, ismattype, pytype, testserial):
    
    mat1 = [ pytype(1), pytype(2), pytype(3),
             pytype(4), pytype(5), pytype(6) ]
    mat2 = [ pytype(-1), pytype(-2), pytype(-3),
             pytype(-4), pytype(-5), pytype(-6) ]

    ismat1 = ismattype()
    ismat1.Set(Irrep.A1, 0, mattype(2, 3, mat1))
    ismat1.Set(Irrep.A2, 1, mattype(2, 3, mat2))

    ismat2 = ismattype()
    ismat2.Set(Irrep.A2, 1, mattype(2, 3, mat2))
    ismat2.Set(Irrep.A1, 0, mattype(2, 3, mat1))

    tester.TestValue("Comparison of equal irrep spin matrices", True, ismat1 == ismat2)

    sf1 = mattype(2, 3, mat1)
    sf2 = mattype(2, 3, mat2)

    ismat3 = ismattype(ismat1)
    tester.TestValue("Comparison of copied irrep spin matrices", True, ismat3 == ismat2)

    ismat3.Set(Irrep.E1, 0, mattype(2, 3, mat2))
    tester.TestValue("Comparison of modified irrep spin matrices", False, ismat3 == ismat2)

    ismat3 = ismattype(ismat1)
    ismat3.Set(Irrep.A2, 1, mattype(2, 3, mat1))
    tester.TestValue("Comparison of modified irrep spin matrices", False, ismat3 == ismat2)

    tester.TestValue("Getting from irrep spin mat", True, ismat1.Get(Irrep.A1, 0) == sf1)
    tester.TestValue("Getting from irrep spin mat", True, ismat2.Get(Irrep.A1, 0) == sf1)
    tester.TestValue("Getting from irrep spin mat", True, ismat1.Get(Irrep.A2, 1) == sf2)
    tester.TestValue("Getting from irrep spin mat", True, ismat2.Get(Irrep.A2, 1) == sf2)

    tester.Test("Getting unset matrix", False, ismat1.Get, Irrep.E1, 0)
    tester.Test("Getting unset matrix", False, ismat1.Get, Irrep.E1, 1)
    tester.Test("Getting unset matrix", False, ismat1.Get, Irrep.E2, 1)
    tester.Test("Getting unset matrix", False, ismat2.Get, Irrep.E1, 0)
    tester.Test("Getting unset matrix", False, ismat2.Get, Irrep.E1, 1)
    tester.Test("Getting unset matrix", False, ismat2.Get, Irrep.E2, 1)

    ismat4 = ismattype()
    tester.TestValue("Comparison of empty irrep spin matrices", True, ismat4 == ismat4)

    tester.Test("Serialization of IrrepSpinMatrix", True, testserial, ismat1)
    

def Run():
    try:
        tester = Tester("Testing SimpleMatrix")
        tester.PrintHeader()

        TestMatrix(tester, SimpleMatrixF, float, TestSerialization_SimpleMatrixF)
        TestMatrix(tester, SimpleMatrixD, float, TestSerialization_SimpleMatrixD)
        TestIrrepSpinMatrix(tester, SimpleMatrixF, IrrepSpinMatrixF, float, TestSerialization_IrrepSpinMatrixF)
        TestIrrepSpinMatrix(tester, SimpleMatrixD, IrrepSpinMatrixD, float, TestSerialization_IrrepSpinMatrixD)
        

        tester.PrintResults()


    except Exception as e:
      GlobalOutput("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      GlobalError("\n")
      GlobalError(str(e))
      GlobalError("\n")




psr.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
psr.Finalize()
