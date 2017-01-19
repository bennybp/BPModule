import os
import sys
import pulsar as psr

def run_test():

    tester = psr.PyTester("Basic Module Loading/Calling")

    ma = psr.ModuleAdministrator()
    ma.load_module("testmodules", "TestCppModule1", "TEST_CPP_MOD1")
    ma.load_module("testmodules", "TestCppModule2", "TEST_CPP_MOD2")
    ma.load_module("testmodules", "TestCppModule3", "TEST_CPP_MOD3_c1")
    ma.load_module("testmodules", "TestCppModule3", "TEST_CPP_MOD3_c2")
    ma.load_module("testmodules", "TestCppModule3", "TEST_CPP_MOD3_p1")
    ma.load_module("testmodules", "TestCppModule3", "TEST_CPP_MOD3_p2")

    ma.load_module("testmodules", "TestPyModule1", "TEST_PY_MOD1")
    ma.load_module("testmodules", "TestPyModule2", "TEST_PY_MOD2")
    ma.load_module("testmodules", "TestPyModule3", "TEST_PY_MOD3_c1")
    ma.load_module("testmodules", "TestPyModule3", "TEST_PY_MOD3_c2")
    ma.load_module("testmodules", "TestPyModule3", "TEST_PY_MOD3_p1")
    ma.load_module("testmodules", "TestPyModule3", "TEST_PY_MOD3_p2")

    ma.change_option("TEST_CPP_MOD3_c1", "OTHER_MODULE", "TEST_CPP_MOD1")
    ma.change_option("TEST_CPP_MOD3_c2", "OTHER_MODULE", "TEST_CPP_MOD2")
    ma.change_option("TEST_CPP_MOD3_p1", "OTHER_MODULE", "TEST_PY_MOD1")
    ma.change_option("TEST_CPP_MOD3_p2", "OTHER_MODULE", "TEST_PY_MOD2")
    ma.change_option("TEST_PY_MOD3_c1", "OTHER_MODULE", "TEST_CPP_MOD1")
    ma.change_option("TEST_PY_MOD3_c2", "OTHER_MODULE", "TEST_CPP_MOD2")
    ma.change_option("TEST_PY_MOD3_p1", "OTHER_MODULE", "TEST_PY_MOD1")
    ma.change_option("TEST_PY_MOD3_p2", "OTHER_MODULE", "TEST_PY_MOD2")

    mcpp1 = ma.get_module("TEST_CPP_MOD1", 0)
    mcpp2 = ma.get_module("TEST_CPP_MOD2", 0)
    mpy1 = ma.get_module("TEST_PY_MOD1", 0)
    mpy2 = ma.get_module("TEST_PY_MOD2", 0)

    tester.test_call("Test simple C++ module function", True, mcpp1.run_test)
    tester.test_call("Test simple Python module function", True, mpy1.run_test)
    tester.test_call("Test throwing an exception in a C++ module", False, mcpp2.run_test)
    tester.test_call("Test throwing an exception in a Python module", False, mpy2.run_test)

    mcpp3_c1 = ma.get_module("TEST_CPP_MOD3_c1", 0)
    mcpp3_c2 = ma.get_module("TEST_CPP_MOD3_c2", 0)
    mcpp3_p1 = ma.get_module("TEST_CPP_MOD3_p1", 0)
    mcpp3_p2 = ma.get_module("TEST_CPP_MOD3_p2", 0)

    tester.test_call("C++ calling a C++ module", True, mcpp3_c1.run_test)
    tester.test_call("C++ calling a C++ module that throws", False, mcpp3_c2.run_test)
    tester.test_call("C++ calling a Python module", True, mcpp3_p1.run_test)
    tester.test_call("C++ calling a Python module that throws", False, mcpp3_p2.run_test)

    mpy3_c1 = ma.get_module("TEST_PY_MOD3_c1", 0)
    mpy3_c2 = ma.get_module("TEST_PY_MOD3_c2", 0)
    mpy3_p1 = ma.get_module("TEST_PY_MOD3_p1", 0)
    mpy3_p2 = ma.get_module("TEST_PY_MOD3_p2", 0)

    tester.test_call("Python calling a C++ module", True, mpy3_c1.run_test)
    tester.test_call("Python calling a C++ module that throws", False, mpy3_c2.run_test)
    tester.test_call("Python calling a Python module", True, mpy3_p1.run_test)
    tester.test_call("Python calling a Python module that throws", False, mpy3_p2.run_test)

    tester.print_results()
    return tester.nfailed()
