#!/usr/bin/env python3


import os
import sys
import argparse
import copy
import traceback 

sys.path.insert(0, "/home/ben/programming/BPModule/install/modules")

import bpmodule as bp


def Run():
    try:
        bp.output.Output("\n")
        bp.output.Output(bp.output.Line('-'))

        # The limits on various integer types
        intlimitdict = { "sshort"             : bp.testing.Limits_sshort(),
                         "ushort"             : bp.testing.Limits_ushort(),
                         "sint"               : bp.testing.Limits_sint(),
                         "uint"               : bp.testing.Limits_uint(),
                         "slong"              : bp.testing.Limits_slong(),
                         "ulong"              : bp.testing.Limits_ulong(),
                         "slonglong"          : bp.testing.Limits_slonglong(),
                         "ulonglong"          : bp.testing.Limits_ulonglong(),
                         "vector<sshort>"     : bp.testing.Limits_sshort(),
                         "vector<ushort>"     : bp.testing.Limits_ushort(),
                         "vector<sint>"       : bp.testing.Limits_sint(),
                         "vector<uint>"       : bp.testing.Limits_uint(),
                         "vector<slong>"      : bp.testing.Limits_slong(),
                         "vector<ulong>"      : bp.testing.Limits_ulong(),
                         "vector<slonglong>"  : bp.testing.Limits_slonglong(),
                         "vector<ulonglong>"  : bp.testing.Limits_ulonglong(),
                       }
        
        
        # Dictionary of functions for getting/setting options through C++
        intfuncdict = { "sshort"             : bp.testing.TestOptGet_sshort,
                        "ushort"             : bp.testing.TestOptGet_ushort,
                        "sint"               : bp.testing.TestOptGet_sint,
                        "uint"               : bp.testing.TestOptGet_uint,
                        "slong"              : bp.testing.TestOptGet_slong,
                        "ulong"              : bp.testing.TestOptGet_ulong,
                        "slonglong"          : bp.testing.TestOptGet_slonglong,
                        "ulonglong"          : bp.testing.TestOptGet_ulonglong,
                      }






 
        for k,v in intlimitdict.items():
            bp.output.Output("Limit for %|1$-18| : [ %|2$-21| , %|3$-21| ]\n", k, v[0], v[1])
        bp.output.Output(bp.output.Line('-'))
        bp.output.Output("\n")


        tester = bp.testing.Tester("Testing passing and getting large numbers, etc, from OptionMap objects")
        tester.PrintHeader()

        # loop over integer types
        for k in intfuncdict:
            # get the limits
            limits = intlimitdict[k]

            # create an OptionMap
            opt = bp.options.OptionMap({ "test_opt" : ( "int", None, True, None, "(no help)" ) })

            # try setting the values
            vals = [ limits[0]-1, limits[0], limits[0]+1, limits[1]-1, limits[1], limits[1]+1 ]

            for val in vals:
                # TODO - FIXME
                if val > intlimitdict["slonglong"][1] or val < intlimitdict["slonglong"][0]:
                    continue

                opt.Change("test_opt", val)

                # now attempt to get it as various types
                for k2,v2 in intfuncdict.items():
                    limits2 = intlimitdict[k2]

                    s = "Getting {} as {}".format(k, k2)
                    expected = val >= limits2[0] and val <= limits2[1]
                    tester.Test(s, expected, bp.testing.PyTestFunc, v2, opt,  "test_opt")
                    
            
                     


        tester.PrintResults()


    except Exception as e:
      bp.output.Output("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      bp.output.Error("\n")
      bp.output.Error(str(e))
      bp.output.Error("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
