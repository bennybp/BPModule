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

        tester = bp.testing.Tester("Testing conversions from Python to C++")
        tester.PrintHeader()

        # integer types
        inttypes = [ "sshort", "ushort", "sint", "uint", "slong", "ulong", "slonglong", "ulonglong",
                     "vector<sshort>", "vector<ushort>", "vector<sint>", "vector<uint>",
                     "vector<slong>", "vector<ulong>", "vector<slonglong>", "vector<ulonglong>",
                   ]

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


                      # function                                     failing function                                 name                   acceptable python types
        testfuncs =    [ (bp.testing.TestPyCppPy_sshort,             bp.testing.TestPyCppPy_Fail_sshort,               "sshort",              ["int"]),
                         (bp.testing.TestPyCppPy_ushort,             bp.testing.TestPyCppPy_Fail_ushort,               "ushort",              ["int"]),
                         (bp.testing.TestPyCppPy_sint,               bp.testing.TestPyCppPy_Fail_sint,                 "sint",                ["int"]),
                         (bp.testing.TestPyCppPy_uint,               bp.testing.TestPyCppPy_Fail_uint,                 "uint",                ["int"]),
                         (bp.testing.TestPyCppPy_slong,              bp.testing.TestPyCppPy_Fail_slong,                "slong",               ["int"]),
                         (bp.testing.TestPyCppPy_ulong,              bp.testing.TestPyCppPy_Fail_ulong,                "ulong",               ["int"]),
                         (bp.testing.TestPyCppPy_slonglong,          bp.testing.TestPyCppPy_Fail_slonglong,            "slonglong",           ["int"]),
                         (bp.testing.TestPyCppPy_ulonglong,          bp.testing.TestPyCppPy_Fail_ulonglong,            "ulonglong",           ["int"]),
                         (bp.testing.TestPyCppPy_float,              bp.testing.TestPyCppPy_Fail_float,                "float",               ["float", "int"]),
                         (bp.testing.TestPyCppPy_double,             bp.testing.TestPyCppPy_Fail_double,               "double",              ["float", "int"]),
                         (bp.testing.TestPyCppPy_longdouble,         bp.testing.TestPyCppPy_Fail_longdouble,           "longdouble",          ["float", "int"]),
                         (bp.testing.TestPyCppPy_string,             bp.testing.TestPyCppPy_Fail_string,               "string",              ["string"]),
                         (bp.testing.TestPyCppPy_vector_sshort,      bp.testing.TestPyCppPy_Fail_vector_sshort,        "vector<sshort>",      ["list<int>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_ushort,      bp.testing.TestPyCppPy_Fail_vector_ushort,        "vector<ushort>",      ["list<int>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_sint,        bp.testing.TestPyCppPy_Fail_vector_sint,          "vector<sint>",        ["list<int>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_uint,        bp.testing.TestPyCppPy_Fail_vector_uint,          "vector<uint>",        ["list<int>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_slong,       bp.testing.TestPyCppPy_Fail_vector_slong,         "vector<slong>",       ["list<int>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_ulong,       bp.testing.TestPyCppPy_Fail_vector_ulong,         "vector<ulong>",       ["list<int>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_slonglong,   bp.testing.TestPyCppPy_Fail_vector_slonglong,     "vector<slonglong>",   ["list<int>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_ulonglong,   bp.testing.TestPyCppPy_Fail_vector_ulonglong,     "vector<ulonglong>",   ["list<int>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_float,       bp.testing.TestPyCppPy_Fail_vector_float,         "vector<float>",       ["list<float>",  "list<int>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_double,      bp.testing.TestPyCppPy_Fail_vector_double,        "vector<double>",      ["list<float>",  "list<int>", "list<empty>"]),
                         (bp.testing.TestPyCppPy_vector_longdouble,  bp.testing.TestPyCppPy_Fail_vector_longdouble,    "vector<long double>", ["list<float>",  "list<int>", "list<empty>"]),
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
            testdat.append(  ( "list<int>", [ val[0], val[0]+1, val[0]-1 ] ) )
            testdat.append(  ( "list<int>", [ val[1], val[1]+1, val[1]-1 ] ) )
            testdat.append(  ( "list<int>", [ val[0], val[0]+1, val[1], val[1]-1 ] ) )
            

            

        # some other random data
        testdat.extend( [ ( "int",                 int(-1)                             ),
                          ( "int",                 int(0)                              ),
                          ( "int",                 int(1)                              ),
                          ( "float",               float(0.0)                          ),
                          ( "float",               float(5)                            ),
                          ( "string",              "HelloString1"                      ),
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
                success = ( d[0] in f[3] )
                bp.output.GlobalWarning(str(success) + "\n")   

                # get the range for integer type
                if success and f[2] in inttypes:
                    if d[0] == "int":
                        success = d[1] >= intlimitdict[f[2]][0] and d[1] <= intlimitdict[f[2]][1]
                    elif d[0] == "list<int>":
                        success = min(d[1]) >= intlimitdict[f[2]][0] and max(d[1]) <= intlimitdict[f[2]][1]

                tester.Test( s, not success, f[0], d[1]  )

                # Test failing cpp-to-python as well
                if success:
                    s = "Test python {} <-> C++ Failure {}".format(d[0], f[2])
                    tester.Test( s, True, f[1], d[1]  )
       

        tester.PrintResults() 


    except Exception as e:
      bp.output.GlobalOutput("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      bp.output.GlobalError("\n")
      bp.output.GlobalError(str(e))
      bp.output.GlobalError("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
