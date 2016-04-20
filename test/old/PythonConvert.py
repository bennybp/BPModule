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

        tester = psr.testing.Tester("Testing conversions from Python to C++")
        tester.PrintHeader()

        # integer types
        inttypes = [ "sshort", "ushort", "sint", "uint", "slong", "ulong", "slonglong", "ulonglong",
                     "vector<sshort>", "vector<ushort>", "vector<sint>", "vector<uint>",
                     "vector<slong>", "vector<ulong>", "vector<slonglong>", "vector<ulonglong>",
                   ]

        # The limits on various integer types
        intlimitdict = { "sshort"             : psr.testing.Limits_sshort(),
                         "ushort"             : psr.testing.Limits_ushort(),
                         "sint"               : psr.testing.Limits_sint(),
                         "uint"               : psr.testing.Limits_uint(),
                         "slong"              : psr.testing.Limits_slong(),
                         "ulong"              : psr.testing.Limits_ulong(),
                         "slonglong"          : psr.testing.Limits_slonglong(),
                         "ulonglong"          : psr.testing.Limits_ulonglong(),
                         "vector<sshort>"     : psr.testing.Limits_sshort(),
                         "vector<ushort>"     : psr.testing.Limits_ushort(),
                         "vector<sint>"       : psr.testing.Limits_sint(),
                         "vector<uint>"       : psr.testing.Limits_uint(),
                         "vector<slong>"      : psr.testing.Limits_slong(),
                         "vector<ulong>"      : psr.testing.Limits_ulong(),
                         "vector<slonglong>"  : psr.testing.Limits_slonglong(),
                         "vector<ulonglong>"  : psr.testing.Limits_ulonglong(),
                       }


                      # function                                     failing function                                 name                   acceptable python types
        testfuncs =    [ (psr.testing.TestPyCppPy_sshort,             psr.testing.TestPyCppPy_Fail_sshort,               "sshort",              ["int"]),
                         (psr.testing.TestPyCppPy_ushort,             psr.testing.TestPyCppPy_Fail_ushort,               "ushort",              ["int"]),
                         (psr.testing.TestPyCppPy_sint,               psr.testing.TestPyCppPy_Fail_sint,                 "sint",                ["int"]),
                         (psr.testing.TestPyCppPy_uint,               psr.testing.TestPyCppPy_Fail_uint,                 "uint",                ["int"]),
                         (psr.testing.TestPyCppPy_slong,              psr.testing.TestPyCppPy_Fail_slong,                "slong",               ["int"]),
                         (psr.testing.TestPyCppPy_ulong,              psr.testing.TestPyCppPy_Fail_ulong,                "ulong",               ["int"]),
                         (psr.testing.TestPyCppPy_slonglong,          psr.testing.TestPyCppPy_Fail_slonglong,            "slonglong",           ["int"]),
                         (psr.testing.TestPyCppPy_ulonglong,          psr.testing.TestPyCppPy_Fail_ulonglong,            "ulonglong",           ["int"]),
                         (psr.testing.TestPyCppPy_float,              psr.testing.TestPyCppPy_Fail_float,                "float",               ["float", "int"]),
                         (psr.testing.TestPyCppPy_double,             psr.testing.TestPyCppPy_Fail_double,               "double",              ["float", "int"]),
                         (psr.testing.TestPyCppPy_longdouble,         psr.testing.TestPyCppPy_Fail_longdouble,           "longdouble",          ["float", "int"]),
                         (psr.testing.TestPyCppPy_string,             psr.testing.TestPyCppPy_Fail_string,               "string",              ["string"]),
                         (psr.testing.TestPyCppPy_vector_sshort,      psr.testing.TestPyCppPy_Fail_vector_sshort,        "vector<sshort>",      ["list<int>", "list<empty>"]),
                         (psr.testing.TestPyCppPy_vector_ushort,      psr.testing.TestPyCppPy_Fail_vector_ushort,        "vector<ushort>",      ["list<int>", "list<empty>"]),
                         (psr.testing.TestPyCppPy_vector_sint,        psr.testing.TestPyCppPy_Fail_vector_sint,          "vector<sint>",        ["list<int>", "list<empty>"]),
                         (psr.testing.TestPyCppPy_vector_uint,        psr.testing.TestPyCppPy_Fail_vector_uint,          "vector<uint>",        ["list<int>", "list<empty>"]),
                         (psr.testing.TestPyCppPy_vector_slong,       psr.testing.TestPyCppPy_Fail_vector_slong,         "vector<slong>",       ["list<int>", "list<empty>"]),
                         (psr.testing.TestPyCppPy_vector_ulong,       psr.testing.TestPyCppPy_Fail_vector_ulong,         "vector<ulong>",       ["list<int>", "list<empty>"]),
                         (psr.testing.TestPyCppPy_vector_slonglong,   psr.testing.TestPyCppPy_Fail_vector_slonglong,     "vector<slonglong>",   ["list<int>", "list<empty>"]),
                         (psr.testing.TestPyCppPy_vector_ulonglong,   psr.testing.TestPyCppPy_Fail_vector_ulonglong,     "vector<ulonglong>",   ["list<int>", "list<empty>"]),
                         (psr.testing.TestPyCppPy_vector_float,       psr.testing.TestPyCppPy_Fail_vector_float,         "vector<float>",       ["list<float>",  "list<int>", "list<empty>"]),
                         (psr.testing.TestPyCppPy_vector_double,      psr.testing.TestPyCppPy_Fail_vector_double,        "vector<double>",      ["list<float>",  "list<int>", "list<empty>"]),
                         (psr.testing.TestPyCppPy_vector_longdouble,  psr.testing.TestPyCppPy_Fail_vector_longdouble,    "vector<long double>", ["list<float>",  "list<int>", "list<empty>"]),
                         (psr.testing.TestPyCppPy_vector_string,      psr.testing.TestPyCppPy_Fail_vector_string,        "vector<string>",      ["list<string>", "list<empty>"])
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
                psr.output.GlobalWarning(str(success) + "\n")   

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
      psr.output.GlobalOutput("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      psr.output.GlobalError("\n")
      psr.output.GlobalError(str(e))
      psr.output.GlobalError("\n")




psr.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
psr.Finalize()
