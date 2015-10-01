#!/usr/bin/env python3


############################
# Testing of ModuleInfo
############################

import os
import sys
import argparse
import copy
import traceback 

sys.path.insert(0, "/home/ben/programming/BPModule/install/modules")
sys.path.insert(0, "/home/ben/programming/ambit/install/lib")

import bpmodule as bp


def Run():
    try:
        bp.testing.PrintHeader("Testing conversions from Python to C++")

        inttypes = [ "sshort", "ushort", "sint", "uint", "slong", "ulong", "slonglong", "ulonglong",
                     "vector<sshort>", "vector<ushort>", "vector<sint>", "vector<uint>",
                     "vector<slong>", "vector<ulong>", "vector<slonglong>", "vector<ulonglong>",
                   ]


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


                      # function                                            name                   acceptable python types
        testfuncs =    [ (bp.testing.TestConvertToCpp_sshort,               "sshort",              ["int"]),
                         (bp.testing.TestConvertToCpp_ushort,               "ushort",              ["int"]),
                         (bp.testing.TestConvertToCpp_sint,                 "sint",                ["int"]),
                         (bp.testing.TestConvertToCpp_uint,                 "uint",                ["int"]),
                         (bp.testing.TestConvertToCpp_slong,                "slong",               ["int"]),
                         (bp.testing.TestConvertToCpp_ulong,                "ulong",               ["int"]),
                         (bp.testing.TestConvertToCpp_slonglong,            "slonglong",           ["int"]),
                         (bp.testing.TestConvertToCpp_ulonglong,            "ulonglong",           ["int"]),

                         (bp.testing.TestConvertToCpp_float,                "float",               ["float"]),
                         (bp.testing.TestConvertToCpp_double,               "double",              ["float"]),
                         (bp.testing.TestConvertToCpp_longdouble,           "longdouble",          ["float"]),

                         (bp.testing.TestConvertToCpp_string,               "string",              ["string"]),


                         (bp.testing.TestConvertToCpp_vector_sshort,        "vector<sshort>",      ["list<int>", "list<empty>"]),
                         (bp.testing.TestConvertToCpp_vector_ushort,        "vector<ushort>",      ["list<int>", "list<empty>"]),
                         (bp.testing.TestConvertToCpp_vector_sint,          "vector<sint>",        ["list<int>", "list<empty>"]),
                         (bp.testing.TestConvertToCpp_vector_uint,          "vector<uint>",        ["list<int>", "list<empty>"]),
                         (bp.testing.TestConvertToCpp_vector_slong,         "vector<slong>",       ["list<int>", "list<empty>"]),
                         (bp.testing.TestConvertToCpp_vector_ulong,         "vector<ulong>",       ["list<int>", "list<empty>"]),
                         (bp.testing.TestConvertToCpp_vector_slonglong,     "vector<slonglong>",   ["list<int>", "list<empty>"]),
                         (bp.testing.TestConvertToCpp_vector_ulonglong,     "vector<ulonglong>",   ["list<int>", "list<empty>"]),

                         (bp.testing.TestConvertToCpp_vector_float,         "vector<float>",       ["list<float>", "list<empty>"]),
                         (bp.testing.TestConvertToCpp_vector_double,        "vector<double>",      ["list<float>", "list<empty>"]),
                         (bp.testing.TestConvertToCpp_vector_longdouble,    "vector<long double>", ["list<float>", "list<empty>"]),

                         (bp.testing.TestConvertToCpp_vector_string,        "vector<string>",      ["list<string>", "list<empty>"])
                       ]  


        testdat = []

        # fill in using integer limits
        for key,val in intlimitdict.items():
            testdat.append(  ( "int", val[0]   ) )
            testdat.append(  ( "int", val[1]   ) )
            testdat.append(  ( "int", val[0]+1 ) )
            testdat.append(  ( "int", val[1]+1 ) )
            testdat.append(  ( "int", val[0]-1 ) )
            testdat.append(  ( "int", val[1]-1 ) )

            testdat.append(  ( "list<int>", [ val[0]   ] ) )
            testdat.append(  ( "list<int>", [ val[1]   ] ) )
            testdat.append(  ( "list<int>", [ val[0]+1 ] ) )
            testdat.append(  ( "list<int>", [ val[1]+1 ] ) )
            testdat.append(  ( "list<int>", [ val[0]-1 ] ) )
            testdat.append(  ( "list<int>", [ val[1]-1 ] ) )
            

            


        testdat.extend( [ ( "int",                 int(-1)                             ),
                          ( "int",                 int(0)                              ),
                          ( "int",                 int(1)                              ),
                          ( "float",               float(0.0)                          ),
                          ( "float",               float(5)                            ),
                          ( "string",              "HelloString1"                      ),
                          ( "string",              "HelloString2"                      ),
                          ( "list<int>",           [ int(5) ]                          ),
                          ( "list<int>",           [ int(5), int(0) ]                  ),
                          ( "list<float>",         [ float(5.0), float(0.0) ]          ),
                          ( "list<string>",        [ "HelloString1", "HelloString2" ]  ),
                          ( "list<empty>",         []                                  ),
                          ( "list<list<empty>>",   [[]]                                ),
                          ( "list<list<int>>",     [[int(5)]]                          ),
                          ( "list<list<float>>",   [[float(5.0)]]                      ),
                          ( "list<list<string>>",  [["Hello"]]                         ),
                          ( "list<heterogeneous>", [ int(5), "Hello" ]                 ),
                          ( "list<heterogeneous>", [ float(5.0), "Hello" ]             ),
                        ] )


        nfailed = 0
        ntest = 1


        for f in testfuncs:
            for d in testdat:
                s = "Test python {} -> C++ {}".format(d[0], f[1])
                expected = ( d[0] in f[2] )

                # get the range for integer type
                if expected and f[1] in inttypes:
                    if d[0] == "int":
                        expected = d[1] >= intlimitdict[f[1]][0] and d[1] <= intlimitdict[f[1]][1]
                    elif d[0] == "list<int>":
                        expected = min(d[1]) >= intlimitdict[f[1]][0] and max(d[1]) <= intlimitdict[f[1]][1]

                nfailed += f[0](ntest, s, expected, d[1])
                ntest += 1
       

        bp.testing.PrintResults(nfailed) 


    except Exception as e:
      bp.output.Output("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      bp.output.Error("\n")
      bp.output.Error(str(e))
      bp.output.Error("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
