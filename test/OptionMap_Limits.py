#!/usr/bin/env python3


import os
import sys
import argparse
import copy
import traceback 

# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

import bpmodule as bp


def Run():
    try:
        bp.output.Output("\n")
        bp.output.Output(bp.util.Line('-'))

        # The limits on various integer types
        intlimitdict = { "sshort"             : bp.testing.Limits_sshort(),
                         "ushort"             : bp.testing.Limits_ushort(),
                         "sint"               : bp.testing.Limits_sint(),
                         "uint"               : bp.testing.Limits_uint(),
                         "slong"              : bp.testing.Limits_slong(),
                         "ulong"              : bp.testing.Limits_ulong(),
                         "slonglong"          : bp.testing.Limits_slonglong(),
                         "ulonglong"          : bp.testing.Limits_ulonglong()
                       }


        # Dictionary of functions for getting/setting options through C++
        intfuncdict = { "sshort"             : ( bp.testing.TestOptionMapGet_sshort,    bp.testing.TestOptionMapGet_vector_sshort ),
                        "ushort"             : ( bp.testing.TestOptionMapGet_ushort,    bp.testing.TestOptionMapGet_vector_ushort ),
                        "sint"               : ( bp.testing.TestOptionMapGet_sint,      bp.testing.TestOptionMapGet_vector_sint ),
                        "uint"               : ( bp.testing.TestOptionMapGet_uint,      bp.testing.TestOptionMapGet_vector_uint ),
                        "slong"              : ( bp.testing.TestOptionMapGet_slong,     bp.testing.TestOptionMapGet_vector_slong ),
                        "ulong"              : ( bp.testing.TestOptionMapGet_ulong,     bp.testing.TestOptionMapGet_vector_ulong ),
                        "slonglong"          : ( bp.testing.TestOptionMapGet_slonglong, bp.testing.TestOptionMapGet_vector_slonglong ),
                        "ulonglong"          : ( bp.testing.TestOptionMapGet_ulonglong, bp.testing.TestOptionMapGet_vector_ulonglong ),
                      }


 
        for k,v in intlimitdict.items():
            bp.output.Output("Limit for %|1$-18| : [ %|2$-21| , %|3$-21| ]\n", k, v[0], v[1])
        bp.output.Output(bp.util.Line('-'))
        bp.output.Output("\n")


        tester = bp.testing.Tester("Testing passing and getting large numbers, etc, from OptionMap objects")
        tester.PrintHeader()

        # loop over integer types
        for k,limits in intlimitdict.items():

            # create an OptionMap
            opt = bp.options.OptionMap( "nomodule", 
                                        { "test_opt" : ( "int", None, True, None, "(no help)" ),
                                          "test_opt_vec" : ( "listint", None, True, None, "(no help)" ) },
                                        None)

            # try setting the values
            vals = [ limits[0]-1, limits[0], limits[0]+1, limits[1]-1, limits[1], limits[1]+1 ]

            for val in vals:
                # TODO - FIXME
                if val > intlimitdict["slonglong"][1] or val < intlimitdict["slonglong"][0]:
                    continue

                opt.Change("test_opt", val)
                opt.Change("test_opt_vec",  [val])

                # now attempt to get it as various types
                for k2,v2 in intfuncdict.items():
                    limits2 = intlimitdict[k2]
                    expected = val >= limits2[0] and val <= limits2[1]

                    s = "Getting {} as {}".format(k, k2)
                    tester.Test(s, expected, bp.testing.PyTestFunc, v2[0], opt,  "test_opt")

                    s = "Getting {} as vector of {}".format(k, k2)
                    tester.Test(s, expected, bp.testing.PyTestFunc, v2[1], opt,  "test_opt_vec")
                    


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
