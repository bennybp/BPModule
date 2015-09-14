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

        nfailed = 0
        ntest = 1


        # Single types
        nfailed += bp.testing.TestConvertToCpp_int    (ntest, "Test python int   -> C++ int",       True,  int(5))
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_long   (ntest, "Test python int   -> C++ long",      True,  int(5))
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_float  (ntest, "Test python int   -> C++ float",     True,  int(5))
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_double (ntest, "Test python int   -> C++ double",    True,  int(5))
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_string (ntest, "Test python int   -> C++ string",    False, int(5))
        ntest += 1
    
        nfailed += bp.testing.TestConvertToCpp_int    (ntest, "Test python float -> C++ int",       False, float(5.5))
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_long   (ntest, "Test python float -> C++ long",      False, float(5.5))
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_float  (ntest, "Test python float -> C++ float",     True,  float(5.5))
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_double (ntest, "Test python float -> C++ double",    True,  float(5.5))
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_string (ntest, "Test python float -> C++ string",    False, float(5.5))
        ntest += 1

        nfailed += bp.testing.TestConvertToCpp_int    (ntest, "Test python str   -> C++ int",       False, "Hello")
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_long   (ntest, "Test python str   -> C++ long",      False, "Hello")
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_float  (ntest, "Test python str   -> C++ float",     False, "Hello")
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_double (ntest, "Test python str   -> C++ double",    False, "Hello")
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_string (ntest, "Test python str   -> C++ string",    True,  "Hello")
        ntest += 1
        
        nfailed += bp.testing.TestConvertToCpp_int    (ntest, "Test python list  -> C++ int",       False, [])
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_long   (ntest, "Test python list  -> C++ long",      False, [])
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_float  (ntest, "Test python list  -> C++ float",     False, [])
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_double (ntest, "Test python list  -> C++ double",    False, [])
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_string (ntest, "Test python list  -> C++ string",    False, [])
        ntest += 1

        nfailed += bp.testing.TestConvertToCpp_int    (ntest, "Test python dict  -> C++ int",       False, {"hi" : 5})
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_long   (ntest, "Test python dict  -> C++ long",      False, {"hi" : 5})
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_float  (ntest, "Test python dict  -> C++ float",     False, {"hi" : 5})
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_double (ntest, "Test python dict  -> C++ double",    False, {"hi" : 5})
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_string (ntest, "Test python dict  -> C++ string",    False, {"hi" : 5})
        ntest += 1

        nfailed += bp.testing.TestConvertToCpp_int    (ntest, "Test python set   -> C++ int",       False, {"hi" , 5})
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_long   (ntest, "Test python set   -> C++ long",      False, {"hi" , 5})
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_float  (ntest, "Test python set   -> C++ float",     False, {"hi" , 5})
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_double (ntest, "Test python set   -> C++ double",    False, {"hi" , 5})
        ntest += 1
        nfailed += bp.testing.TestConvertToCpp_string (ntest, "Test python set   -> C++ string",    False, {"hi" , 5})
        ntest += 1



        # Lists to vectors
        nfailed += bp.testing.TestConvertToCppVector_int    (ntest, "Test python list<int>   -> C++ vector<int>",    True,  [ 5, 10, 15 ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_long   (ntest, "Test python list<int>   -> C++ vector<long>",   True,  [ 5, 10, 15 ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_float  (ntest, "Test python list<int>   -> C++ vector<float>",  True,  [ 5, 10, 15 ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_double (ntest, "Test python list<int>   -> C++ vector<double>", True,  [ 5, 10, 15 ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_string (ntest, "Test python list<int>   -> C++ vector<string>", False, [ 5, 10, 15 ] )
        ntest += 1

        nfailed += bp.testing.TestConvertToCppVector_int    (ntest, "Test python list<float> -> C++ vector<int>",    False, [ 5.1, 10.1, 15.1 ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_long   (ntest, "Test python list<float> -> C++ vector<long>",   False, [ 5.1, 10.1, 15.1 ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_float  (ntest, "Test python list<float> -> C++ vector<float>",  True,  [ 5.1, 10.1, 15.1 ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_double (ntest, "Test python list<float> -> C++ vector<double>", True,  [ 5.1, 10.1, 15.1 ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_string (ntest, "Test python list<float> -> C++ vector<string>", False, [ 5.1, 10.1, 15.1 ] )
        ntest += 1

        nfailed += bp.testing.TestConvertToCppVector_int    (ntest, "Test python list<str>   -> C++ vector<int>",    False, [ "Hi1", "Hi2" ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_long   (ntest, "Test python list<str>   -> C++ vector<long>",   False, [ "Hi1", "Hi2" ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_float  (ntest, "Test python list<str>   -> C++ vector<float>",  False, [ "Hi1", "Hi2" ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_double (ntest, "Test python list<str>   -> C++ vector<double>", False, [ "Hi1", "Hi2" ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_string (ntest, "Test python list<str>   -> C++ vector<string>", True,  [ "Hi1", "Hi2" ] )
        ntest += 1

        nfailed += bp.testing.TestConvertToCppVector_int    (ntest, "Test python list<list>  -> C++ vector<int>",    False, [ [4, 4] ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_long   (ntest, "Test python list<list>  -> C++ vector<long>",   False, [ [4, 4] ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_float  (ntest, "Test python list<list>  -> C++ vector<float>",  False, [ [4.0, 4.0] ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_double (ntest, "Test python list<list>  -> C++ vector<double>", False, [ [4.0, 4.0] ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_string (ntest, "Test python list<list>  -> C++ vector<string>", False, [ ["Hi1", "Hi2"] ] )
        ntest += 1

        nfailed += bp.testing.TestConvertToCppVector_int    (ntest, "Test python list<dict>  -> C++ vector<int>",    False, [ {"Hi1": 4} ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_long   (ntest, "Test python list<dict>  -> C++ vector<long>",   False, [ {"Hi1": 4} ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_float  (ntest, "Test python list<dict>  -> C++ vector<float>",  False, [ {"Hi1": 4.0} ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_double (ntest, "Test python list<dict>  -> C++ vector<double>", False, [ {"Hi1": 4.0} ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_string (ntest, "Test python list<dict>  -> C++ vector<string>", False, [ {"Hi1": "Hi2"} ] )
        ntest += 1

        nfailed += bp.testing.TestConvertToCppVector_int    (ntest, "Test python list<set>   -> C++ vector<int>",    False, [ {4, 4} ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_long   (ntest, "Test python list<set>   -> C++ vector<long>",   False, [ {4, 4} ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_float  (ntest, "Test python list<set>   -> C++ vector<float>",  False, [ {4.0, 4.0} ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_double (ntest, "Test python list<set>   -> C++ vector<double>", False, [ {4.0, 4.0} ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_string (ntest, "Test python list<set>   -> C++ vector<string>", False, [ {"Hi1", "Hi2"} ] )
        ntest += 1



        # Heterogeneous lists to vectors (should all fail)
        nfailed += bp.testing.TestConvertToCppVector_int    (ntest, "Test python hetero list<int>   -> C++ vector<int>",    False, [ 5, 10, "Hi" ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_long   (ntest, "Test python hetero list<int>   -> C++ vector<long>",   False, [ 5, 10, "Hi" ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_float  (ntest, "Test python hetero list<int>   -> C++ vector<float>",  False, [ 5, 10, "Hi" ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_double (ntest, "Test python hetero list<int>   -> C++ vector<double>", False, [ 5, 10, "Hi" ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_string (ntest, "Test python hetero list<int>   -> C++ vector<string>", False, [ 5, 10, "Hi" ] )
        ntest += 1

        nfailed += bp.testing.TestConvertToCppVector_int    (ntest, "Test python hetero list<float> -> C++ vector<int>",    False, [ 5.1, 10.1, "Hi" ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_long   (ntest, "Test python hetero list<float> -> C++ vector<long>",   False, [ 5.1, 10.1, "Hi" ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_float  (ntest, "Test python hetero list<float> -> C++ vector<float>",  False, [ 5.1, 10.1, "Hi" ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_double (ntest, "Test python hetero list<float> -> C++ vector<double>", False, [ 5.1, 10.1, "Hi" ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_string (ntest, "Test python hetero list<float> -> C++ vector<string>", False, [ 5.1, 10.1, "Hi" ] )
        ntest += 1

        nfailed += bp.testing.TestConvertToCppVector_int    (ntest, "Test python hetero list<str>   -> C++ vector<int>",    False, [ "Hi1", 4 ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_long   (ntest, "Test python hetero list<str>   -> C++ vector<long>",   False, [ "Hi1", 4 ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_float  (ntest, "Test python hetero list<str>   -> C++ vector<float>",  False, [ "Hi1", 4 ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_double (ntest, "Test python hetero list<str>   -> C++ vector<double>", False, [ "Hi1", 4 ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_string (ntest, "Test python hetero list<str>   -> C++ vector<string>", False, [ "Hi1", 4 ] )
        ntest += 1

        nfailed += bp.testing.TestConvertToCppVector_int    (ntest, "Test python hetero list<list>  -> C++ vector<int>",    False, [ 4,   ["Hi1", "Hi2"] ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_long   (ntest, "Test python hetero list<list>  -> C++ vector<long>",   False, [ 4,   ["Hi1", "Hi2"] ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_float  (ntest, "Test python hetero list<list>  -> C++ vector<float>",  False, [ 4.0, ["Hi1", "Hi2"] ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_double (ntest, "Test python hetero list<list>  -> C++ vector<double>", False, [ 4.0, ["Hi1", "Hi2"] ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_string (ntest, "Test python hetero list<list>  -> C++ vector<string>", False, [ "4", ["Hi1", "Hi2"] ] )
        ntest += 1

        nfailed += bp.testing.TestConvertToCppVector_int    (ntest, "Test python hetero list<dict>  -> C++ vector<int>",    False, [ 4,   {"Hi1": "Hi2"} ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_long   (ntest, "Test python hetero list<dict>  -> C++ vector<long>",   False, [ 4,   {"Hi1": "Hi2"} ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_float  (ntest, "Test python hetero list<dict>  -> C++ vector<float>",  False, [ 4.0, {"Hi1": "Hi2"} ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_double (ntest, "Test python hetero list<dict>  -> C++ vector<double>", False, [ 4.0, {"Hi1": "Hi2"} ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_string (ntest, "Test python hetero list<dict>  -> C++ vector<string>", False, [ "4", {"Hi1": "Hi2"} ] )
        ntest += 1

        nfailed += bp.testing.TestConvertToCppVector_int    (ntest, "Test python hetero list<set>   -> C++ vector<int>",    False, [ 4,   {"Hi1", "Hi2"} ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_long   (ntest, "Test python hetero list<set>   -> C++ vector<long>",   False, [ 4,   {"Hi1", "Hi2"} ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_float  (ntest, "Test python hetero list<set>   -> C++ vector<float>",  False, [ 4.0, {"Hi1", "Hi2"} ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_double (ntest, "Test python hetero list<set>   -> C++ vector<double>", False, [ 4.0, {"Hi1", "Hi2"} ] )
        ntest += 1
        nfailed += bp.testing.TestConvertToCppVector_string (ntest, "Test python hetero list<set>   -> C++ vector<string>", False, [ "4", {"Hi1", "Hi2"} ] )
        ntest += 1


        bp.testing.PrintResults(nfailed) 


    except Exception as e:
      bp.output.Output("Caught exception in main handler. Contact the developers\n")
      #traceback.print_exc()
      bp.output.Error("\n")
      bp.output.Error(str(e))
      bp.output.Error("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
