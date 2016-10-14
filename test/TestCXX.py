from TestFxns import *
import sys

Test_Name="TestPoint"

def run():
    minfo = {
        Test_Name:{
            "type"        : "c_module",
            "base"        : "Test_Base",
            "version"     : "0.1a",
            "description" : "Tests Stuff",
            "authors"     : [""],
            "refs"        : [""],
            "options"     : {}
        }
    }

    mm.load_module("TestPoint","TestPoint","TestPoint")
    my_mod=mm.get_module("TestPoint",0)
    my_mod.run_test()


with psr.ModuleAdministrator() as mm:
    run()
psr.finalize()

