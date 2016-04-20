#!/usr/bin/env python3


import os
import sys
import argparse
import copy
import traceback 

# Add the pulsar path
thispath = os.path.dirname(os.path.realpath(__file__))
psrpath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, psrpath)

import pulsar as psr


def Run():
    try:
        psr.output.GlobalOutput("\n")
        psr.output.GlobalOutput(psr.util.Line('-'))

        # The limits on various integer types
        intlimitdict = { "sshort"             : psr.testing.Limits_sshort(),
                         "ushort"             : psr.testing.Limits_ushort(),
                         "sint"               : psr.testing.Limits_sint(),
                         "uint"               : psr.testing.Limits_uint(),
                         "slong"              : psr.testing.Limits_slong(),
                         "ulong"              : psr.testing.Limits_ulong(),
                         "slonglong"          : psr.testing.Limits_slonglong(),
                         "ulonglong"          : psr.testing.Limits_ulonglong()
                       }


        # Dictionary of functions for getting/setting options through C++
        intfuncdict = { "sshort"             : ( psr.testing.TestOptionMapGet_sshort,    psr.testing.TestOptionMapGet_vector_sshort ),
                        "ushort"             : ( psr.testing.TestOptionMapGet_ushort,    psr.testing.TestOptionMapGet_vector_ushort ),
                        "sint"               : ( psr.testing.TestOptionMapGet_sint,      psr.testing.TestOptionMapGet_vector_sint ),
                        "uint"               : ( psr.testing.TestOptionMapGet_uint,      psr.testing.TestOptionMapGet_vector_uint ),
                        "slong"              : ( psr.testing.TestOptionMapGet_slong,     psr.testing.TestOptionMapGet_vector_slong ),
                        "ulong"              : ( psr.testing.TestOptionMapGet_ulong,     psr.testing.TestOptionMapGet_vector_ulong ),
                        "slonglong"          : ( psr.testing.TestOptionMapGet_slonglong, psr.testing.TestOptionMapGet_vector_slonglong ),
                        "ulonglong"          : ( psr.testing.TestOptionMapGet_ulonglong, psr.testing.TestOptionMapGet_vector_ulonglong ),
                      }


 
        for k,v in intlimitdict.items():
            psr.output.GlobalOutput("Limit for {:<18} : [ {:<21} , {:<21} ]\n".format(k, v[0], v[1]))
        psr.output.GlobalOutput(psr.util.Line('-'))
        psr.output.GlobalOutput("\n")


        tester = psr.testing.Tester("Testing passing and getting large numbers, etc, from OptionMap objects")
        tester.PrintHeader()

        # loop over integer types
        for k,limits in intlimitdict.items():

            # create an OptionMap
            opt = psr.datastore.OptionMap( "nomodule", 
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
                    tester.Test(s, expected, psr.testing.PyTestFunc, v2[0], opt,  "test_opt")

                    s = "Getting {} as vector of {}".format(k, k2)
                    tester.Test(s, expected, psr.testing.PyTestFunc, v2[1], opt,  "test_opt_vec")
                    


        tester.PrintResults()


    except Exception as e:
      psr.output.GlobalOutput("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      psr.output.GlobalError("\n")
      psr.output.GlobalError(str(e))
      psr.output.GlobalError("\n")




psr.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
psr.Finalize()
