#!/usr/bin/env python3

import os
import sys
from  importlib.machinery import SourceFileLoader

sys.path.insert(0, "/home/ben/programming/BPModule/install/modules")


validtypes = ["int", "float", "bool", "str", "listint", "listfloat", "listbool", "liststr"]

indent = "    "
indent1 = indent*1
indent2 = indent*2
indent3 = indent*3
indent4 = indent*4
indent5 = indent*5

def TestStringItem(minfo, item, required):
    if not item in minfo:
        if required:
            print(" -> " + indent2 + "Item {} does not exist!".format(item))
            return False
        else:
            print(indent3 + "Item {} does not exist, but that's ok".format(item))
            return True

    if type(minfo[item]) != str:
        print(" -> " + indent2 + "Item {} is not a string!".format(item))
        return False
    
    print(indent3 + "Item {} looks ok".format(item))
    return True




def TestListStringItem(minfo, item, required):
    if not item in minfo:
        if required:
            print(" -> " + indent2 + "Item {} does not exist!".format(item))
            return False
        else:
            print(" -> " + indent2 + "Item {} does not exist, but that's ok".format(item))
            return True

    if type(minfo[item]) != list:
        print(" -> " + indent2 + "Item {} is not a list!".format(item))
        return False
        

    ret = True

    # should be ok if empty
    for i in minfo[item]:
        if type(i) != str:
            print(" -> " + indent2 + "Item {} , element {} is not a string!".format(item, i))
            ret = False

    if ret:    
        print(indent3 + "Item {} looks ok".format(item))

    return ret




def TestFile(item, path):
    if not os.path.isfile(path):
        print(" -> " + indent2 + "Item {} - path is not a file: ".format(item, path))
        return False
    else:
        print(indent3 + "Item {} - path exists and is a file".format(item, path))
    return True




def CheckList(lst, t):
    if type(lst) != list:
        print(" -> " + indent4 + "Default is not a list")
        return False
    for i in lst:
        if type(i) != t:
            print(" -> " + indent4 + "List contains element \"{}\" that is not of type {}".format(i, t)) 
            return False
    return True
        



def TestOptions(minfo):
    print(indent3 + "Checking options")

    if not "options" in minfo:
        print(" -> " + indent3 + "Missing the options item")
        return False


    opt = minfo["options"]

    if type(opt) != dict:
        print(" -> " + indent3 + "Options is not a dictionary")
        return False

    ret = True
    for key,val in opt.items():
        print(indent4 + "Checking {}".format(key))
        if type(key) != str:
            print(" -> " + indent4 + "Key is not a string: {}".format(key))
            ret = False
            continue

        # Check all for errors
        if type(val[0]) != str:
            print(" -> " + indent4 + "Type is not a string")
            ret = False
        elif not val[0] in validtypes:
            print(" -> " + indent4 + "Type {} is not valid".format(val[0]))
            ret = False


        if type(val[2]) != bool:
            print(" -> " + indent4 + "\"required\" is not a bool (is a {})".format(type(val[2])))
            ret = False
        else:
            if val[2] and val[1] != None:
                print(" -> " + indent4 + "Default given for a required option")
                ret = False

        if type(val[4]) != str and val[4] != None:
            print(" -> " + indent4 + "Help string is an invalid type (should be string or None)")
            ret = False
            
        
        # check default type
        if val[1] != None:
            t = val[0]
            if (  (t == "int" and type(val[1]) != int) or
                      (t == "float" and type(val[1]) != float) or
                      (t == "bool" and type(val[1]) != bool) or 
                      (t == "str" and type(val[1]) != str)  ):
                print(" -> " + indent4 + "Default of type {} doesn't match type string \"{}\"".format(type(val[1]), t))
                ret = False
               
             
            if (  (t == "listint" and not CheckList(val[1], int)) or
                      (t == "listfloat" and not CheckList(val[1], float)) or
                      (t == "listbool" and not CheckList(val[1], bool)) or
                      (t == "liststr" and not CheckList(val[1], str))  ):
                ret = False # Stuff is printed in CheckList


            # validate default
            if val[3] != None:
                if not hasattr(val[3], "Validate"):
                    print(" -> " + indent4 + "Validator {} does not have \"Validate()\" function".format(val[3]))
                    ret = False

                if not callable(val[3].Validate):
                    print(" -> " + indent4 + "Validate() function of {} is not callable".format(val[3]))
                    ret = False

                # check for two arguments - one is "self"
                if val[3].Validate.__code__.co_argcount != 2:
                    print(" -> " + indent4 + "Validate() function of {} does not take one argument".format(val[3]))
                    ret = False
    
                else:
                    valid = val[3].Validate(val[1])
                    if not valid:
                        print(" -> " + indent4 + "Default value fails initial validation")
                        ret = False

                if hasattr(val[3], "descstr"):
                    descstr = val[3].descstr
                    if descstr != None and type(descstr) != str:
                        print(" -> " + indent4 + "Validator has descstr member, but it's not a string")
                        ret = False
                else
                    print(indent5 + "No description string, but that's ok")
        
        

    if ret:
        print(indent3 + "Options look ok")
    else:
        print(indent3 + "Errors with the options")

    return ret

        




##########################
# START HERE
##########################

paths = sys.argv[1:]
print()

allok = True
for supermodule in paths:

    try:
        print(indent1 + "Checking supermodule {}".format(supermodule))

        if not os.path.isdir(supermodule):
            print(" -> " + "{}\" does not exist or is not a directory".format(supermodule))
            continue

        mfile = os.path.join(supermodule, "modinfo.py")

        if not os.path.isfile(mfile):
            print(" -> " + "\"{}\" does not exist or is not a file".format(mfile))
            continue


        mod = SourceFileLoader("testload", mfile).load_module()

        if not hasattr(mod, 'minfo'):
            print(" -> " + "\"{}\" does not have the minfo member")
            continue


        # loop over the modules in the supermodule
        for mname,minfo in mod.minfo.items():
            print(indent2 + 'Checking module {}'.format(mname))

            ok = True
            
            ok = ok and TestStringItem(minfo, "type", True)
            ok = ok and TestStringItem(minfo, "version", True)
            ok = ok and TestStringItem(minfo, "description", True)
            ok = ok and TestListStringItem(minfo, "refs", True)
            ok = ok and TestListStringItem(minfo, "authors", True)

            if minfo["type"] == "c_module":
                ok = ok and TestStringItem(minfo, "soname", True)
                ok = ok and TestFile("soname", os.path.join(supermodule, minfo["soname"]))


            ok = ok and TestOptions(minfo)

            if ok:
                print(indent3 + "Module {} looks ok!".format(mname))
            else:
                print(" -> " + indent2 + "PROBLEMS WITH MODULE {}!".format(mname))

        if ok:
            print(indent1 + "Supermodule {} looks ok to me!".format(supermodule))
        else:
            print(" -> " + "ISSUES WITH SUPERMODULE {}!".format(supermodule))
            allok = False
        print()


    if allok:
        print("All supermodules seem ok")
    else:
        print("***Problem detected. Fix it!***")
    except RuntimeError as e:
        print("\n\n")
        print("******************************")
        print("Supermodule {} failed!".format(supermodule))
        print(e)
        print("******************************")




        





