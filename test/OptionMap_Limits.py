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
        bp.testing.PrintHeader("Testing passing and getting large numebers, etc, from OptionMap objects")

        validtypes = ["int", "float", "bool", "str", "listint", "listfloat", "listbool", "liststr" ]

        nfailed = 0
        ntest = 1


        sshortlimits = bp.testing.Limits_sshort();
        ushortlimits = bp.testing.Limits_ushort();
        sintlimits = bp.testing.Limits_sint();
        uintlimits = bp.testing.Limits_uint();
        slonglimits = bp.testing.Limits_slong();
        ulonglimits = bp.testing.Limits_ulong();
        slonglonglimits = bp.testing.Limits_slonglong();
        ulonglonglimits = bp.testing.Limits_ulonglong();

        bp.output.Output("       Signed short limits: [ %1% , %2% ]\n", sshortlimits[0],    sshortlimits[1]);
        bp.output.Output("     Unsigned short limits: [ %1% , %2% ]\n", ushortlimits[0],    ushortlimits[1]);
        bp.output.Output("         Signed int limits: [ %1% , %2% ]\n", sintlimits[0],      sintlimits[1]);
        bp.output.Output("       Unsigned int limits: [ %1% , %2% ]\n", uintlimits[0],      uintlimits[1]);
        bp.output.Output("        Signed long limits: [ %1% , %2% ]\n", slonglimits[0],     slonglimits[1]);
        bp.output.Output("      Unsigned long limits: [ %1% , %2% ]\n", ulonglimits[0],     ulonglimits[1]);
        bp.output.Output("   Signed long long limits: [ %1% , %2% ]\n", slonglonglimits[0], slonglonglimits[1]);
        bp.output.Output(" Unsigned long long limits: [ %1% , %2% ]\n", ulonglonglimits[0], ulonglonglimits[1]);

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
