#!/usr/bin/env python3

import os
import sys
import traceback 
import operator
from random import shuffle

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, psrpath)

import pulsar as psr
from pulsar.output import *
from pulsar.testing import *
from pulsar.math import *


def Run():
    try:
        tester = Tester("Testing Other Math Functionality")
        tester.print_header()

        ####################
        # Test Reordering
        ####################
        alist = [ x for x in range(-10, 11) ]
        blist = list(alist)
        order = [ alist.index(x) for x in blist ]

        frompsr = Test_MakeOrdering(alist, blist)
        tester.test_value("Test trivial ordering", True, frompsr == order)

        newlist = Test_Reorder(order, alist, 1, 1)
        tester.test_value("Test trivial reordering", True, newlist == blist)

        for i in range(0, 10):
            shuffle(blist)
            order = [ alist.index(x) for x in blist ]
            frompsr = Test_MakeOrdering(alist, blist)
            tester.test_value("Test shuffled order {}".format(i), True,
                             frompsr == order)

            newlist = Test_Reorder(order, alist, 1, 1)
            tester.test_value("Test shuffled reordering", True, newlist == blist)


        alist = [ 10.0, 20.0, 30.0, 40.0, 50.0,
                  11.0, 21.0, 31.0, 41.0, 51.0,
                  12.0, 22.0, 32.0, 42.0, 52.0,
                  13.0, 23.0, 33.0, 43.0, 53.0,
                  14.0, 24.0, 34.0, 44.0, 54.0,
                  15.0, 25.0, 35.0, 45.0, 55.0 ]

        blist = [ 13.0, 23.0, 33.0, 43.0, 53.0,
                  15.0, 25.0, 35.0, 45.0, 55.0,
                  12.0, 22.0, 32.0, 42.0, 52.0,
                  10.0, 20.0, 30.0, 40.0, 50.0,
                  11.0, 21.0, 31.0, 41.0, 51.0,
                  14.0, 24.0, 34.0, 44.0, 54.0 ]

        order = [ 3, 5, 2, 0, 1, 4 ]
        newlist = Test_Reorder(order, alist, 5, 1)
        tester.test_value("Test reorder with width", True, newlist == blist)


        blist = [ 20.0, 10.0, 40.0, 30.0, 50.0, 
                  21.0, 11.0, 41.0, 31.0, 51.0, 
                  22.0, 12.0, 42.0, 32.0, 52.0, 
                  23.0, 13.0, 43.0, 33.0, 53.0, 
                  24.0, 14.0, 44.0, 34.0, 54.0, 
                  25.0, 15.0, 45.0, 35.0, 55.0 ]

        order = [ 1, 0, 3, 2, 4 ]
        newlist = Test_Reorder(order, alist, 1, 6)
        tester.test_value("Test reorder with niter", True, newlist == blist)

        alist = [ 10.0, 20.0, 30.0, 40.0, 50.0,
                  11.0, 21.0, 31.0, 41.0, 51.0,
                  12.0, 22.0, 32.0, 42.0, 52.0,
                  13.0, 23.0, 33.0, 43.0, 53.0,
                  14.0, 24.0, 34.0, 44.0, 54.0,
                  15.0, 25.0, 35.0, 45.0, 55.0 ]

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
