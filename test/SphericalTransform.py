#!/usr/bin/env python3


import os
import sys
import traceback 


# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, psrpath)

import pulsar as psr
from pulsar.output import *
from pulsar.testing import *
from pulsar.system import *
from pulsar.math import *

from helper.TestAtoms import nonane

def CompareList(lst1, lst2, tol):
  if len(lst1) != len(lst2):
    return False

  else:
    for i in range(0, len(lst1)):
        if abs(lst1[i]-lst2[i]) > tol:
            return False

  return True


def Run():
    try:
        tester = Tester("Testing System - Spherical Transformation")
        tester.print_header()

        test_dsds = [ 3.90267020822592436e-03, 0.00000000000000000e+00, 4.10345422739425092e-68,
                      3.48462765316867248e-03, 0.00000000000000000e+00, 3.48462765316867248e-03,
                      0.00000000000000000e+00, 2.09021277528625992e-04, 0.00000000000000000e+00,
                      0.00000000000000000e+00, 8.91417011643743182e-69, 0.00000000000000000e+00,
                      4.10345422739425175e-68, 0.00000000000000000e+00, 2.09021277528625992e-04,
                      2.32062020410676621e-68, 0.00000000000000000e+00, 4.10345422739425175e-68,
                      3.48462765316867248e-03, 0.00000000000000000e+00, 2.32062020410676538e-68,
                      3.90267020822592436e-03, 0.00000000000000000e+00, 3.48462765316867248e-03,
                      0.00000000000000000e+00, 8.91417011643742976e-69, 0.00000000000000000e+00,
                      0.00000000000000000e+00, 2.09021277528625992e-04, 0.00000000000000000e+00, 
                      3.48462765316867248e-03, 0.00000000000000000e+00, 4.10345422739425175e-68,
                      3.48462765316867248e-03, 0.00000000000000000e+00, 3.90267020822592436e-03 ]

        ref_dsds =  [ 6.27063832585877815e-04, 1.54397955489818124e-68, 0.00000000000000000e+00,
                      0.00000000000000000e+00, 0.00000000000000000e+00, 1.54397955489818021e-68,
                      6.27063832585877923e-04, 0.00000000000000000e+00, 2.67425103493122810e-68,
                      0.00000000000000000e+00, 0.00000000000000000e+00, 0.00000000000000000e+00,
                      6.27063832585877923e-04, 0.00000000000000000e+00, 2.67425103493122852e-68,
                      0.00000000000000000e+00, 2.67425103493122769e-68, 0.00000000000000000e+00,
                      6.27063832585877381e-04, 0.00000000000000000e+00, 0.00000000000000000e+00,
                      0.00000000000000000e+00, 2.67425103493122934e-68, 0.00000000000000000e+00,
                      6.27063832585877923e-04 ] 


        test_ssds = [ 1.77164764775880323e+00, 0.00000000000000000e+00, 1.41729965144881537e-65,
                      1.77164764775880323e+00, 0.00000000000000000e+00, 1.77164764775880323e+00 ]

        ref_ssds =  [ 0.00000000000000000e+00, 2.45483500585900873e-65, 0.00000000000000000e+00,
                      0.00000000000000000e+00, 0.00000000000000000e+00 ]

        test_ssfs = [ -3.78261394053485330e-21,  4.11831824290721354e-05, -2.75473864714008605e-05,
                      -1.79727382464451386e-21,  3.58799063279207122e-22, -1.50087162137397470e-21,
                       1.41069782885149165e-04, -3.92666532356309112e-05,  4.90221873741535931e-05,
                      -8.78856613981809096e-05 ]

        ref_ssfs = [ 1.23353981623667479e-05, -2.59400180117856065e-22,  8.47265285953339485e-06,
                     2.26942625038125339e-05,  1.38962279671519032e-21,  1.27219025836033357e-21,
                    -1.38509624080573933e-05 ]

        test_ppfs = [   1.99047877360860140e-49,  0.00000000000000000e+00,  -9.44085979960664555e-18,
                        6.24020620416126068e-50,  0.00000000000000000e+00,  6.24020620416126068e-50,
                        0.00000000000000000e+00,  -7.13393304417208719e-18,  0.00000000000000000e+00,
                        -2.14017991325162646e-17,  0.00000000000000000e+00,  2.84714840292179403e-50,
                        0.00000000000000000e+00,  0.00000000000000000e+00,  -1.15346337771727938e-18,
                        0.00000000000000000e+00,  7.35727608516315535e-50,  0.00000000000000000e+00,
                        2.45242536172105163e-50,  0.00000000000000000e+00,  -8.66424601919588663e-18,
                        0.00000000000000000e+00,  2.84714840292179403e-50,  -2.88808200639862836e-18,
                        0.00000000000000000e+00,  -5.19500876183318750e-18,  0.00000000000000000e+00,
                        2.45242536172105163e-50,  0.00000000000000000e+00,  7.35727608516315535e-50,
                        0.00000000000000000e+00,  2.84714840292179403e-50,  0.00000000000000000e+00,
                        0.00000000000000000e+00,  -1.15346337771727938e-18,  0.00000000000000000e+00,
                        7.35727608516315535e-50,  0.00000000000000000e+00,  2.45242536172105163e-50,
                        0.00000000000000000e+00,  4.00606644215747419e-50,  0.00000000000000000e+00,
                        -7.13393304417208719e-18,  1.73007852191990031e-50,  0.00000000000000000e+00,
                        1.33535548071915791e-50,  0.00000000000000000e+00,  -9.44085979960664555e-18,
                        0.00000000000000000e+00,  -2.14017991325162646e-17,  0.00000000000000000e+00,
                        -2.88808200639862836e-18,  0.00000000000000000e+00,  0.00000000000000000e+00,
                        1.97361520600371234e-51,  0.00000000000000000e+00,  -8.66424601919588663e-18,
                        0.00000000000000000e+00,  -5.19500876183318750e-18,  0.00000000000000000e+00,
                        -8.66424601919588663e-18,  0.00000000000000000e+00,  2.84714840292179403e-50,
                        -2.88808200639862836e-18,  0.00000000000000000e+00,  -5.19500876183318750e-18,
                        0.00000000000000000e+00,  2.45242536172105163e-50,  0.00000000000000000e+00,
                        7.35727608516315535e-50,  0.00000000000000000e+00,  -2.88808200639862836e-18,
                        0.00000000000000000e+00,  0.00000000000000000e+00,  1.97361520600371234e-51,
                        0.00000000000000000e+00,  -8.66424601919588663e-18,  0.00000000000000000e+00,
                        -5.19500876183318750e-18,  0.00000000000000000e+00,  4.00606644215747419e-50,
                        0.00000000000000000e+00,  -1.29100970569693455e-17,  1.33535548071915791e-50,
                        0.00000000000000000e+00,  1.73007852191990031e-50,  0.00000000000000000e+00,
                        -1.29100970569693455e-17,  0.00000000000000000e+00,  -4.56510714372117161e-17 ]

        ref_ppfs =  [ -6.92078026630368126e-18,    9.66870040661399966e-51,    0.00000000000000000e+00,
                       0.00000000000000000e+00,    0.00000000000000000e+00,    4.74778387287989937e-66,
                       0.00000000000000000e+00,   -5.92084561801113140e-51,   -5.65079342478902842e-18,
                       0.00000000000000000e+00,    7.64377882467409937e-51,    0.00000000000000000e+00,
                      -1.54074395550978868e-33,    0.00000000000000000e+00,    0.00000000000000000e+00,
                       0.00000000000000000e+00,   -5.65079342478902919e-18,    0.00000000000000000e+00,
                       7.64377882467409819e-51,    0.00000000000000000e+00,    1.54074395550978868e-33,
                      -5.92084561801113140e-51,   -5.65079342478902842e-18,    0.00000000000000000e+00,
                       7.64377882467409937e-51,    0.00000000000000000e+00,   -1.54074395550978868e-33,
                       0.00000000000000000e+00,    3.46039013315183601e-18,   -7.25152530496052586e-51,
                       0.00000000000000000e+00,   -4.46734445235917841e-18,    0.00000000000000000e+00,
                       9.36167891357127805e-51,    0.00000000000000000e+00,    0.00000000000000000e+00,
                       0.00000000000000000e+00,   -2.41717510165350704e-51,    0.00000000000000000e+00,
                      -4.46734445235917841e-18,    0.00000000000000000e+00,    9.36167891357122107e-51,
                       0.00000000000000000e+00,    0.00000000000000000e+00,   -5.65079342478902919e-18,
                       0.00000000000000000e+00,    7.64377882467409819e-51,    0.00000000000000000e+00,
                       1.54074395550978868e-33,    0.00000000000000000e+00,    0.00000000000000000e+00,
                      -2.41717510165350704e-51,    0.00000000000000000e+00,   -4.46734445235917841e-18,
                       0.00000000000000000e+00,    9.36167891357122107e-51,    3.46039013315183601e-18,
                      -2.41717510165350229e-51,    0.00000000000000000e+00,    4.46734445235917841e-18,
                       0.00000000000000000e+00,   -9.36167891357122582e-51,    0.00000000000000000e+00 ]


        # Load the sph transform data from pulsar
        spt = all_spherical_transforms()

        # Load the ordering
        psrorder = all_ao_orderings()

        # Put in psi4 spherical ordering
        psi4_sphorder = {}
        for l in range(0, 5):
            psi4_sphorder[l] = [0]
            for ml in range(1, l+1):
                psi4_sphorder[l].extend( [ml, -ml] )

        psi4order = AOOrderingMaps(psrorder)
        psi4order.spherical_order = psi4_sphorder

        # Transform the 4 indices, starting with the left
        # and then reorder. Explicitly does s orbitals to test
        bsreorder = make_basis_reorder_map(psrorder, psi4order)

        #########
        # ssds
        #########
        res = Test_spherical_transform_block(spt[0], test_ssds, 6, 0, 1)
        res = Test_spherical_transform_block(spt[0], res, 6, 0, 1)
        res = Test_spherical_transform_block(spt[2], res, 1, 2, 1)
        res = Test_spherical_transform_block(spt[0], res, 1, 0, 5)

        res = Test_Reorder(bsreorder[ShellType.SphericalGaussian][0], res, 5, 1)
        res = Test_Reorder(bsreorder[ShellType.SphericalGaussian][2], res, 1, 1)
        res = Test_Reorder(bsreorder[ShellType.SphericalGaussian][0], res, 1, 5)
        res = Test_Reorder(bsreorder[ShellType.SphericalGaussian][0], res, 1, 5)

        tester.test_value("Testing ssds", True, CompareList(res, ref_ssds, 1e-18))

        #########
        # ssfs
        #########
        res = Test_spherical_transform_block(spt[0], test_ssfs, 10, 0, 1)
        res = Test_spherical_transform_block(spt[0], res, 10, 0, 1)
        res = Test_spherical_transform_block(spt[3], res, 1, 3, 1)
        res = Test_spherical_transform_block(spt[0], res, 1, 0, 7)

        res = Test_Reorder(bsreorder[ShellType.SphericalGaussian][0], res, 7, 1)
        res = Test_Reorder(bsreorder[ShellType.SphericalGaussian][0], res, 7, 1)
        res = Test_Reorder(bsreorder[ShellType.SphericalGaussian][3], res, 1, 1)
        res = Test_Reorder(bsreorder[ShellType.SphericalGaussian][0], res, 1, 7)

        tester.test_value("Testing ssfs", True, CompareList(res, ref_ssfs, 1e-18))

        #########
        # dsds
        #########
        res = Test_spherical_transform_block(spt[2], test_dsds, 6, 2, 1) 
        res = Test_spherical_transform_block(spt[0], res, 6, 0, 5) 
        res = Test_spherical_transform_block(spt[2], res, 1, 2, 5)
        res = Test_spherical_transform_block(spt[0], res, 1, 0, 25) 

        res = Test_Reorder(bsreorder[ShellType.SphericalGaussian][2], res, 5, 1)
        res = Test_Reorder(bsreorder[ShellType.SphericalGaussian][0], res, 5, 5)
        res = Test_Reorder(bsreorder[ShellType.SphericalGaussian][2], res, 1, 5)
        res = Test_Reorder(bsreorder[ShellType.SphericalGaussian][0], res, 1, 25)
        tester.test_value("Testing dsds", True, CompareList(res, ref_dsds, 1e-18))

        #########
        # ppfs
        #########
        res = Test_spherical_transform_block(spt[1], test_ppfs, 30, 1, 1) 
        res = Test_spherical_transform_block(spt[1], res, 10, 1, 3)
        res = Test_spherical_transform_block(spt[3], res, 1, 3, 9)
        res = Test_spherical_transform_block(spt[0], res, 1, 0, 63)

        res = Test_Reorder(bsreorder[ShellType.SphericalGaussian][1], res, 21, 1)
        res = Test_Reorder(bsreorder[ShellType.SphericalGaussian][1], res, 7,  3)
        res = Test_Reorder(bsreorder[ShellType.SphericalGaussian][3], res, 1,  9)
        res = Test_Reorder(bsreorder[ShellType.SphericalGaussian][0], res, 1,  63)
        tester.test_value("Testing ppfs", True, CompareList(res, ref_ppfs, 1e-18))



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
