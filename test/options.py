#!/usr/bin/env python3


import os
import sys
import argparse
import traceback 

modpath = os.path.join("/home/ben/programming/BPModule/install/modules")
sys.path.insert(0, modpath)

import bppython as bp


def ShouldFail(modname, opt):
  try:
    bp.LoadModule(modname, opt, test = True)
  except:
    print("Module that should have failed did fail")
    return True
  print("Module that should have failed did NOT fail")
  return False


def ShouldSucceed(modname, opt):
  try:
    bp.LoadModule(modname, opt, test = True)
    print("Module that should have succeeded did succeed")
  except:
    print("Module that should have succeeded did NOT succeed")
    return False
  return True



bp.Init(debug = True, output="/tmp/test.out")





# Load the python modules
# Test
try:
    ShouldSucceed("testmodule1", 
                  { "TESTMOD1": 
                    { 
                        "double_req" : 2.0,
                        "int_req": 5,
                        "str_req": "Hello",
                        "bool_req": False,

                        "str_opt": "Optional",
                        "int_opt_def": 5,


                        "strvec_opt_def" : [ "I", "Am" ],
                        "intvec_opt_def" : [ 5, 10, 15, 20, 25 ],

                        "strvec_req" : ["Super", "Dork"],
                        "intvec_req" : [ 6, 10 ],
                        "doublevec_req" : [ 6.0, 10.123 ],
                        "boolvec_req" : [True],

                        "strvec_opt" : ["Super", "Dork"],
                        "intvec_opt" : [ 6, 10, 1 ],
                        "doublevec_opt" : [ 6.0, 10.123 ],
                        "boolvec_opt" : [False],
                    }    
                  }
               )
    ShouldFail("testmodule1", 
                  { "TESTMOD1": 
                    { 
                        "double_req" : 2.0,
                        "int_req": 5,
                        "str_req": "Hello",
                        "bool_req": False,

                        "str_opt": "Optional",
                        "int_opt_def": 5,


                        "strvec_opt_def" : [ "I", "Am" ],
                        "intvec_opt_def" : [ 5, 10, 15, 20, 25 ],

                        "strvec_req" : ["Super", "Dork"],
                        "intvec_req" : [ 6, 10 ],
                        "doublevec_req" : [ 6.0, 10.123 ],
                        "boolvec_req" : [True],

                        "strvec_opt" : ["Super", "Dork"],
                        "intvec_opt" : [ 6, 10, 1 ],
                        "doublevec_opt" : [ 6.0, 10.123 ],
                        "boolvec_opt" : [9],
                    }    
                  }
               )

    print("\nDone testing\n")

except Exception as e:
  bp.Output("\n\n")
  bp.Error("***Caught exception in main handler\n")
  traceback.print_exc()
  bp.Error(str(e))
  bp.Output("\n\n")


bp.Finalize()
