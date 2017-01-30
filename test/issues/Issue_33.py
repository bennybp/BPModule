import os
import sys
import pulsar as psr

def run_test():

    tester = psr.PyTester("Basic Module Loading/Calling")

    ma = psr.ModuleAdministrator()
    ma.load_module("testmodules", "TestCppModule1", "TEST_CPP_MOD1")
    ma.load_module("testmodules", "TestPyModule1", "TEST_PY_MOD1")

    mcpp1 = ma.get_module("TEST_CPP_MOD1", 0)
    mpy1 = ma.get_module("TEST_PY_MOD1", 0)

    tester.test_call("Test simple C++ module function", True, mcpp1.run_test)
    tester.test_call("Test simple Python module function", True, mpy1.run_test)
    tester.test_call("Test chained get_module with C++ module", True, ma.get_module("TEST_CPP_MOD1", 0).run_test)
    tester.test_call("Test chained get_module with Python module", True, ma.get_module("TEST_PY_MOD1", 0).run_test)

    tester.print_results()
    return tester.nfailed()
