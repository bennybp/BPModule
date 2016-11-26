import os
import sys
import pulsar as psr

# So we can find "testmodules"
thispath = os.path.dirname(os.path.realpath(__file__))
modpath = os.path.dirname(thispath)
sys.path.insert(0, modpath)
print(thispath)

def run_test_inner(tester, ma):
        ma.load_module("testmodules", "TestCppModule1", "TEST_CPP_MOD1")
        ma.load_module("testmodules", "TestCppModule2", "TEST_CPP_MOD2")
        ma.load_module("testmodules", "TestPyModule1", "TEST_PY_MOD1")
        ma.load_module("testmodules", "TestPyModule2", "TEST_PY_MOD2")

        mcpp1 = ma.get_module("TEST_CPP_MOD1", 0)
        mcpp2 = ma.get_module("TEST_CPP_MOD2", 0)
        mpy1 = ma.get_module("TEST_PY_MOD1", 0)
        mpy2 = ma.get_module("TEST_PY_MOD2", 0)
        
        tester.test_call("Test simple C++ module function",
                         True, mcpp1.run_test)

        tester.test_call("Test simple Python module function",
                         True, mpy1.run_test)

        tester.test_call("Test throwing an exception in a C++ module",
                         False, mcpp2.run_test)

        tester.test_call("Test throwing an exception in a Python module",
                         False, mpy2.run_test)


def run_test():

    tester = psr.PyTester("Basic Module Loading/Calling")

    with psr.ModuleAdministrator() as ma:
        run_test_inner(tester, ma)

    tester.print_results()
    return tester.nfailed()
