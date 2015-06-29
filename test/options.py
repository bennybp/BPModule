#!/usr/bin/env python3


import os
import sys
import argparse
import traceback 

modpath = os.path.join("/home/ben/programming/BPModule/install/modules")
sys.path.insert(0, modpath)

import bppython as bp

bp.Init(debug = True)

bp.Output("******************************\n")
bp.Output("* Testing output             *\n")
bp.Output("****************************** %1% %2% %3%\n", "Hi there", 3, 3.1415)
bp.Success("****************************** %1% %2% %3%\n", "Hi there", 3, 3.1415)
bp.Changed("****************************** %1% %2% %3%\n", "Hi there", 3, 3.1415)
bp.Warning("****************************** %1% %2% %3%\n", "Hi there", 3, 3.1415)
bp.Error("****************************** %1% %2% %3%\n", "Hi there", 3, 3.1415)
bp.Debug("****************************** %1% %2% %3%\n", "Hi there", 3, 3.1415)


# Load the python modules
# Test
try:
  bp.LoadModule("testmodule1", 
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
                    #"intvec_opt" : [ 6, 10 ],
                    #"doublevec_opt" : [ 6.0, 10.123 ],
                    #"boolvec_opt" : [True],
                    
                  }
               )

  bp.LoadModule("testpymodule1")

  b1 = bp.mst.GetModule_Test("TESTMOD1")
  b2 = bp.mst.GetModule_Test("TESTPYMOD1")

  b1.RunTest()
  print()
  b2.RunTest()
  print()

  print("\nDone testing\n")

except Exception as e:
  bp.Output("\n\n")
  bp.Error("***Caught exception in main handler\n")
  traceback.print_exc()
  bp.Error(str(e))
  bp.Output("\n\n")


bp.Finalize()
