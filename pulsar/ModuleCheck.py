#!/usr/bin/env python3

import os
import sys
from  importlib.machinery import SourceFileLoader

from pulsar.output import *
from pulsar.exception import GeneralException


indent = "    "
indent1 = indent*1
indent2 = indent*2
indent3 = indent*3
indent4 = indent*4
indent5 = indent*5



def TestStringItem(minfo, item, required):
    if not item in minfo:
        if required:
            print_global_error(" -> " + indent2 + "Item {} does not exist!\n".format(item))
            return False
        else:
            print_global_debug(indent3 + "Item {} does not exist, but that's ok\n".format(item))
            return True

    if type(minfo[item]) != str:
        print_global_error(" -> " + indent2 + "Item {} is not a string!\n".format(item))
        return False
    
    print_global_debug(indent3 + "Item {} looks ok\n".format(item))
    return True




def TestListStringItem(minfo, item, required):
    if not item in minfo:
        if required:
            print_global_error(" -> " + indent2 + "Item {} does not exist!\n".format(item))
            return False
        else:
            print_global_debug(indent3 + "Item {} does not exist, but that's ok\n".format(item))
            return True

    if type(minfo[item]) != list:
        print_global_error(" -> " + indent2 + "Item {} is not a list!\n".format(item))
        return False
        

    ret = True

    # should be ok if empty
    for i in minfo[item]:
        if type(i) != str:
            print_global_error(" -> " + indent2 + "Item {} , element {} is not a string!\n".format(item, i))
            ret = False

    if ret:    
        print_global_debug(indent3 + "Item {} looks ok\n".format(item))

    return ret




def TestFile(item, path):
    if not os.path.isfile(path):
        print_global_error(" -> " + indent2 + "Item {} - path is not a file: {}\n".format(item, path))
        return False
    else:
        print_global_debug(indent3 + "Item {} - path exists and is a file: {}\n".format(item, path))
    return True




def CheckList(lst, t):
    if type(lst) != list:
        print_global_error(" -> " + indent4 + "Default is not a list\n")
        return False
    for i in lst:
        if type(i) != t:
            print_global_error(" -> " + indent4 + "List contains element \"{}\" that is not of type {}\n".format(i, t)) 
            return False
    return True
        



def TestOptions(minfo):
    print_global_debug(indent3 + "Checking options\n")

    if not "options" in minfo:
        print_global_error(" -> " + indent3 + "Missing the options item\n")
        return False


    opt = minfo["options"]

    if type(opt) != dict:
        print_global_error(" -> " + indent3 + "Options is not a dictionary\n")
        return False

    ret = True
    for key,val in opt.items():
        print_global_debug(indent4 + "Checking {}\n".format(key))
        if type(key) != str:
            print_global_error(" -> " + indent4 + "Key is not a string: {}\n".format(key))
            ret = False
            continue

        # Check all for errors
        if type(val[0]) != str:
            print_global_error(" -> " + indent4 + "Type is not a string\n")
            ret = False
        elif not val[0] in validtypes:
            print_global_error(" -> " + indent4 + "Type {} is not valid\n".format(val[0]))
            ret = False


        if type(val[2]) != bool:
            print_global_error(" -> " + indent4 + "\"required\" is not a bool (is a {})\n".format(type(val[2])))
            ret = False
        else:
            if val[2] and val[1] != None:
                print_global_error(" -> " + indent4 + "Default given for a required option\n")
                ret = False

        if type(val[4]) != str and val[4] != None:
            print_global_error(" -> " + indent4 + "Help string is an invalid type (should be string or None)\n")
            ret = False
            
        
        # check default type
        if val[1] != None:
            t = val[0]
            if (  (t == "int" and type(val[1]) != int) or
                      (t == "float" and type(val[1]) != float) or
                      (t == "bool" and type(val[1]) != bool) or 
                      (t == "str" and type(val[1]) != str)  ):
                print_global_error(" -> " + indent4 + "Default of type {} doesn't match type string \"{}\"\n".format(type(val[1]), t))
                ret = False
               
             
            if (  (t == "listint" and not CheckList(val[1], int)) or
                      (t == "listfloat" and not CheckList(val[1], float)) or
                      (t == "listbool" and not CheckList(val[1], bool)) or
                      (t == "liststr" and not CheckList(val[1], str))  ):
                ret = False # Error info is output in CheckList


            # validate default
            if val[3] != None:
                if not hasattr(val[3], "Validate"):
                    print_global_error(" -> " + indent4 + "Validator {} does not have \"Validate()\" function\n".format(val[3]))
                    ret = False

                if not callable(val[3].Validate):
                    print_global_error(" -> " + indent4 + "Validate() function of {} is not callable\n".format(val[3]))
                    ret = False

                # check for two arguments - one is "self"
                if val[3].Validate.__code__.co_argcount != 2:
                    print_global_error(" -> " + indent4 + "Validate() function of {} does not take one argument\n".format(val[3]))
                    ret = False
    
                else:
                    valid = val[3].Validate(val[1])
                    if not valid:
                        print_global_error(" -> " + indent4 + "Default value fails initial validation\n")
                        ret = False

                if hasattr(val[3], "descstr"):
                    descstr = val[3].descstr
                    if descstr != None and type(descstr) != str:
                        print_global_error(" -> " + indent4 + "Validator has descstr member, but it's not a string\n")
                        ret = False
                else:
                    print_global_debug(indent5 + "No description string, but that's ok\n")
        

    if ret:
        print_global_debug(indent3 + "Options look ok\n")
    else:
        print_global_error(indent3 + "Errors with the options\n")

    return ret

        



def check_supermodule(supermodule):
    print_global_debug(indent1 + "Checking supermodule {}\n".format(supermodule))

    if not os.path.isdir(supermodule):
        print_global_error(" -> " + "{}\" does not exist or is not a directory\n".format(supermodule))

    mfile = os.path.join(supermodule, "modinfo.py")

    if not os.path.isfile(mfile):
        print_global_error(" -> " + "\"{}\" does not exist or is not a file\n".format(mfile))

    print_global_warning(indent1 + "REST OF CHECKSUPERMODULE NOT YET IMPLEMENTED")
    return






    mod = SourceFileLoader("testload", mfile).load_module()

    if not hasattr(mod, 'minfo'):
        print_global_error(" -> " + "\"{}\" does not have the minfo member\n")

    allok = True

    # loop over the modules in the supermodule
    for mname,minfo in mod.minfo.items():
        print_global_debug(indent2 + 'Checking module {}\n'.format(mname))

        ok = True

        if type(mname) != str:
            print_global_error(" -> " + indent2 + "Module key is not a string")
            ok = False            

        
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
            print_global_debug(indent3 + "Module {} looks ok!\n".format(mname))
        else:
            print_global_error(" -> " + indent2 + "PROBLEMS WITH MODULE {}!\n".format(mname))
            allok = False

    if allok:
        print_global_debug(indent1 + "Supermodule {} looks ok to me!\n".format(supermodule))
    else:
        raise GeneralException("Problem in initial check of a supermodule", "supermodule", supermodule)

