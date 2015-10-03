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

        tester = bp.testing.Tester("Testing conversions from Python to C++")
        tester.PrintHeader()

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
        testfuncs =    [ (bp.testing.TestPyCppPy_sshort,             bp.testing.TestPyCppPy_Fail_sshort,               "sshort",              ["int"]),
                         (bp.testing.TestPyCppPy_ushort,             bp.testing.TestPyCppPy_Fail_ushort,               "ushort",              ["int"]),
                         (bp.testing.TestPyCppPy_sint,               bp.testing.TestPyCppPy_Fail_sint,                 "sint",                ["int"]),
                         (bp.testing.TestPyCppPy_uint,               bp.testing.TestPyCppPy_Fail_uint,                 "uint",                ["int"]),
                         (bp.testing.TestPyCppPy_slong,              bp.testing.TestPyCppPy_Fail_slong,                "slong",               ["int"]),
                         (bp.testing.TestPyCppPy_ulong,              bp.testing.TestPyCppPy_Fail_ulong,                "ulong",               ["int"]),
                         (bp.testing.TestPyCppPy_slonglong,          bp.testing.TestPyCppPy_Fail_slonglong,            "slonglong",           ["int"]),
                         (bp.testing.TestPyCppPy_ulonglong,          bp.testing.TestPyCppPy_Fail_ulonglong,            "ulonglong",           ["int"]),
                         (bp.testing.TestPyCppPy_float,              bp.testing.TestPyCppPy_Fail_float,                "float",               ["float"]),
                         (bp.testing.TestPyCppPy_double,             bp.testing.TestPyCppPy_Fail_double,               "double",              ["float"]),
                         (bp.testing.TestPyCppPy_longdouble,         bp.testing.TestPyCppPy_Fail_longdouble,           "longdouble",          ["float"]),
                         (bp.testing.TestPyCppPy_string,             bp.testing.TestPyCppPy_Fail_string,               "string",              ["string"]),
                         (bp.testing.TestPyCppPy_vector_sshort,      bp.testing.TestPyCppPy_Fail_vector_sshort,        "vector<sshort>",      ["list<int>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_ushort,      bp.testing.TestPyCppPy_Fail_vector_ushort,        "vector<ushort>",      ["list<int>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_sint,        bp.testing.TestPyCppPy_Fail_vector_sint,          "vector<sint>",        ["list<int>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_uint,        bp.testing.TestPyCppPy_Fail_vector_uint,          "vector<uint>",        ["list<int>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_slong,       bp.testing.TestPyCppPy_Fail_vector_slong,         "vector<slong>",       ["list<int>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_ulong,       bp.testing.TestPyCppPy_Fail_vector_ulong,         "vector<ulong>",       ["list<int>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_slonglong,   bp.testing.TestPyCppPy_Fail_vector_slonglong,     "vector<slonglong>",   ["list<int>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_ulonglong,   bp.testing.TestPyCppPy_Fail_vector_ulonglong,     "vector<ulonglong>",   ["list<int>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_float,       bp.testing.TestPyCppPy_Fail_vector_float,         "vector<float>",       ["list<float>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_double,      bp.testing.TestPyCppPy_Fail_vector_double,        "vector<double>",      ["list<float>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_longdouble,  bp.testing.TestPyCppPy_Fail_vector_longdouble,    "vector<long double>", ["list<float>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_string,      bp.testing.TestPyCppPy_Fail_vector_string,        "vector<string>",      ["list<string>", "list<empty>"])
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


        for f in testfuncs:
            for d in testdat:
                s = "Test python {} <-> C++ {}".format(d[0], f[2])
                expected = ( d[0] in f[3] )

                # get the range for integer type
                if expected and f[2] in inttypes:
                    if d[0] == "int":
                        expected = d[1] >= intlimitdict[f[2]][0] and d[1] <= intlimitdict[f[2]][1]
                    elif d[0] == "list<int>":
                        expected = min(d[1]) >= intlimitdict[f[2]][0] and max(d[1]) <= intlimitdict[f[2]][1]

                tester.Test( s, expected, f[0], d[1]  )

                # Test failing cpp-to-python as well
                if expected:
                    s = "Test python {} <-> C++ Failure {}".format(d[0], f[2])
                    tester.Test( s, False, f[1], d[1]  )
       

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
