#!/usr/bin/env python3


import os
import sys
import traceback 


# Add the bpmodule path
thispath = os.path.dirname(os.path.realpath(__file__))
bppath = os.path.join(os.path.dirname(thispath), "modules")
sys.path.insert(0, bppath)

import bpmodule as bp
from bpmodule.output import *
from bpmodule.testing import *
from bpmodule.datastore import *

from helper.SetOperations import TestSetOperations
from helper.TestAtoms import nonane

# can d be converted to t
def IsValid(t, d):
    if t == d:
        return True

    if t == "float" and d == "int":  # cast int to float
        return True

    if t == "int" and d == "float":  # cast float to int
        return True

    if t == "listfloat" and d == "listint":  # cast int to float
        return True

    if t == "listint" and d == "listfloat":  # cast float to int
        return True

    return False



def Run():
    try:

        tester = Tester("Testing construction of OptionMap objects")
        tester.PrintHeader()

        # map to optionstypes,
        opttypemap = { "int"   : OptionType.Int,
                       "float" : OptionType.Float,
                       "bool"  : OptionType.Bool,
                       "str"   : OptionType.String,
                       "setint"   : OptionType.SetInt,
                       "setfloat" : OptionType.SetFloat,
                       "setbool"  : OptionType.SetBool,
                       "setstr"   : OptionType.SetString,
                       "listint"   : OptionType.ListInt,
                       "listfloat" : OptionType.ListFloat,
                       "listbool"  : OptionType.ListBool,
                       "liststr"   : OptionType.ListString,
                      }
                    # TODO
                    #                        OptionType.DictIntInt
                    #                        OptionType.DictIntFloat
                    #                        OptionType.DictIntBool
                    #                        OptionType.DictIntString,
                    #                        OptionType.DictStringInt
                    #                        OptionType.DictStringFloat
                    #                        OptionType.DictStringBool
                    #                        OptionType.DictStringString


        # valid data for some types
        validvals = [ ("int" ,   5),
                      ("float" , 5.0),
                      ("bool" , True),
                      ("str", "Hello"),
                      ("listint" , [-1, 0, 1]),
                      ("listfloat" , [-1.0, 0.0, 1.0]),
                      ("listbool" , [False, False, True]),
                      ("liststr" , ["My", "Test", "Strings"]),
                      ("listint" , []),
                      ("listfloat" , []),
                      ("listbool" , []),
                      ("liststr" , []),
                      ("setint" , { -1, 0, 1, 10 }),
                      ("setfloat" , { -1.1, 0.1, 1.1, 10.1 }),
                      ("setbool" , set({True, False})),
                      ("setstr" , { "My", "Set", "of", "Strings" }),
                      ("setint" , set()),
                      ("setfloat" , set()),
                      ("setbool" , set()),
                      ("setstr" , set())
                    ]



        # Test single element, no validator
        for k,v in validvals:

            # with default, not required
            optmap = OptionMap()
            s = "Test adding {} - not required, with default".format(k)
            opttype = opttypemap[k]
            tester.Test(s, True, PyTestFunc, optmap.AddOption, k, 
                        opttype, False, None, "No help", v)

            # with default, required (should never work)
            optmap = OptionMap()
            s = "Test adding {} - required, with default".format(k)
            opttype = opttypemap[k]
            tester.Test(s, False, PyTestFunc, optmap.AddOption, k, 
                        opttype, True, None, "No help", v)

            # no default, not required
            optmap = OptionMap()
            s = "Test adding {} - not required, no default".format(k)
            opttype = opttypemap[k]
            tester.Test(s, True, PyTestFunc, optmap.AddOption, k, 
                        opttype, False, None, "No help", None)

            # no default, required
            optmap = OptionMap()
            s = "Test adding {} - required, no default".format(k)
            opttype = opttypemap[k]
            tester.Test(s, True, PyTestFunc, optmap.AddOption, k, 
                        opttype, True, None, "No help", None)

        # Adding multiple elements
        optmap = OptionMap("nomodule")
        for k, v in validvals: 
            s = "Test adding {}".format(k)
            opttype = opttypemap[k]
            dup = optmap.HasKey(k) # Can't add duplicate keys
            tester.Test(s, not dup, PyTestFunc, optmap.AddOption, k, 
                        opttype, False, None, "No help", v)
        


#
#        for t1 in validtypes:
#            # create a list of dicts to test
#            allopt = [ { "test_opt" : ( t1,   None,          True, None, "(no help)" ) },
#                       { "test_opt" : ( t1,   None,          False, None, "(no help)" ) },
#                       { "test_opt" : ( t1,   validvals[t1], False, None, "(no help)" ) },
#                     ]
#
#
#            for opt in allopt:
#                # Construct with some valid data
#                opm = OptionMap("nomodule", opt, None)
#
#                v = opt["test_opt"]
#
#                # should be invalid if required
#                isvalid = not v[2]
#
#                # have (default) value?
#                hasdefault = v[1] != None
#
#                s = "OptionMap {} : Testing validity".format(t1)
#                tester.Test(s, isvalid, PyTestBoolFunc, opm.AllReqSet)
#
#                s = "OptionMap {} : Has option?".format(t1)
#                tester.Test(s, True, PyTestBoolFunc, opm.HasKey, "test_opt")
#
#                s = "OptionMap {} : Has option value?".format(t1)
#                tester.Test(s, hasdefault, PyTestBoolFunc, opm.Has, "test_opt")
#
#                # set the value
#                for d1 in testelements:
#                    s = "OptionMap {} : Resetting option".format(t1)
#                    tester.Test(s, True, PyTestFunc, opm.ResetToDefault, "test_opt")
#
#                    s = "OptionMap {} : Testing validity after resetting".format(t1)
#                    tester.Test(s, isvalid, PyTestBoolFunc, opm.AllReqSet)
#
#                    s = "OptionMap {} : Setting option with {}".format(t1, d1[0])
#                    expected = IsValid(t1, d1[0])
#                    tester.Test(s, expected, PyTestFunc, opm.Change, "test_opt", d1[1])
#
#                    # should be valid if expected == True, since that means the value is now set
#                    # (or if it was previously valid)
#                    s = "OptionMap {} : Is valid now?".format(t1)
#                    tester.Test(s, expected or isvalid, PyTestBoolFunc, opm.AllReqSet)
#
#                    # should still have the option
#                    s = "OptionMap {} : Has option?".format(t1)
#                    tester.Test(s, True, PyTestBoolFunc, opm.HasKey, "test_opt")
#
#                    # value is set if expected == True
#                    s = "OptionMap {} : Has option value?".format(t1)
#                    tester.Test(s, expected or hasdefault, PyTestBoolFunc, opm.Has, "test_opt")
#
#
#
#                    # Reset again
#                    s = "OptionMap {} : Resetting option".format(t1)
#                    tester.Test(s, True, PyTestFunc, opm.ResetToDefault, "test_opt")
#
#                    s = "OptionMap {} : Testing validity after resetting".format(t1)
#                    tester.Test(s, isvalid, PyTestBoolFunc, opm.AllReqSet)
#
#                    s = "OptionMap {} : Setting option as dict with {}".format(t1, d1[0])
#                    expected = IsValid(t1, d1[0])
#                    tester.Test(s, expected, PyTestFunc, opm.ChangeDict, { "test_opt" :  d1[1] })
#
#                    # should be valid if expected == True, since that means the value is now set
#                    # (or if it was previously valid)
#                    s = "OptionMap {} : Is valid now?".format(t1)
#                    tester.Test(s, expected or isvalid, PyTestBoolFunc, opm.AllReqSet)
#
#                    # should still have the option
#                    s = "OptionMap {} : Has option?".format(t1)
#                    tester.Test(s, True, PyTestBoolFunc, opm.HasKey, "test_opt")
#
#                    # value is set if expected == True
#                    s = "OptionMap {} : Has option value?".format(t1)
#                    tester.Test(s, expected or hasdefault, PyTestBoolFunc, opm.Has, "test_opt")



        tester.PrintResults() 


    except Exception as e:
      GlobalOutput("Caught exception in main handler. Contact the developers\n")
      traceback.print_exc()
      GlobalError("\n")
      GlobalError(str(e))
      GlobalError("\n")




bp.Init(sys.argv, out = "stdout", color = True, debug = True)
Run()
bp.Finalize()
