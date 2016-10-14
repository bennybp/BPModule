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

from helper.SetOperations import test_set_operations


def TestMatrix(tester, mattype, pytype, testserial):
    mat = [ pytype(1), pytype(2), pytype(3),
            pytype(4), pytype(5), pytype(6) ]


    sf = mattype(2, 3, mat)

    idx = 0 
    for i in range(0, 2):
      for j in range(0, 3):
        tester.test_value("Getting values - {} {}".format(i, j), sf[i, j], mat[idx])
        sf[i,j] = 10*idx
        tester.test_value("Setting values - {} {}".format(i, j), sf[i, j], 10*idx)
        idx += 1

    # Comparison
    sf2 = mattype(sf)
    tester.test_value("Comparison of copied matrices", True, sf == sf2)
    sf = mattype(2, 3, mat)
    sf2 = mattype(2, 3, mat)
    tester.test_value("Comparison of equal matrices", True, sf == sf2)
    sf2[0, 0] += 1
    tester.test_value("Comparison after changing", False, sf == sf2)
    

    # Creating with invalid values
    mat = [ 1, 2, 3 ]
    tester.test("Creating with invalid length", False, mattype, 2, 3, mat)


    # Serialization
    tester.test("Serialization", True, testserial, sf)


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

    tester.test_value("Comparison of equal irrep spin matrices", True, ismat1 == ismat2)

    sf1 = mattype(2, 3, mat1)
    sf2 = mattype(2, 3, mat2)

    ismat3 = ismattype(ismat1)
    tester.test_value("Comparison of copied irrep spin matrices", True, ismat3 == ismat2)

    ismat3.Set(Irrep.E1, 0, mattype(2, 3, mat2))
    tester.test_value("Comparison of modified irrep spin matrices", False, ismat3 == ismat2)

    ismat3 = ismattype(ismat1)
    ismat3.Set(Irrep.A2, 1, mattype(2, 3, mat1))
    tester.test_value("Comparison of modified irrep spin matrices", False, ismat3 == ismat2)

    tester.test_value("Getting from irrep spin mat", True, ismat1.Get(Irrep.A1, 0) == sf1)
    tester.test_value("Getting from irrep spin mat", True, ismat2.Get(Irrep.A1, 0) == sf1)
    tester.test_value("Getting from irrep spin mat", True, ismat1.Get(Irrep.A2, 1) == sf2)
    tester.test_value("Getting from irrep spin mat", True, ismat2.Get(Irrep.A2, 1) == sf2)

    tester.test("Getting unset matrix", False, ismat1.Get, Irrep.E1, 0)
    tester.test("Getting unset matrix", False, ismat1.Get, Irrep.E1, 1)
    tester.test("Getting unset matrix", False, ismat1.Get, Irrep.E2, 1)
    tester.test("Getting unset matrix", False, ismat2.Get, Irrep.E1, 0)
    tester.test("Getting unset matrix", False, ismat2.Get, Irrep.E1, 1)
    tester.test("Getting unset matrix", False, ismat2.Get, Irrep.E2, 1)

    ismat4 = ismattype()
    tester.test_value("Comparison of empty irrep spin matrices", True, ismat4 == ismat4)

    tester.test("Serialization of IrrepSpinMatrix", True, testserial, ismat1)
    

def Run():
    try:
        tester = Tester("Testing SimpleMatrix")
        tester.print_header()

        TestMatrix(tester, SimpleMatrixF, float, TestSerialization_SimpleMatrixF)
        TestMatrix(tester, SimpleMatrixD, float, TestSerialization_SimpleMatrixD)
        TestIrrepSpinMatrix(tester, SimpleMatrixF, IrrepSpinMatrixF, float, TestSerialization_IrrepSpinMatrixF)
        TestIrrepSpinMatrix(tester, SimpleMatrixD, IrrepSpinMatrixD, float, TestSerialization_IrrepSpinMatrixD)
        

        tester.print_results()


    except Exception as e:
      print_global_output("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      print_global_error("\n")
      print_global_error(str(e))
      print_global_error("\n")




psr.initialize(sys.argv, out = "stdout", color = True, debug = True)
Run()
psr.finalize()
